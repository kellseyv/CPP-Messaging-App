#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;

int main() {
  int connection_fd;
  sockaddr_in address{};

  connection_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (connection_fd == -1) {
    perror("Client socket creation failed");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_port = htons(PORT);
  address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  if (connect(connection_fd, reinterpret_cast<const sockaddr *>(&address),
              sizeof(address)) == -1) {
    perror("Connection to server failed");
    close(connection_fd);
    exit(EXIT_FAILURE);
  }

  std::cout << "Connected to server!" << std::endl;

  std::string initial_message;

  std::cout << "Enter a message: ";
  std::getline(std::cin, initial_message);
  initial_message += '\n';

  ssize_t bytes_sent =
      send(connection_fd, initial_message.data(), initial_message.size(), 0);

  if (bytes_sent == -1) {
    perror("Error sending message from client");
    close(connection_fd);
    exit(EXIT_FAILURE);
  }

  char buffer[BUFFER_SIZE];

  ssize_t bytes_recieved = recv(connection_fd, buffer, BUFFER_SIZE, 0);

  if (bytes_recieved == -1) {
    perror("Error recieving message from server");
    close(connection_fd);
    exit(EXIT_FAILURE);
  } else if (bytes_recieved == 0) {
    perror("Connection to server disconnected");
    close(connection_fd);
    exit(EXIT_FAILURE);
  } else {
    std::cout << "Server message: ";
    std::cout.write(buffer, bytes_recieved);
    std::cout << std::endl;
  }

  close(connection_fd);
  return 0;
}
