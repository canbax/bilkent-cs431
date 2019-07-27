#include "mbed.h"
#include <string>

using namespace std;

Serial pc(USBTX, USBRX); // tx, rx
Ticker ticker;
AnalogIn analogInp(A0); // p15

const float NOISE_SAMPLING_PERIOD = 1.0;
bool isTicked = false;

void tickTock() { isTicked = true; }

int main() {
  pc.baud(115200);
  pc.printf("Hello World!\n\r");
  ticker.attach(&tickTock, NOISE_SAMPLING_PERIOD);

  // spin in a main loop
  while (1) {
    if (isTicked) {
      float f = analogInp.read();
      pc.printf(" %.2f ", f);
      isTicked = false;
    }
  }
}