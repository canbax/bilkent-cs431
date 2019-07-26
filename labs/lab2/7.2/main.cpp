#include "mbed.h"
#include <string>

using namespace std;

Serial pc(USBTX, USBRX); // tx, rx
AnalogIn analogInp(A0);
AnalogOut analogOup(p18);
const float pseudoAnalogInp[10] = {0.1, 0.2, 0.3, 0.4, 0.5,
                                   0.6, 0.7, 0.8, 0.9, 1};
int idx = 0;

void fake_read_adc() {
  analogOup.write(pseudoAnalogInp[idx]);
  idx++;
  if (idx == 10) {
    idx = 0;
  }

  float f = analogInp.read();
  pc.printf(" %.2f ", f);
}

int main() {
  pc.printf("Hello World!\n\r");

  // spin in a main loop
  while (1) {
    char c = pc.getc();
    if (c == '\n' || c == '\r') {
      fake_read_adc();
    }
  }
}