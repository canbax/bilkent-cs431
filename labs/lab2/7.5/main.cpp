#include "mbed.h"
#include <string>

using namespace std;

Serial pc(USBTX, USBRX); // tx, rx
Ticker ticker;
AnalogIn analogInp(p20);

const int BAUD_RATE = 921600;
const string STR_DUMP = "d";
const string STR_AVG = "a";
const int CHAR_ARR_SIZE = 1024;
const int ANALOG_INP_SIZE = 1000;
const float SAMPLING_PERDIOD = 0.01; // 1kHz
bool isTicked = false;

float analogInputs[ANALOG_INP_SIZE];
int analogIdx = 0;
bool isNoiseBufferFulled = false;

char charArr[CHAR_ARR_SIZE];
char currChar;
int charIdx = 0;

void dumpIfCommanded(string inp) {
  if (inp.find(STR_DUMP) != string::npos) {
    pc.printf("\n");

    for (int i = analogIdx; i >= 0; i--) {
      // pc.printf(" %.1f ", analogInputs[i]);
      if (analogInputs[i] < 0.5f) {
        pc.printf("0");
      } else {
        pc.printf("1");
      }
    }
    if (isNoiseBufferFulled) {
      for (int i = ANALOG_INP_SIZE - 1; i > analogIdx; i--) {
        // pc.printf(" %.1f ", analogInputs[i]);
        if (analogInputs[i] < 0.5f) {
          pc.printf("0");
        } else {
          pc.printf("1");
        }
      }
    }
    pc.printf("\n");
  }
  if (inp.find(STR_AVG) != string::npos) {
    float tot = 0.0;
    for (int i = analogIdx; i >= 0; i--) {
      tot += analogInputs[i];
    }
    if (isNoiseBufferFulled) {
      for (int i = ANALOG_INP_SIZE - 1; i > analogIdx; i--) {
        tot += analogInputs[i];
      }
    }
    float f = tot / ANALOG_INP_SIZE;
    pc.printf("\n%.4f\n", f);
  }
}

void tickTock() {
  analogInputs[analogIdx] = analogInp.read();
  analogIdx++;
  if (analogIdx == ANALOG_INP_SIZE) {
    isNoiseBufferFulled = true;
    analogIdx = 0;
  }
}

int main() {
  pc.baud(BAUD_RATE);
  pc.printf("Hello World!\n\r");
  ticker.attach(&tickTock, SAMPLING_PERDIOD);

  // spin in a main loop
  while (1) {
    char ch = pc.getc();
    if (ch == '\n' || ch == '\r') {
      pc.printf(charArr);
      pc.printf("\n");
      dumpIfCommanded(charArr);
      charIdx = 0;
      memset(charArr, 0, sizeof charArr);
    } else {
      charArr[charIdx] = ch;
      charIdx++;
    }
  }
}