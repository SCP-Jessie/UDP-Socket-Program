#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(void){
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    int server_struct_length = sizeof(server_addr);
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created\n");
    
    // Set port and IP - the server's info
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    while(1){
	    // Get input from the user:
	    printf("Enter message: ");
	    //gets continues through whitespaces while scanf does not
	    gets(client_message);
	    
	    // Send the message to server:
	    // include address of target
	    if(sendto(socket_desc, client_message, strlen(client_message), 0,
		 (struct sockaddr*)&server_addr, server_struct_length) < 0){
		printf("Send failed\n");
		return -1;
	    }
	    
	    // Receive the server's response:
	    if(recvfrom(socket_desc, server_message, sizeof(server_message), 0,
		 (struct sockaddr*)&server_addr, &server_struct_length) < 0){
		printf("Error: recvfrom failed\n");
		return -1;
	    }
	    
	    printf("Server reply : %s\n", server_message);
	    memset(server_message, 0, sizeof(server_message));
	    memset(client_message, 0, sizeof(server_message));
    
    }
    
    // Close socket
    close(socket_desc);
    
    return 0;
}

