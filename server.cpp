#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

int main() {
    int listen_fd, connection_fd;
    sockaddr_in address = {0};

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (listen_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(listen_fd,reinterpret_cast<const sockaddr*>(&address),sizeof(address)) == -1) {
        perror("Binding failed");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    if(listen(listen_fd, 1) == -1){
      perror("Listen function failed");
      close(listen_fd);
      exit(EXIT_FAILURE);
    }
    
    std::cout << "Server Listening on port 127.0.0.1:8080" << std::endl;

    sockaddr_in client_address = {0};
    socklen_t client_addrlen = sizeof(client_address);

    connection_fd = accept(listen_fd, reinterpret_cast<sockaddr*>(&client_address), &client_addrlen);

    if(connection_fd == -1){
      perror("Connection failed");
      close(listen_fd);
      exit(EXIT_FAILURE);
    } else {
      std::cout << "Client connected!" << std::endl;
    }

    close(connection_fd);
    close(listen_fd);
    return 0;
}
