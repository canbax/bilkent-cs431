#include "mbed.h"
#include<string>
#include <ctype.h>

Serial pc(USBTX, USBRX); // tx, rx
DigitalOut myled(LED1);

char buffer[1024];
int charIdx = 0;
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
    int len = morseChar.size();
    for (int i = 0; i < len; i++) {
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

int main()
{
    pc.printf("Hello World!\n\r");
    while(1) {
        char c = pc.getc();
        flashCharInMorse(c);
    }
}
