#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <cstdlib>
#include <iostream>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

class Connection{
    public:
        Connection(std::string server_address, unsigned int port_number);

        void sendToServer(const char *message);
        int sendFile(std::string filepath);
    private:
        int tcp_socket,
            read_write_size;

        char buffer[256];

        struct sockaddr_in server_address;
        struct hostent *server;

        int sendHeaderInfo(const char *file_size, const char *filename);
};

#endif
