#include "mbed.h"
#include <string>

using namespace std;

Serial pc(USBTX, USBRX); // tx, rx
InterruptIn noiseInterrupt(p5);
DigitalOut led(LED1);
DigitalOut led2(LED2);

const string STR_DUMP = "dumpfifo";
const int BUFFER_SIZE = 1024;
const int NOISE_BUFFER_SIZE = 32;
const float NOISE_SAMPLING_PERIOD = 1.0;

unsigned int noiseBuffer[NOISE_BUFFER_SIZE];
int noiseBufferIdx = 0;

char chBuffer[BUFFER_SIZE];
int chBufferIdx = 0;

Ticker ticker;

void tickTock() {
  led = !led;
  noiseBufferIdx++;
  if (noiseBufferIdx == NOISE_BUFFER_SIZE) {
    noiseBufferIdx = 0;
  }
}

void dumpNoiseIfCommanded(string inp) {
  if (inp.find(STR_DUMP) != string::npos) {
    pc.printf("\n");
    for (int i = 0; i < noiseBufferIdx; i++) {
      pc.printf("%d", noiseBuffer[i]);
    }
    pc.printf("\n");
  }
}

void noiseOccured() {
  led2 = !led2;
  noiseBuffer[noiseBufferIdx] = 1;
}

int main() {
  pc.printf("Hello World!\n\r");
  ticker.attach(&tickTock, NOISE_SAMPLING_PERIOD);
  noiseInterrupt.rise(&noiseOccured);

  // spin in a main loop
  while (1) {
    char c = pc.getc();

    if (c == '\n' || c == '\r') {
      pc.printf(chBuffer);
      pc.printf("\n");
      dumpNoiseIfCommanded(chBuffer);
      chBufferIdx = 0;
      noiseBufferIdx = 0;
      memset(chBuffer, 0, sizeof chBuffer);
      memset(noiseBuffer, 0, sizeof noiseBuffer);
    } else {
      chBuffer[chBufferIdx] = c;
      chBufferIdx++;
    }
  }
}