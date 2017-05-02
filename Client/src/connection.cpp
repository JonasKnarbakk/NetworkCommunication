#include "connection.hpp"
#include <fstream>

void error(const char *msg){
    perror(msg);
    exit(1);
}

Connection::Connection(std::string serv_addr, unsigned int port){

    if(port < 1024){

        error("Port number given is not a valid port number.\nA valid port numbers is above the reserved system ports, ranging from 0-1023.\n");

    }

    tcp_socket = socket(AF_INET, SOCK_STREAM, 0); /* Open the socket */

    if(tcp_socket < 0){

        error("ERROR: could not open socket!\n");
    
    }

    server = gethostbyname(serv_addr.c_str());

    if( server == NULL ){
    
        error("ERROR: no such host!\n");
    
    }

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET; /* AF_INET is requires */
    memcpy(server->h_addr, &server_address.sin_addr.s_addr, server->h_length); /* Copy over the server adress */
    server_address.sin_port = htons(port); /* Set the port number */
}

void Connection::sendToServer(const char *message){

    if(connect(tcp_socket, (const sockaddr*) &server_address, sizeof(server_address)) < 0){
    
        error("ERROR: couldn't connect to the server\n");
    
    }

    // Zero out the buffer
    memset(buffer, 0, sizeof(buffer));

    // Copy the message into the buffer
    memcpy(buffer, message, strlen(message));

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
}

int Connection::sendFile(std::string filepath){

    // Open the file
    std::ifstream file;

    file.open(filepath, std::ios::binary);

    if(!file.is_open()){
        // Failed to open file
        return -1;
    }

    // Get the length of the file
    file.seekg(0, file.end);
    ssize_t buffer_size = file.tellg();
    file.seekg(0, file.beg);

    char buffer[buffer_size];

    // Read the file
    file.read(buffer, buffer_size);

    if(file){
        std::cout << "File read successfully! bytes read: " << buffer_size << std::endl;
    } else {
        std::cout << "Could only read " << file.gcount() << " bytes could be read!" << std::endl;
    }
       
    file.close();

    // Send the file
    if(connect(tcp_socket, (const sockaddr*) &server_address, sizeof(server_address)) < 0){
    
        error("ERROR: couldn't connect to the server\n");
    
    }

    // Send size info to the server
    char size_info[100];

    memset(size_info, 0, sizeof(size_info));

    memcpy(size_info, std::to_string(buffer_size).c_str(), strlen(std::to_string(buffer_size).c_str()));

    read_write_size = write(tcp_socket, size_info, sizeof(size_info));
   
    if(read_write_size < 0){
        error("ERROR: couldn't write to socket\n");
    }

    read_write_size = write(tcp_socket, buffer, sizeof(buffer));

    // Zero out the buffer
    memset(buffer, 0, sizeof(buffer));

    // Read the response from the server
    read_write_size = read(tcp_socket, buffer, sizeof(buffer));

    if(read_write_size < 0){
        error("ERROR: couldn't read from socket\n");
    }

    std::cout << buffer << std::endl;

    close(tcp_socket);
    return 0;
}
