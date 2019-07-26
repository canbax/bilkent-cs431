#include "mbed.h"
#include <string>

using namespace std;

Serial pc(USBTX, USBRX); // tx, rx
InterruptIn noiseInterrupt(p5);
DigitalOut led(LED1);
DigitalOut led2(LED2);
PwmOut led3(LED3);

const string STR_DUMP = "dumpfifo";
const int BUFFER_SIZE = 1024;
const int NOISE_BUFFER_SIZE = 20;
const float NOISE_SAMPLING_PERIOD = 0.2;

unsigned int noiseBuffer[NOISE_BUFFER_SIZE];
int noiseBufferIdx = 0;
bool isNoiseBufferFulled = false;
bool isCycled = false;
bool isNoiseOccured = false;
int maxNoiseCount = -1;

char chBuffer[BUFFER_SIZE];
char currChar;
int chBufferIdx = 0;

Ticker ticker;

void dumpNoise() {

  int cnt = 0;

  for (int i = noiseBufferIdx; i >= 0; i--) {
    if (noiseBuffer[i]) {
      cnt++;
    }
  }
  if (isNoiseBufferFulled) {
    for (int i = NOISE_BUFFER_SIZE - 1; i > noiseBufferIdx; i--) {
      if (noiseBuffer[i]) {
        cnt++;
      }
    }
  }
  if (cnt > maxNoiseCount) {
    maxNoiseCount = cnt;
  }
  if (cnt == 0) {
    float f = float(maxNoiseCount) / NOISE_BUFFER_SIZE;
    led3 = f;
    maxNoiseCount = 0;
    pc.printf(" maxNoiseCount: %d, brightness: %.2f", maxNoiseCount, f);
  }
  pc.printf("cnt %d ", cnt);
}

void noiseOccured() {
  led2 = !led2;
  isNoiseOccured = true;
}

void tickTock() {
  led = !led;
  if (isNoiseOccured) {
    noiseBuffer[noiseBufferIdx] = 1;
  } else {
    noiseBuffer[noiseBufferIdx] = 0;
  }
  isNoiseOccured = false;
  noiseBufferIdx++;
  
  if (noiseBufferIdx == NOISE_BUFFER_SIZE) {
    noiseBufferIdx = 0;
    isNoiseBufferFulled = true;
    isCycled = true;
  }
}

int main() {
  pc.printf("Hello World!\n\r");
  ticker.attach(&tickTock, NOISE_SAMPLING_PERIOD);
  noiseInterrupt.rise(&noiseOccured);

  // spin in a main loop
  while (1) {
    if (isCycled) {
      dumpNoise();
      isCycled = false;
    }
  }
}