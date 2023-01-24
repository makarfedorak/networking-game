#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

const int PORT = 1234;
const int MAX_CONNECTIONS = 5;
const int BUFFER_SIZE = 1024;

int main() {
    int server_fd, client_fd, valread;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE];

    // create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // set server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // bind socket to address
    if (bind(server_fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error binding socket to address" << std::endl;
        return 1;
    }

    // listen for incoming connections
    listen(server_fd, MAX_CONNECTIONS);

    // accept incoming connection
    socklen_t client_length = sizeof(client_address);
    client_fd = accept(server_fd, (struct sockaddr *) &client_address, &client_length);
    if (client_fd < 0) {
        std::cerr << "Error accepting incoming connection" << std::endl;
        return 1;
    }

    // read data from client
    valread = read(client_fd, buffer, BUFFER_SIZE);
    std::cout << "Received message: " << buffer << std::endl;

    // send data to client
    std::string message = "Hello client";
    send(client_fd, message.c_str(), message.length(), 0);

    // close sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
