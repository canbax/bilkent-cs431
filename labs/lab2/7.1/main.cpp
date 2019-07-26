#include "mbed.h"
#include <string>

using namespace std;

Serial pc(USBTX, USBRX); // tx, rx
AnalogIn analogInp(A0);

int main() {
  pc.printf("Hello World!\n\r");

  // spin in a main loop
  while (1) {
    char c = pc.getc();
    if (c == '\n' || c == '\r') {
      float f = analogInp.read();
      pc.printf(" %.2f ", f);
    }
  }
}