#include <stdlib.h> /* atoi */
#include <stdio.h>
#include <time.h>

#include "mbed.h"
#include "EthernetNetIf.h"
#include "HTTPServer.h"
#include "dht22.h"
#include "StepperMotor.h"
#include "HttpController.h"
#include "globalVariable.h"

DHT dht22(p23);
EthernetNetIf eth;
HTTPServer svr;
Serial pc(USBTX, USBRX);                      // tx, rx
StepperMotor stepperMotor(p9, p10, p11, p12); // creates new stepper motor: IN1, IN2, IN3, IN4
// to set fan speed
PwmOut fan(p21);
// to sense rain
InterruptIn rainSensor(p6);
LocalFileSystem local("local");
FILE *logFile;

// to get fan speed (RPM)
InterruptIn tach(p5);
bool isRainDropped = false;
int lastRainTime = -1;
int prevRainTime = -1;
int currTemperature = -1;
int currHumidity = -1;

// required
int requiredTemperature = 20;
int requiredHumidity = 20;

// variables for fan
float fanValue = 0.1f;
int rotationCnt = 0;
int prevAngle = 0;
int currAngle = 0;
int currTime = 0;
Ticker ticker;
Ticker dhtTicker;

char commandStr[32];
int strIdx = -1;
bool isRoofOpen = true;
bool isControlByValues = true;

const string currentDateTime()
{
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
  tstruct = *localtime(&now);
  // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
  // for more information about date/time format
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

  return buf;
}

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
  // pc.printf("lastraintime=%d\tprevraintime=%d\tcurrtime=%d\tisroofopen=%d\n",lastRainTime,prevRainTime,currTime,isRoofOpen);
  HttpController::setFanRps(currAngle - prevAngle);
}

void keyboardInp()
{
  strIdx++;
  commandStr[strIdx] = pc.getc();
  pc.printf(&commandStr[strIdx]);
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
  prevRainTime = lastRainTime;
  lastRainTime = currTime;
  pc.printf("\n--------------------------------------- rained ------------------------------------");
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

  svr.addHandler<SimpleHandler>("/hello"); //Default handler
  svr.addHandler<HttpController>("/");     //Default handler
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

bool isExist()
{
  FILE *file;
  if (file = fopen("/local/out.txt", "r"))
  {
    fclose(file);
    return 1;
  }
  return 0;
}

void setUpLogSystem()
{
  bool b = isExist();
  if (b)
  {
    pc.printf(" log file exists ");
    logFile = fopen("/local/out.txt", "a");
  }
  else
  {
    pc.printf(" log file DOES NOT exists ");
    logFile = fopen("/local/out.txt", "w");
  }
  fclose(logFile);
}

void printTempratureAndHumidity()
{
  printf("Temperature: %d Celcius\r\n", currTemperature);
  printf("Humidity: %d, \r\n", currHumidity);
}

void readDHT22()
{
  int error = 0;

  error = dht22.readData();
  if (0 == error)
  {
    currTemperature = dht22.ReadTemperature();
    currHumidity = dht22.ReadHumidity();
    HttpController::setTemperatureAndHumidity(currTemperature, currHumidity);
    // string s = currentDateTime();
    // time_t t = time(NULL);
    // logFile = fopen("/local/out.txt", "a");
    // fprintf(logFile, "\n %d, t: %d, h: %d", currTime, currTemperature, currHumidity);
    // fclose(logFile);
  }
  else
  {
    printf("Error: %d\r\n", error);
  }
}

void setRequiredTemperature(int t)
{
  requiredTemperature = t;
}

void setRequiredHumidity(int h)
{
  requiredHumidity = h;
}

void openCloseRoof(bool shouldOpen)
{
  // check if already in the state
  if ((isRoofOpen && shouldOpen) || (!isRoofOpen && !shouldOpen))
  {
    return;
  }
  isRoofOpen = !isRoofOpen;
  HttpController::setRoofState(isRoofOpen);
  int direction = 1;
  if (shouldOpen)
  {
    direction = 0;
  }
  if (shouldOpen)
  {
    pc.printf("\nopening roof...");
  }
  else
  {
    pc.printf("\nclosing roof...");
  }
  stepperMotor.step(512, direction, 1200);
}

void controlByRequiredValues()
{
  int diff = currTemperature - requiredTemperature;
  if (diff > 0)
  {
    fan = float(currTemperature - requiredTemperature) / 10.0f;
  }
  else
  {
    fan = 0;
  }

  // rain drops for 2 consequtive times
  if (lastRainTime > 0 && lastRainTime - prevRainTime < 5 && currTime - lastRainTime < 5 && isRoofOpen)
  {
    // close if rain starts
    openCloseRoof(false);
  } // no rain for 1 min
  else if (currTime - lastRainTime > 10 && !isRoofOpen)
  {
    // open if rain ends
    openCloseRoof(true);
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
    if (lastRainTime < 0)
    {
      pc.printf("never rained before");
    }
    else
    {
      pc.printf("%d  seconds ago rained", currTime - lastRainTime);
    }
    break;
  // control stepper
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
    printTempratureAndHumidity();
    break;
  // control requiredTemprature and requiredHumidity
  case 'c':
  case 'C':
    if (commandStr[1] == '!')
    {
      isControlByValues = !isControlByValues;
      pc.printf("\nisControlByValues: %d", isControlByValues);
    }
    else if (commandStr[2] == 't' || commandStr[2] == 'T')
    {
      int t = atoi(&commandStr[3]);
      pc.printf("read t : %d", t);
      setRequiredTemperature(t);
    }
    else if (commandStr[2] == 'h' || commandStr[2] == 'H')
    {
      int h = atoi(&commandStr[3]);
      pc.printf("read h : %d", h);
      setRequiredHumidity(h);
    }

    break;
  default:
    break;
  }
}

void listenCommands()
{
  if (commandStr[strIdx] == '\n' || commandStr[strIdx] == '\r')
  {
    processCommand();
    strIdx = -1;
    memset(commandStr, 0, sizeof commandStr);
  }
}

void setupRainSensor()
{
  rainSensor.rise(&rainDropped);
}

void setUpDht22()
{
  dhtTicker.attach(&readDHT22, 5.0);
}

void listenEthernet()
{
  Net::poll();
}

void setup()
{
  setupEthernet();
  setupFan();
  setupRainSensor();
  setupController();
  setUpDht22();
  setUpLogSystem();
}

int main()
{
  pc.printf("starting Green House Air Conditioner...\n");
  setup();

  pc.printf("running Green House Air Conditioner...\n");
  while (1)
  {
    listenEthernet();
    listenCommands();
    if (isControlByValues)
    {
      controlByRequiredValues();
    }
  }
  return 0;
}
