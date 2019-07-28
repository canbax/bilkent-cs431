#include "mbed.h"
#include <string>

using namespace std;

Serial pc(USBTX, USBRX); // tx, rx
Ticker ticker;
AnalogIn analogInp(p20);

PwmOut led1(LED1);
PwmOut led2(LED2);
PwmOut led3(LED3);
PwmOut led4(LED4);

const int BAUD_RATE = 921600;
const string STR_DUMP = "d";
const string STR_AVG = "a";
const string STR_CNT = "c";
const int CHAR_ARR_SIZE = 32;
const int ANALOG_INP_SIZE = 4000;
// to make more precise counting decrease sampling period
const float SAMPLING_PERDIOD = 0.0025; 
bool isTicked = false;

float analogInputs[ANALOG_INP_SIZE];
int analogIdx = 0;
bool isNoiseBufferFulled = false;

char charArr[CHAR_ARR_SIZE];
char currChar;
int charIdx = 0;
int currCntLessThanAvg = 0;
float currAnalogSum = 0.0;
int maxCnt = 0;

// try to count number of claps by assuming debounce will happen and by quantization of input signals
// to make more precise measurements decrease MIN_CLAP_DUR
const float MIN_CLAP_DUR = 0.1;
const int MAX_CLAP_CNT = int(ANALOG_INP_SIZE * SAMPLING_PERDIOD / MIN_CLAP_DUR);
bool isClapArr[MAX_CLAP_CNT];
int currClapCnt = 0;

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
  if (inp.find(STR_CNT) != string::npos) {
    float tot = 0.0;
    for (int i = analogIdx; i >= 0; i--) {
      tot += analogInputs[i];
    }
    if (isNoiseBufferFulled) {
      for (int i = ANALOG_INP_SIZE - 1; i > analogIdx; i--) {
        tot += analogInputs[i];
      }
    }
    float avg = tot / ANALOG_INP_SIZE;
    int cntLessThanAvg = 0;
    for (int i = analogIdx; i >= 0; i--) {
      if (analogInputs[i] < avg) {
        cntLessThanAvg++;
      }
    }
    if (isNoiseBufferFulled) {
      for (int i = ANALOG_INP_SIZE - 1; i > analogIdx; i--) {
        if (analogInputs[i] < avg) {
          cntLessThanAvg++;
        }
      }
    }
    pc.printf("\ncnt less: %d\n", cntLessThanAvg);
  }
}

void tickTock() {
  float prevAnalogSum = currAnalogSum;
  float f = analogInp.read();
  currAnalogSum += f;
  float currAvg = currAnalogSum / (analogIdx + 1);
  if (isNoiseBufferFulled) {
    // value in array will be overriden 'cuz array is full
    currAnalogSum -= analogInputs[analogIdx];
    currAvg = currAnalogSum / ANALOG_INP_SIZE;
    float prevAvg = prevAnalogSum / ANALOG_INP_SIZE;
    if (analogInputs[analogIdx] < prevAvg) {
      currCntLessThanAvg--;
    }
  }
  if (f < currAvg) {
    currCntLessThanAvg++;
  }

  if (currCntLessThanAvg > maxCnt) {
    maxCnt = currCntLessThanAvg;
  } 
  if (currCntLessThanAvg == 0) {
    maxCnt = 0;
  }

  // pc.printf(" maxCnt: %d ", maxCnt);

  // if f is 0 it means clap
  if (f < 0.5) {
    if (!isClapArr[analogIdx / MAX_CLAP_CNT]) {
      currClapCnt++;
    }
    isClapArr[analogIdx / MAX_CLAP_CNT] = true;
  }

  analogInputs[analogIdx] = f;
  analogIdx++;

  if (analogIdx == ANALOG_INP_SIZE) {
    isNoiseBufferFulled = true;
    analogIdx = 0;
    memset(isClapArr, 0, sizeof(isClapArr));
    currClapCnt = 0;
  }
  
  led1 = led2 = led3 = led4 = float(currClapCnt) / MAX_CLAP_CNT;

  pc.printf(" %d ", currClapCnt);
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