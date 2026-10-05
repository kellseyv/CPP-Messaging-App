#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

constexpr int PORT = 8080;

int main() {
    int listen_fd;
    int connection_fd;

    sockaddr_in server_address{};

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (listen_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(
            listen_fd,
            reinterpret_cast<const sockaddr*>(&server_address),
            sizeof(server_address)
        ) == -1) {
        perror("Binding failed");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(listen_fd, 1) == -1) {
        perror("Listen failed");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on 127.0.0.1:8080" << std::endl;

    sockaddr_in client_address{};
    socklen_t client_addrlen = sizeof(client_address);

    connection_fd = accept(
        listen_fd,
        reinterpret_cast<sockaddr*>(&client_address),
        &client_addrlen
    );

    if (connection_fd == -1) {
        perror("Accept failed");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Client connected!" << std::endl;

    close(connection_fd);
    close(listen_fd);

    return 0;
}
