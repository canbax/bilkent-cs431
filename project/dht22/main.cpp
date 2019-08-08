#include "mbed.h"
#include "dht22.h"

DHT sensor(p23);

void task_DHT();

int main()
{   
    printf("\nprogram started\n");
    while(1) {
         task_DHT();
    }
}

void task_DHT(){
    int error = 0;
    int h, c;
    float dp = 0.0f;
 
        wait(2.0f);
        error = sensor.readData();
        if (0 == error) {
            c   = sensor.ReadTemperature(CELCIUS);
            h   = sensor.ReadHumidity();
            dp  = sensor.CalcdewPoint(c, h);
    
            printf("Temperature in Celcius: %d \r\n", c);
            printf("Humidity is %d, Dewpoint: %4.2f\r\n\n", h, dp);
        } else {
            printf("Error: %d\r\n", error);
        }
}
