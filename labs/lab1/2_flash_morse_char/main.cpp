#include "mbed.h"
#include<string>
#include <ctype.h>

Serial pc(USBTX, USBRX); // tx, rx

char buffer[1024];
int charIdx = 0;
string alphabet = "abcdefghijklmnopqrstuvwqyz";
string morseAlphabet[] = {".-","-...","-.-.","-..", ".", "..-.", "--.",

                          "....", "..", ".---", "-.-", ".-..", "--",

                          "-.", "---", ".--.", "--.-", ".-.", "...", "-",

                          "..-", "...-", ".--", "-..-", "-.--", "--.."
                         };

void flashChar(char c)
{
    c = tolower(c);
    int index = alphabet.find(c);

    if(index != -1) {
        return morse[index];
    } else {
        return " ";
    }
}

void flashMorse(string morseChar) {
    morseChar.le    
}

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
