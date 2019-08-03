#include "mbed.h"

PwmOut fan(p21);
// AnalogOut  fan(p18);
Serial pc(USBTX, USBRX); // tx, rx
float fanValue = 0.1f;
InterruptIn tach(p5);
// AnalogIn tach(A0);
int rotationCnt = 0;
int prevAngle = 0;
int currAngle = 0;
char currChar = 'c';

void rotated()
{   
    rotationCnt++;
}

void tick()
{
    prevAngle = currAngle;
    currAngle = rotationCnt / 2;
}

void keyboardInp() {
    currChar = pc.getc();
}

Ticker ticker;

int main()
{
    pc.printf("\n program started \n");
    fan.period(0.00004);
    // 2 rise per rotation
    tach.rise(&rotated);
    // ticker.attach(&rotated, 0.01);
    ticker.attach(&tick, 1.0);
    pc.attach(&keyboardInp, Serial::RxIrq);
    while (1) {
        
        if (currChar <= '9' && currChar >= '0') {
            fanValue = float(currChar - '0') / 10.0f;
            fan = fanValue;
            pc.printf(" fan = %.1f ", fanValue);
            currChar = 'x';
        }
        if (currChar == 'r') {
            // rotation per second
            int rps = currAngle - prevAngle;
            pc.printf(" rps = %d ", rps);
            currChar = 'x';
        }
    }

}