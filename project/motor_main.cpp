/*
############################################
##           sMotor v0.1 Test Program     ##
##          created by Samuel Matildes    ##
############################################
        ---- sam.naeec@gmail.com -----
This library was made for 4-Phase Stepper Motors
I don't take any resposability for the damage caused to your equipment.

*/

#include "mbed.h"
#include "sMotor.h"


Serial pc(USBTX, USBRX);
sMotor motor(p9, p10, p11, p12); // creates new stepper motor: IN1, IN2, IN3, IN4

int step_speed = 1200 ; // set default motor speed
int numstep = 512 ; // defines full turn of 360 degree
//you might want to calibrate this value according to your motor


int main() {

    //Credits
    printf("4 Phase Stepper Motor v0.1 - Test Program\r\n");
    printf("developed by Samuel Matildes\r\n");
    printf("\n\r");

    // Screen Menu
    printf("Default Speed: %d\n\r",step_speed);
    printf("1- 360 degree clockwise step\n\r");
    printf("2- 360 degree anticlockwise step\n\r");
    printf("3- 180 degree clockwise step\n\r");
    printf("4- 180 degree anticlockwise step\n\r");
    printf("5- Change Speed\n\r");

    while (1) {

        if (pc.readable()) { // checks for serial

            if (pc.getc()=='1')
                motor.step(numstep,0,step_speed); // number of steps, direction, speed

            if (pc.getc()=='2')
                motor.step(numstep,1,step_speed);

            if (pc.getc()=='3')
                motor.step(numstep/2,0,step_speed);

            if (pc.getc()=='4')
                motor.step(numstep/2,1,step_speed);

            if (pc.getc()=='5') {
                printf("Current Speed: %d\n\r", step_speed);
                printf("New speed: \n\r");
                pc.scanf("%d",&step_speed); // sets new speed
            }
        }
    }
}