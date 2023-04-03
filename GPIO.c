#include <stdio.h>
#include <wiringPi.h>

#define GPIO_PIN 1

int main(void) {
    if (wiringPiSetup() == -1) {
        printf("Failed to initialize wiringPi\n");
        return 1;
    }

    pinMode(GPIO_PIN, INPUT);

    int pinValue = digitalRead(GPIO_PIN);

    if (pinValue == HIGH) {
        printf("GPIO pin %d is receiving power\n", GPIO_PIN);
    } else {
        printf("GPIO pin %d is not receiving power\n", GPIO_PIN);
    }

    return 0;
}

