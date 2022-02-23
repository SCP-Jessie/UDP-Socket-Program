#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(void){
    int socket_desc;
    // hold address info for both the client and server
    struct sockaddr_in server_addr, client_addr;
    char server_message[2000], client_message[2000];
    int client_struct_length = sizeof(client_addr);
    
    // Create UDP socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0){
        printf("Could not create socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Bind:
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Error: Bind failed");
        return -1;
    }
    //printf("Done with binding\n");
    puts("Bind completed");
    
    while(1)
    {
	    printf("Listening for incoming messages...\n\n");
	    
	    // Receive client's message - return length of client message
	    // Directly receive client information here - put into client_message
	    // Wait for client's sendto() call
	    if (recvfrom(socket_desc, client_message, sizeof(client_message), 0,
		 (struct sockaddr*)&client_addr, &client_struct_length) < 0){
		printf("Couldn't receive\n");
		return -1;
	    }
	    printf("Received message from IP: %s and port: %i\n",
		   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	    
	    printf("Msg from client: %s\n", client_message);
	    
	    // Have client echo server response:
	    //Copy client_message into server_message
	    strcpy(server_message, client_message);
	    
	    char extra_message[] = " no";
	    strcat(server_message, extra_message);
	    //send what ever is now in server_message back to client
	    // send to client's recvfrom() call with client address
	    if (sendto(socket_desc, server_message, strlen(server_message), 0,
		 (struct sockaddr*)&client_addr, client_struct_length) < 0){
		printf("Can't send\n");
		return -1;
	    }
	    
	    // clear contents
	    memset(server_message, 0, sizeof(server_message));
	    memset(client_message, 0, sizeof(server_message));
    
    }
    // Close socket
    close(socket_desc);
    
    return 0;
}
