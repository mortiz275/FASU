#include <pigpio.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/*
 * ALARM PROJECT CLIENT CODE 
 * Change the following define statements to desired settings
 *
 * After saving the file, run the following command to apply changes:
 * "gcc -o client client.c"
 *
 * Start the client by running the following command:
 * "./client"
 */

//IP address for the RPi to connect to (Server IP)
#define IP_ADDRESS "172.25.60.147" 
//Port to connect to (Server Port)
#define PORT 8080 

//A descriptive ID to display on the server
#define CLIENT_ID "KITCHEN" 

//The GPIO pin numbers to use for each alarm
#define GPIO_PIN_FIRE 17 
#define GPIO_PIN_WATER 18
#define GPIO_PIN_BURGLARY 19

//How many seconds to wait before sending a new status update
#define SLEEP_INTERVAL 5 


//Connects to the server and sends message
int main(int argc, char const* argv[]){
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};

    //Set up GPIO
    if(gpioInitialise() < 0){
        fprintf(stderr, "Failed to initialise GPIO pins\n");
    }

    gpioSetMode(GPIO_PIN_FIRE, PI_INPUT);
    gpioSetMode(GPIO_PIN_WATER, PI_INPUT);
    gpioSetMode(GPIO_PIN_BURGLARY, PI_INPUT);

    //Continue to connect and send status messages
    while(1){
	    //Create socket
	    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("Socket creation error\n");
		return -1;
	    }

	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_port = htons(PORT);

	    if(inet_pton(AF_INET, IP_ADDRESS, &serv_addr.sin_addr) <= 0){
		printf("Invalid address\n");
		return -1;
	    }


        if((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0){
            printf("Connection failed\n");
            return -1;
        }

        //Form status message depending on pin values: "<status code><CLIENT_ID>...\n"
        //Current codes: 
        //K = OK
        //W = FLOOD 
        //F = FIRE
        //B = BURGLARY
        //Invalid code = warning
        char statusMessage[1024] = {0}; 

        if(gpioRead(GPIO_PIN_FIRE)){
            strcat(statusMessage, "F");
            strcat(statusMessage, CLIENT_ID); 
        }
        else if(gpioRead(GPIO_PIN_WATER)){
            strcat(statusMessage, "W");
            strcat(statusMessage, CLIENT_ID); 
        }
        else if(gpioRead(GPIO_PIN_BURGLARY)){
            strcat(statusMessage, "B");
            strcat(statusMessage, CLIENT_ID);
        }
        else{
            strcat(statusMessage, "K");
            strcat(statusMessage, CLIENT_ID); 
        }

        //char* statusMessage = CLIENT_ID;
        //Send the message
        send(client_fd, statusMessage, strlen(statusMessage), 0);
        printf("Status message sent\n");
        
        //Receive server reply
        valread = read(client_fd, buffer, 1024);
        printf("%s\n", buffer);

        close(client_fd);
        sleep(SLEEP_INTERVAL);
    }

	return 0;
}
