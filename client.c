#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define IP_ADDRESS "127.0.0.1" //IP address for the RPi
#define PORT 8080 //Port to connect to
#define CLIENT_ID "WKITCHEN" //Message to be sent to server
			     //Current codes: 
			     //K = OK
			     //W = FLOOD 
			     //F = FIRE
			     //Invalid code = warning

//Connects to the server and sends message
//TODO: add in GPIO pin features
int main(int argc, char const* argv[]){
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};

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

	char* statusMessage = CLIENT_ID;
	send(client_fd, statusMessage, strlen(statusMessage), 0);
	printf("Status message sent\n");
	
	valread = read(client_fd, buffer, 1024);
	printf("%s\n", buffer);

	close(client_fd);
	return 0;
}
