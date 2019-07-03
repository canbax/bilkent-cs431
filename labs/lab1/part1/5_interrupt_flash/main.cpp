#include "mbed.h"
#include <string>
#include <ctype.h>

// constant values
const int BUFFER_SIZE = 1024;
const float SHORT_FLASH_DUR = 0.5;
const float LONG_FLASH_DUR = 1.5;

Serial pc(USBTX, USBRX); // tx, rx
DigitalOut myled(LED1);

char buffer[BUFFER_SIZE];
char buffer2[BUFFER_SIZE];
int bufferIdx = -1;
int bufferSize = 0;
int flashIdx = 0;
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
            bufferSize = bufferIdx + 1;
            // we are done with buffering set bufferIdx to -1
            bufferIdx = -1;
            strcpy(buffer, buffer2);
            // we are done with buffer2, make it ready to use
            memset(buffer2, 0, sizeof buffer2);
            // flashIdx will be incremented in the loop of flashCharBuffer
            flashIdx = -1;
            isInterrupted = true;
            pc.printf("\ninterrupt occured with %s", buffer);
            return;
        } else {
            // since it was pressed enter, bufferIdx = 0
            bufferIdx++;
            buffer2[bufferIdx] = c;
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
        if(isInterrupted) {
            return;
        }
        char ch = morseChar[i];
        if (ch == '-') {
            flashLed(LONG_FLASH_DUR);
        } else if (ch == '.') {
            flashLed(SHORT_FLASH_DUR);
        } else {
            pc.printf("line 86 not 'dot' or 'dash' in morse string");
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
    for (flashIdx = 0; flashIdx < bufferSize; flashIdx++) {
        pc.printf(" flashIdx: %d", flashIdx);
        flashCharInMorse(buffer[flashIdx]);
    }
    if (bufferSize > 0) {
        pc.printf("\nend of flashCharBuffer with size %d", bufferSize);
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
            bufferSize = bufferIdx + 1;
            bufferIdx = -1;
            flashCharBuffer();
            memset(buffer, 0, sizeof buffer);
        } else {
            bufferIdx++;
            buffer[bufferIdx] = c;
        }
    }
}
