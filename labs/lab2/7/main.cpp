#include "mbed.h"
#include <string>

using namespace std;

Serial pc(USBTX, USBRX); // tx, rx
InterruptIn noiseInterrupt(p5);
DigitalOut led(LED1);
DigitalOut led2(LED2);

const string STR_DUMP = "dumpfifo";
const int BUFFER_SIZE = 1024;
const int NOISE_BUFFER_SIZE = 20;
const float NOISE_SAMPLING_PERIOD = 0.2;

unsigned int noiseBuffer[NOISE_BUFFER_SIZE];
int noiseBufferIdx = 0;
bool isNoiseBufferFulled = false;
bool isCycled = false;

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
  pc.printf(" %d ", cnt);
}

void noiseOccured() {
  led2 = !led2;
  noiseBuffer[noiseBufferIdx] = 1;
}

void tickTock() {
  led = !led;
  noiseBufferIdx++;
  noiseBuffer[noiseBufferIdx] = 0;
  if (noiseBufferIdx == NOISE_BUFFER_SIZE) {
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
      noiseBufferIdx = 0;
      isCycled = false;
    }
  }
}