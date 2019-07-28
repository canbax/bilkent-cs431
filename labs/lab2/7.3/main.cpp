#include "mbed.h"
#include <string>

using namespace std;

Serial pc(USBTX, USBRX); // tx, rx
Ticker ticker;
AnalogIn analogInp(p20); // p15

const int BAUD_RATE = 921600;
const float NOISE_SAMPLING_PERIOD = 0.00025; // 4kHz
bool isTicked = false;

void tickTock() { isTicked = true; }

int main() {
  pc.baud(BAUD_RATE);
  pc.printf("Hello World!\n\r");
  ticker.attach(&tickTock, NOISE_SAMPLING_PERIOD);

  // spin in a main loop
  while (1) {
    if (isTicked) {
      float f = analogInp.read();
      if (f > 0.5) {
        pc.printf("1");
      } else {
        // if there is a clap it will print here
        pc.printf("0");
      }
      isTicked = false;
    }
  }
}