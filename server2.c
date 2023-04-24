#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define PORT 8080 //Port Number
#define LOST_INTERVAL 10 //Time (seconds) to wait before checking if RPis are lost
#define LOST_THRESHOLD 5 //Time (seconds) allowed to pass before RPis are considered lost

time_t lastPing;
time_t lastLostCheck;

//Structure to hold client ID and last ping
typedef struct{
	char client_id[20];
	time_t last_ping;
}clientData;

//Dynamic array to keep track of clients who have connected
int clients = 0;
clientData *client_data_arr;

//Finds the correct client and updates the ping
void update_ping(char *client_id){
	if(clients != 0){
		for(int i = 0; i < clients; i++){
			if(strcmp(client_data_arr[i].client_id, client_id) == 0){
				client_data_arr[i].last_ping = time(NULL);
				return;
			}
		}
	}

	//If we make it here the client isn't in the array, so we add it.
	clientData new_client = {"", 0};
	strcpy(new_client.client_id, client_id);
	new_client.last_ping = time(NULL);

	clients++;
	client_data_arr = realloc(client_data_arr, clients * sizeof(clientData));
	client_data_arr[clients - 1] = new_client;
}

//Frees the array when the program ends
void free_list(){
	free(client_data_arr);	
}

//Check if the RPi is considered lost
void check_lost(){
	for(int i = 0; i < clients; i++){
		if(difftime(time(NULL), client_data_arr[i].last_ping) > LOST_THRESHOLD){
			time_t current_time;
			struct tm * time_info;
			char time_string[30];

			time(&current_time);
			time_info = localtime(&current_time);
			strftime(time_string, sizeof(time_string), "[%a %b %d %Y %H:%M:%S]", time_info);

			printf("\033[0;33m%s Warning: %s has not been communicating!\n", time_string, client_data_arr[i].client_id);
		}
	}
}

//Function that runs when a connection is received. Prints the status code and is color coordinated
//TODO: Color coordination is kind of jank. Will be fixed in the future
void *handle_client(void *arg){
	int new_socket = *(int *)arg;
	char buffer[1024] = {0};
	char *received = "Received your message";
	int valread = read(new_socket, buffer, 1024);

	time_t current_time;
	struct tm * time_info;
	char time_string[30];

	time(&current_time);
	time_info = localtime(&current_time);
	strftime(time_string, sizeof(time_string), "[%a %b %d %Y %H:%M:%S]", time_info);

	update_ping(&buffer[1]);

	if(buffer[0] == 'K'){
		printf("\033[0;32m%s Received message from: %s status OK\n", time_string, &buffer[1]);
	}else if(buffer[0] == 'F'){
		printf("\033[1;31m%s Received message from: %s status FIRE\n", time_string, &buffer[1]);
	}else if(buffer[0] == 'W'){
		printf("\033[1;34m%s Received message from: %s status FLOOD\n", time_string, &buffer[1]);
	}else{
		printf("\033[0;33m%s Warning: Message from %s does not contain a valid status!\n", time_string, &buffer[1]);
	}

	send(new_socket, received, strlen(received), 0);
	close(new_socket);
	return NULL;
}

//Listens for connections and runs handle_client
void *the_listener(void *arg){
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	
        if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if(listen(server_fd, 3) < 0){
		perror("listen");
		exit(EXIT_FAILURE);
	}
        while(1){
                if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
		        perror("accept");
		        exit(EXIT_FAILURE);
		}
		        
	        pthread_t tid;
	        int *client_sock = malloc(sizeof(int));
	        *client_sock = new_socket;
	        if(pthread_create(&tid, NULL, handle_client, client_sock) != 0){
		        perror("pthread_create");
		        exit(EXIT_FAILURE);
	        }
	}
	
	shutdown(server_fd, SHUT_RDWR);
}

//Main function to make a thread for the listener and to determine when to check for lost RPis
int main(int argc, char const *argv[]){
	client_data_arr = malloc(clients * sizeof(clientData));
	lastPing = time(NULL);
	lastLostCheck = time(NULL);

	atexit(free_list);

	

	pthread_t listener;
	if(pthread_create(&listener, NULL, the_listener, NULL) != 0){
		perror("pthread_create_listener");
		exit(EXIT_FAILURE);
	}

	while(1){
		sleep(LOST_INTERVAL);
		check_lost();
	}
	return 0;
}
