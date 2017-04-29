// Author: Jonas Knarbakk (20:18:36-26/04/2017)
// Description: New project

#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

void error(const char *msg){
  perror(msg);
  exit(1);
}

int main( int argc, char ** argv ){

    if(argc != 3){
        error("Invalid number of arguments: aruments needed are ip address and port number.\n");
    }

    int port_number,
        tcp_socket,
        read_write_size;

    char buffer[256];

    struct sockaddr_in server_address;
    struct hostent *server;

    try{

        port_number = std::stoi(argv[2]);
        
    } catch (std::invalid_argument &e){

        error("The argument given was not a number.\n");
    
    } catch (std::out_of_range &e){
        
        error("The numbers gives was too big.\n");

    }

    if(port_number < 1024){

        error("Port number given is not a valid port number.\nA valid port numbers is above the reserved system ports, ranging from 0-1023.\n");

    }

    tcp_socket = socket(AF_INET, SOCK_STREAM, 0); /* Open the socket */

    if(tcp_socket < 0){

        error("ERROR: could not open socket!\n");
    
    }

    if(bind(tcp_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){

        error("ERROR: could not bind socket!\n");

    }

    server = gethostbyname(argv[1]);

    if( server == NULL ){
    
        error("ERROR: no such host!\n");
    
    }

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET; /* AF_INET is requires */
    memcpy(server->h_addr, &server_address.sin_addr.s_addr, server->h_length); /* Copy over the server adress */
    server_address.sin_port = htons(port_number); /* Set the port number */

    if(connect(tcp_socket, (const sockaddr*) &server_address, sizeof(server_address)) < 0){
    
        error("ERROR: couldn't connect to the server\n");
    
    }

    // Zero out the buffer
    memset(buffer, 0, sizeof(buffer));

    std::cout << "Text to send (max 255 characters): ";

    // Get input from the user
    std::cin.getline(buffer, 255);

    // Send data to the server
    read_write_size = write(tcp_socket, buffer, 255);
    
    if(read_write_size < 0){
        error("ERROR: couldn't write to socket\n");
    }

    // Zero out the buffer
    memset(buffer, 0, sizeof(buffer));

    // Read the response from the server
    read_write_size = read(tcp_socket, buffer, 255);

    if(read_write_size < 0){
        error("ERROR: couldn't read from socket\n");
    }

    std::cout << buffer << std::endl;

    close(tcp_socket);
    
    return 0;

} 
