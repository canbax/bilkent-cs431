#include "mbed.h"
#include <string>
using namespace std;
Serial pc(USBTX, USBRX); // tx, rx

PwmOut led1(LED1);
PwmOut led2(LED2);
PwmOut led3(LED3);
PwmOut led4(LED4);

PwmOut leds[4] = {led1, led2, led3, led4};

char buffer[1024];
int charIdx = 0;
const int LED_CNT = 4;
const string LED_STRS[LED_CNT] = {"one", "two", "three", "four"};

void setLedsFromStr(string inp) {
  for (int i = 0; i < LED_CNT; i++) {
    if (inp.find(LED_STRS[i]) != string::npos) {
      leds[i] = 1.0 - leds[i];
    }
  }
}

int main() {
  pc.printf("Hello World!\n\r");
  led1 = 0.0;
  led2 = 0.1;
  led3 = 0.5;
  led4 = 0.9;

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