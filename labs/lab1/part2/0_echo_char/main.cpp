#include "mbed.h"

Serial pc(USBTX, USBRX); // tx, rx
bool hasInp = false;
char currChar;

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
            pc.putc(currChar);
            hasInp = false;
        }
    }
}
