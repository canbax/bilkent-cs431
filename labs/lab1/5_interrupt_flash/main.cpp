#include "mbed.h"
#include <string>
#include <ctype.h>

Serial pc(USBTX, USBRX); // tx, rx
DigitalOut myled(LED1);

char buffer[1024];
char buffer2[1024];
int charIdx = 0;
int bufferSize = 0;
int flashIndex = 0;
bool isInterrupted = false;
string alphabet = "abcdefghijklmnopqrstuvwqyz1234567890";
string morseAlphabet[] = {".-","-...","-.-.","-..", ".", "..-.", "--.",

                          "....", "..", ".---", "-.-", ".-..", "--",

                          "-.", "---", ".--.", "--.-", ".-.", "...", "-",

                          "..-", "...-", ".--", "-..-", "-.--", "--..",
                          ".----", "..---", "...--", "....-", ".....",
                          "-....", "--...", "---..", "----.", "-----"
                         };


void flashLed(float time)
{
    if(pc.readable()) {
        char c = pc.getc();

        if (c == '\n' || c == '\r') {
            pc.printf(buffer);
            pc.printf("\n");
            bufferSize = charIdx + 1;
            strcpy(buffer,buffer2);
            pc.printf("\nbuffer: %s",buffer);
            pc.printf("\nbuffer2: %s",buffer2);
            flashIndex = 0;
            isInterrupted = true;
            pc.printf("interrupt occured.");
            return;
            //flashCharBuffer();
            //charIdx = 0;
            //memset(buffer2, 0, sizeof buffer2);
            //memset(buffer, 0, sizeof buffer);
        } else {
            //isInterrupted = false;
            buffer2[charIdx] = c;
            charIdx++;
        }
    }
    myled = 1;
    wait(time);
    myled = 0;
    wait(time);
}

string char2morse(char c)
{
    c = tolower(c);
    int index = alphabet.find(c);

    if(index != -1) {
        return morseAlphabet[index];
    } else {
        return " ";
    }
}

void flashMorse(string morseChar)
{
    if (morseChar == " ") {
        return;
    }
    isInterrupted = false;
    int len = morseChar.size();
    for (int i = 0; i < len; i++) {
        if(isInterrupted)
            return;
        char ch = morseChar[i];
        if (ch == '-') {
            flashLed(1.5);
        } else if (ch == '.') {
            flashLed(0.5);
        } else {
            pc.printf("line 46 not dot or dash in morse string");
        }
    }
}

void flashCharInMorse(char c)
{
    string morseChar = char2morse(c);
    flashMorse(morseChar);
}

void flashCharBuffer()
{
    for (flashIndex = 0; flashIndex < bufferSize; flashIndex++) {
        flashCharInMorse(buffer[flashIndex]);
    }
    if (bufferSize > 1) {
        pc.printf("end of flashCharBuffer with size %d", bufferSize - 1);
    }
}

int main()
{
    pc.printf("Hello World!\n\r");
    while(1) {

        char c = pc.getc();
        if (c == '\n' || c == '\r') {
            pc.printf(buffer);
            pc.printf("\n");
            bufferSize = charIdx + 1;
            charIdx = 0;
            flashCharBuffer();
            
            memset(buffer, 0, sizeof buffer);
        } else {
            buffer[charIdx] = c;
            charIdx++;
        }
    }
}
