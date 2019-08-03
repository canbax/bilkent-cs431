/*
############################################
##           sMotor v0.1 Library          ##
##          created by Samuel Matildes    ##
############################################
        ---- sam.naeec@gmail.com -----
This library was made for 4-Phase Stepper Motors
I don't take any resposability for the damage caused to your equipment.

*/

#include "sMotor.h"

#include "mbed.h"

int motorSpeed; // Steper speed

sMotor::sMotor(PinName A0, PinName A1, PinName A2, PinName A3) : _A0(A0), _A1(A1), _A2(A2), _A3(A3) { // Defenition of motor pins
    _A0=0;
    _A1=0;
    _A2=0;
    _A3=0;
}


void sMotor::anticlockwise() { // rotate the motor 1 step anticlockwise 
    for (int i = 0; i < 8; i++) {

        switch (i) { // activate the ports A0, A2, A3, A3 in a binary sequence for steps
            case 0: {
                _A0=0;
                _A1=0;
                _A2=0;
                _A3=1;
            }
            break;
            case 1: {
                _A0=0;
                _A1=0;
                _A2=1;
                _A3=1;
            }
            break;
            case 2: {
                _A0=0;
                _A1=0;
                _A2=1;
                _A3=0;
            }
            break;
            case 3: {
                _A0=0;
                _A1=1;
                _A2=1;
                _A3=0;
            }
            break;
            case 4: {
                _A0=0;
                _A1=1;
                _A2=0;
                _A3=0;
            }
            break;
            case 5: {
                _A0=1;
                _A1=1;
                _A2=0;
                _A3=0;
            }
            break;
            case 6: {
                _A0=1;
                _A1=0;
                _A2=0;
                _A3=0;
            }
            break;
            case 7: {
                _A0=1;
                _A1=0;
                _A2=0;
                _A3=1;
            }
            break;
        }


        wait_us(motorSpeed); // wait time defines the speed 
    }
}

void sMotor::clockwise() { // rotate the motor 1 step clockwise 
    for (int i = 7; i >= 0; i--) {

        switch (i) {
            case 0: {
                _A0=0;
                _A1=0;
                _A2=0;
                _A3=1;
            }
            break;
            case 1: {
                _A0=0;
                _A1=0;
                _A2=1;
                _A3=1;
            }
            break;
            case 2: {
                _A0=0;
                _A1=0;
                _A2=1;
                _A3=0;
            }
            break;
            case 3: {
                _A0=0;
                _A1=1;
                _A2=1;
                _A3=0;
            }
            break;
            case 4: {
                _A0=0;
                _A1=1;
                _A2=0;
                _A3=0;
            }
            break;
            case 5: {
                _A0=1;
                _A1=1;
                _A2=0;
                _A3=0;
            }
            break;
            case 6: {
                _A0=1;
                _A1=0;
                _A2=0;
                _A3=0;
            }
            break;
            case 7: {
                _A0=1;
                _A1=0;
                _A2=0;
                _A3=1;
            }
            break;
        }


        wait_us(motorSpeed); // wait time defines the speed 
    }
}
void sMotor::step(int num_steps, int direction, int speed) {// steper function: number of steps, direction (0- right, 1- left), speed (default 1200)
    int count=0; // initalize step count
    motorSpeed=speed; //set motor speed
    if (direction==0) // turn clockwise
        do {
            clockwise();
            count++;
        } while (count<num_steps); // turn number of steps applied 
    else if (direction==1)// turn anticlockwise
        do {
            anticlockwise();
            count++;
        } while (count<num_steps);// turn number of steps applied 

}