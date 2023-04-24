#include <stdio.h>
//#include <wiringPi.h>
#include <windows.h>
#include <unistd.h>
#include <time.h>

//#define GPIO_PIN 1

/*int main(void) {
    if (wiringPiSetup() == -1) {
        printf("Failed to initialize wiringPi\n");
        return 1;
    }

    pinMode(GPIO_PIN, INPUT);

    int pinValue = digitalRead(GPIO_PIN);


    while (pinValue == HIGH) {
        printf("GPIO pin %d is currently active.\n", GPIO_PIN);
        // Minute long intervals between each update
        sleep(60);
    } else {
        time_t t;
        time(&t);
        // When the pin stops receiving power, this will show the exact date and time it happened.
        printf("GPIO pin %d has went offline at %s . Please check on status.\n", GPIO_PIN, ctime(&t));
    }

    // I assume from here we can then pipe these outputs to the client which will then push them to the server.

    return 0;
}*/

if (gpio_lev & (1 << 11)) {
    printf("Currently receiving power.\n");
    sleep(10);
} else {
    time_t t;
    time(&t);
    printf("Pin has lost power and gone offline at %s. Please check status.\n", ctime(&t));
}

