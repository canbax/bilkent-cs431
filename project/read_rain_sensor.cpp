#include "mbed.h"

InterruptIn button(p5);
DigitalOut led(LED1);
DigitalOut flash(LED4);

Serial pc(USBTX, USBRX);

bool isRainDropped = false;

void flip() {
  isRainDropped = true;
  led = !led;
}

int main() {
  // attach the address of the flip function to the rising edge
  button.rise(&flip);
  while (1) { // wait around, interrupts will interrupt this!
    flash = !flash;
    wait(0.25);
    if (isRainDropped) {
      pc.printf(" rain dropped ");
      isRainDropped = false;
    }
  }
}