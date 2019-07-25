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
bool isNoiseBufferFulled = false;

char chBuffer[BUFFER_SIZE];
char currChar;
int chBufferIdx = 0;

Ticker ticker;

void dumpNoiseIfCommanded(string inp) {
  if (inp.find(STR_DUMP) != string::npos) {
    pc.printf("\n");

    for (int i = noiseBufferIdx; i >= 0; i--) {
      pc.printf("%d", noiseBuffer[i]);
    }
    if (isNoiseBufferFulled) {
      for (int i = NOISE_BUFFER_SIZE - 1; i > noiseBufferIdx; i--) {
        pc.printf("%d", noiseBuffer[i]);
      }
    }
    pc.printf("\n");
  }
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
    noiseBufferIdx = 0;
  }
}

int main() {
  pc.printf("Hello World!\n\r");
  ticker.attach(&tickTock, NOISE_SAMPLING_PERIOD);
  noiseInterrupt.rise(&noiseOccured);
  // pc.attach(&keyPressed, Serial::RxIrq);

  // spin in a main loop
  while (1) {
    char ch = pc.getc();
    if (ch == '\n' || ch == '\r') {
      pc.printf(chBuffer);
      pc.printf("\n");
      dumpNoiseIfCommanded(chBuffer);
      chBufferIdx = 0;
      memset(chBuffer, 0, sizeof chBuffer);
    } else {
      chBuffer[chBufferIdx] = ch;
      chBufferIdx++;
    }
  }
}