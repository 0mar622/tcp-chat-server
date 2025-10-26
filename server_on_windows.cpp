#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    char buffer[1024];
    int port = 6000;

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        return 1;
    }

    // Add this block:
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        return 1;
    }

    // 2. Bind socket to port
    address.sin_family = AF_INET;
    //inet_pton(AF_INET, "192.168.122.1", &address.sin_addr);
    address.sin_addr.s_addr = INADDR_ANY;
      //INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }

    // 3. Listen for connections
    if (listen(server_fd, 1) < 0) {
        perror("listen failed");
        return 1;
    }
    std::cout << "Echo server listening on port " << port << "...\n";

    // 4.
    fd_set master_set, read_set;
    FD_ZERO(&master_set);
    FD_SET(server_fd, &master_set);
    int max_fd = server_fd;

    while (true) {
        read_set = master_set;
        if (select(max_fd + 1, &read_set, nullptr, nullptr, nullptr) < 0) {
            perror("select failed");
            break;
        }

        for (int fd = 0; fd <= max_fd; ++fd) {
            if (!FD_ISSET(fd, &read_set)) continue;

            if (fd == server_fd) {
                // New client connection
                socklen_t addrlen = sizeof(address);
                client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
                if (client_fd < 0) {
                    perror("accept failed");
                    continue;
                }
                FD_SET(client_fd, &master_set);
                if (client_fd > max_fd) max_fd = client_fd;
                std::cout << "New client connected: fd " << client_fd << "\n";
            } else {
                // Handle message from client
                memset(buffer, 0, sizeof(buffer));
                int bytes_read = read(fd, buffer, sizeof(buffer));
                if (bytes_read <= 0) {
                    std::cout << "Client fd " << fd << " disconnected.\n";
                    close(fd);
                    FD_CLR(fd, &master_set);
                } else {
                    std::cout << "Client fd " << fd << " says: " << buffer;
                    send(fd, buffer, bytes_read, 0);
                }
            }
        }
    }


    close(client_fd);
    close(server_fd);
    return 0;
}
