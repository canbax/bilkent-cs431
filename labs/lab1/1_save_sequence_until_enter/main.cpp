#include "mbed.h"

Serial pc(USBTX, USBRX); // tx, rx

char buffer[1024];
int charIdx = 0;

int main()
{
    pc.printf("Hello World!\n\r");
    while(1) {
        char c = pc.getc();
        
        if (c == '\n' || c == '\r') {
            pc.printf(buffer);
            pc.printf("\n");
            charIdx = 0;
            memset(buffer, 0, sizeof buffer);
        } else {
            buffer[charIdx] = c;
            charIdx++;
        }
    }
}
