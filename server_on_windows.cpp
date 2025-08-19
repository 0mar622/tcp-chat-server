#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    char buffer[1024];
    int port = 5000;

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
    address.sin_addr.s_addr = INADDR_ANY;
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

    // 4. Accept one client
    socklen_t addrlen = sizeof(address);
    client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (client_fd < 0) {
        perror("accept failed");
        return 1;
    }
    std::cout << "Client connected!\n";

    // 5. Read and echo back
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = read(client_fd, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            std::cout << "Client disconnected.\n";
            break;
        }
        std::cout << "Received: " << buffer;
        send(client_fd, buffer, bytes_read, 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
