#include "mbed.h"

Serial pc(USBTX, USBRX); // tx, rx


char buffer[1024];

bool hasInp = false;
char currChar;
int charIdx = -1;

void inpHandler() 
{
    hasInp = true;
    currChar = pc.getc();
}

int main()
{
    pc.printf("Hello World!\n\r");
    pc.attach(&inpHandler, Serial::RxIrq);
    
    while(1) {
        if (hasInp) {
            if (currChar == '\n' || currChar == '\r') {
                pc.printf(buffer);
                pc.printf("\n");
                charIdx = -1;
                memset(buffer, 0, sizeof buffer);
            } else {
                charIdx++;
                buffer[charIdx] = currChar;
            }
            hasInp = false;
        }
    }
}