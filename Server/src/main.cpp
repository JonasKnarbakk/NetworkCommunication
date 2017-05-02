// Author: Jonas Knarbakk (20:18:36-26/04/2017)
// Description: New project

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

// Prints the message and exits the program
void error(const char *msg);

// Handles the communication with a client
void handleClient(int tcp_socket);

int main( int argc, char ** argv ){

    // Exit if we don't have the expected amount of arguments
    if( argc != 2 ){
        error("You must supply an argument for which port to run the server..\n");
    }

    int port_number,
        tcp_socket,
        new_tcp_socket,
        pid,
        client_address_lenght;

    bool running = true;

    // Check if we can parse the argument into a number
    try{

        port_number = std::stoi(argv[1]);
        
    } catch (std::invalid_argument &e){

        error("The argument given was not a number.\n");
    
    } catch (std::out_of_range &e){
        
        error("The numbers gives was too big.\n");

    }

    // If the port number is in the reserved ports range, exit
    if(port_number < 1024){

        error("Port number given is not a valid port number.\nA valid port numbers is above the reserved system ports, ranging from 0-1023.\n");

    }

    struct sockaddr_in server_address, client_address;

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET; /* AF_INET is requires */
    server_address.sin_addr.s_addr = INADDR_ANY; /* Set the address to 0.0.0.0 */
    server_address.sin_port = htons(port_number); /* Set the port number */

    std::cout << "Server starting up on port " << port_number << std::endl;

    tcp_socket = socket(AF_INET, SOCK_STREAM, 0); /* Open the socket */

    if(tcp_socket < 0){

        error("ERROR: couldn't open socket!\n");
    
    }

    if(bind(tcp_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){

        error("ERROR: couldn't bind socket!\n");

    }

    // Listen on the socket for connection, set the backlog queue to five
    // which is the maximum number of connections on most systems that can
    // be waiting while a connection is being processed
    listen(tcp_socket, 5); 

    client_address_lenght = sizeof(client_address);

    // Sleep until a connection has been established with a client
    while(running){

        new_tcp_socket = accept(tcp_socket, (struct sockaddr *) &client_address, (socklen_t *) &client_address_lenght);

        if(new_tcp_socket < 0){
            
            error("ERROR: failed to accept client connection!\n");

        }

        pid = fork();

        if(pid < 0){
        
            error("ERROR: failed to fork connection!\n");
        
        }

        if(pid == 0){
        
            // Close the old socket
            close(tcp_socket);

            // Handle the interaction with the client
            handleClient(new_tcp_socket);

            exit(0);
        }

        else {
        
            close(new_tcp_socket);

        }
    }

    close(tcp_socket);
    
    return 0;

}

void error(const char *msg){
    perror(msg);
    exit(1);
}

void handleClient(int tcp_socket){

    int read_write_size = 0;

    char buffer[100];

    memset(buffer, 0, sizeof(buffer));

    read_write_size = read(tcp_socket, buffer, sizeof(buffer));

    if(read_write_size < 0){

        error("ERROR: couldn't read from socket!\n");

    }

    int content_size = std::stoi(buffer);

    char content_buffer[content_size];

    read_write_size = read(tcp_socket, content_buffer, sizeof(content_buffer));

    if(read_write_size < 0){

        error("ERROR: couldn't read from socket!\n");

    }

    std::ofstream file;

    file.open("RecievedFile.png");

    if(file.is_open()){
         
        file.write(content_buffer, content_size); 

    } else {
        
        std::cout << "Failed to open file" << std::endl;

    }

    memset(buffer, 0, sizeof(buffer));

    const char *response = "I got your message\n";

    memcpy(buffer, response, strlen(response));

    read_write_size = write(tcp_socket, buffer, 255);

    if(read_write_size < 0){
    
        error("ERROR: couldn't write to socket!\n");

    }

    close(tcp_socket);
}
