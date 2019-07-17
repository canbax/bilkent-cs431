#include "mbed.h"
#include <string>
using namespace std;
Serial pc(USBTX, USBRX); // tx, rx

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

DigitalOut leds[4] = {led1, led2, led3, led4};

char buffer[1024];
int charIdx = 0;
const int LED_CNT = 4;
const string LED_STRS[LED_CNT] = {"one", "two", "three", "four"};

void setLedsFromStr(string inp) {
    for (int i = 0; i < LED_CNT; i++) {
        if (inp.find(LED_STRS[i]) != string::npos) {
            leds[i] = !leds[i];
        }
    } 
}

int main() {
  pc.printf("Hello World!\n\r");
  while (1) {
    char c = pc.getc();

    if (c == '\n' || c == '\r') {
      pc.printf(buffer);
      pc.printf("\n");
      setLedsFromStr(buffer);
      charIdx = 0;
      memset(buffer, 0, sizeof buffer);
    } else {
      buffer[charIdx] = c;
      charIdx++;
    }
  }
}