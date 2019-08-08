
#include "mbed.h"
#include "EthernetNetIf.h"
#include "HTTPServer.h"
#include "dht22.h"
#include "StepperMotor.h"

DHT dht22(p23);
EthernetNetIf eth;
HTTPServer svr;
Serial pc(USBTX, USBRX);                      // tx, rx
StepperMotor stepperMotor(p9, p10, p11, p12); // creates new stepper motor: IN1, IN2, IN3, IN4
// to set fan speed
PwmOut fan(p21);
// to sense rain
InterruptIn rainSensor(p6);
// to get fan speed (RPM)
InterruptIn tach(p5);
bool isRainDropped = false;
int lastRainTime = -1;
int lastTemperature = -1;
int lastHumidity = -1;

// variables for fan
float fanValue = 0.1f;
int rotationCnt = 0;
int prevAngle = 0;
int currAngle = 0;
int currTime = 0;
Ticker ticker;

char commandStr[32];
int strIdx = 0;

// functions for fan
void rotated()
{
  rotationCnt++;
}

void tick()
{
  currTime++;
  prevAngle = currAngle;
  currAngle = rotationCnt / 2;
}

void keyboardInp()
{
  commandStr[strIdx] = pc.getc();
  strIdx++;
}

void oupFanSpeed()
{
  // rotation per second
  int rps = currAngle - prevAngle;
  pc.printf(" rps = %d ", rps);
}

void setFanSpeed(float speed)
{
  fan = speed;
}
// end of functions for fan

void rainDropped()
{
  isRainDropped = true;
  lastRainTime = currTime;
}

int setupEthernet()
{
  printf("Setting up ethernet... \n");
  EthernetErr ethErr = eth.setup(15000);
  if (ethErr)
  {
    printf("Error %d in setup.\n", ethErr);
    return -1;
  }
  printf("Setup OK\n");

  svr.addHandler<SimpleHandler>("/"); //Default handler
  svr.bind(80);

  printf("Listening...\n");
  return 0;
}

int setupFan()
{
  fan.period(0.00004);
  // 2 rise per rotation
  tach.rise(&rotated);
  ticker.attach(&tick, 1.0);
  return 0;
}

void setupController()
{
  pc.attach(&keyboardInp, Serial::RxIrq);
}

void listenCommands()
{
  if (commandStr[strIdx] == '\n' || commandStr[strIdx] == '\r')
  {
    processCommand();
    strIdx = 0;
    memset(commandStr, 0, sizeof commandStr);
  }
}

void processCommand()
{
  char c = commandStr[0];
  switch (c)
  {
  // means fan
  case 'f':
  case 'F':
    // means get fan speed
    if (commandStr[2] == 'g' || commandStr[2] == 'G')
    {
      oupFanSpeed();
    } // means set fan speed
    else if (commandStr[2] == 's' || commandStr[2] == 'S')
    {
      float f = float(commandStr[4] - '0') / 10.0f;
      setFanSpeed(f);
    }
    break;
  // means rain sensor
  case 'r':
  case 'R':
    pc.printf("%d  seconds ago rained", currTime - lastRainTime);
    break;
  case 's':
  case 'S':
    int direction = commandStr[2] - '0';
    int cntOf90Degree = commandStr[4] - '0';
    int stepCnt = cntOf90Degree * 128; // 512 steps for 360 degree
    stepperMotor.step(stepCnt, direction, 1200);
    break;
  // DHT22
  case 'd':
  case 'D':
    readDHT22();
    break;
  default:
    break;
  }
}

void setupRainSensor()
{
  rainSensor.rise(&rainDropped);
}

void listenEthernet()
{
  Net::poll();
}

void readDHT22()
{
  int error = 0;
  float dp = 0.0f;

  error = dht22.readData();
  if (0 == error)
  {
    lastTemperature = dht22.ReadTemperature(CELCIUS);
    lastHumidity = dht22.ReadHumidity();
    dp = dht22.CalcdewPoint(c, h);

    printf("Temperature in Celcius: %d \r\n", c);
    printf("Humidity is %d, Dewpoint: %4.2f\r\n\n", h, dp);
  }
  else
  {
    printf("Error: %d\r\n", error);
  }
}

void setup()
{
  setupEthernet();
  setupFan();
  setupRainSensor();
}

int main()
{
  printf("starting Green House Air Conditioner...\n");
  setup();

  printf("running Green House Air Conditioner...\n");
  while (1)
  {
    listenEthernet();
    listenCommands();
  }
  return 0;
}
