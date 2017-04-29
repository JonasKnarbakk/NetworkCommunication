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

    if(argc == 2){

        int port_number,
            tcp_socket,
            new_tcp_socket,
            client_address_lenght,
            read_write_size;

        try{

            port_number = std::stoi(argv[1]);
            
        } catch (std::invalid_argument &e){

            error("The argument given was not a number.\n");
        
        } catch (std::out_of_range &e){
            
            error("The numbers gives was too big.\n");

        }

        if(port_number < 1024){

            error("Port number given is not a valid port number.\nA valid port numbers is above the reserved system ports, ranging from 0-1023.\n");

        }

        char buffer[256];

        struct sockaddr_in server_address, client_address;

        memset(&server_address, 0, sizeof(server_address));

        server_address.sin_family = AF_INET; /* AF_INET is requires */
        server_address.sin_addr.s_addr = INADDR_ANY; /* Set the address to 0.0.0.0 */
        server_address.sin_port = htons(port_number); /* Set the port number */

        std::cout << "Server starting up on port " << port_number << std::endl;

        tcp_socket = socket(AF_INET, SOCK_STREAM, 0); /* Open the socket */

        if(tcp_socket < 0){

            error("ERROR: could not open socket!\n");
        
        }

        if(bind(tcp_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){

            error("ERROR: could not bind socket!\n");

        }

        // Listen on the socket for connection, set the backlog queue to five
        // which is the maximum number of connections on most systems that can
        // be waiting while a connection is is being processed .
        listen(tcp_socket, 5); 

        client_address_lenght = sizeof(client_address);

        // Sleep until a connection has been stablished with a client
        new_tcp_socket = accept(tcp_socket, (struct sockaddr *) &client_address, (socklen_t *) &client_address_lenght);            

        if(new_tcp_socket < 0){
            
            error("ERROR: failed to accept client connection!\n");

        }

        memset(buffer, 0, sizeof(buffer));

        read_write_size = read(new_tcp_socket, buffer, sizeof(buffer)-1);

        if(read_write_size < 0){

            error("ERROR: could not read from socket!\n");

        }

        std::cout << "Here is the message " << buffer << std::endl;
        
        return 0;

    } 
    
    else {

        std::cout << "You must supply an argument for which port to run the server.." << std::endl;
        return 0;

    }

}
