#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

constexpr int PORT = 8080;

int main(){
  int connection_fd;
  sockaddr_in address{};

  connection_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (connection_fd == -1){
    perror("Client socket creation failed");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_port = htons(PORT);
  address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  if (connect(connection_fd, 
        reinterpret_cast<const sockaddr*>(&address), 
        sizeof(address)) == -1){
    perror("Connection to server failed");
    close(connection_fd);
    exit(EXIT_FAILURE);
  }

  std::cout << "Connected to server!" << std::endl;

  close(connection_fd);
  return 0;
}
