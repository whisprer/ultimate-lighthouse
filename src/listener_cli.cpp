#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char* argv[]) {
    std::string bind_ip = "0.0.0.0";
    int bind_port = 9876;

    for (int i = 1; i < argc; i++) {
        if ((std::string)argv[i] == "--bind" && i + 1 < argc) {
            bind_ip = argv[++i];
        } else if ((std::string)argv[i] == "--port" && i + 1 < argc) {
            bind_port = std::stoi(argv[++i]);
        }
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        return 1;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(bind_port);
    inet_pton(AF_INET, bind_ip.c_str(), &address.sin_addr);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return 1;
    }

    listen(server_fd, 5);
    std::cout << "Listening on " << bind_ip << ":" << bind_port << std::endl;

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd >= 0) {
            char buffer[1024] = {};
            int bytes = read(client_fd, buffer, sizeof(buffer) - 1);
            if (bytes > 0) {
                buffer[bytes] = '\0';
                std::cout << "Received: " << buffer << std::endl;
            }
            close(client_fd);
        }
    }

    close(server_fd);
    return 0;
}
