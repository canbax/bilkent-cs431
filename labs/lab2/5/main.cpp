#include "mbed.h"
#include <string>

using namespace std;

Serial pc(USBTX, USBRX); // tx, rx
Ticker flipper;
DigitalOut led1(LED1);
const char STR1[5] = {'Y', 'U', 'P', 'P', 'I'};
bool isFlipped = false;

void flip() {
    isFlipped = true;
}

int main() {
  pc.printf("Hello World!\n\r");

  flipper.attach(&flip, 2.0); // the address of the function to be attached

  // spin in a main loop. flipper will interrupt it to call flip
  while (1) {
    if (isFlipped) {
        pc.printf(STR1);
        isFlipped = false;
    }
    
  }
}