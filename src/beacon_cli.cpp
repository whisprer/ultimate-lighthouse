#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char* argv[]) {
    std::string target_ip = "127.0.0.1";
    int target_port = 8080;

    for (int i = 1; i < argc; i++) {
        if ((std::string)argv[i] == "--host" && i + 1 < argc) {
            target_ip = argv[++i];
        } else if ((std::string)argv[i] == "--port" && i + 1 < argc) {
            target_port = std::stoi(argv[++i]);
        }
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    sockaddr_in dest{};
    dest.sin_family = AF_INET;
    dest.sin_port = htons(target_port);
    if (inet_pton(AF_INET, target_ip.c_str(), &dest.sin_addr) <= 0) {
        std::cerr << "Invalid IP address: " << target_ip << std::endl;
        return 1;
    }

    const char* beacon_msg = "Lighthouse beacon activated.";

    while (true) {
        sendto(sock, beacon_msg, strlen(beacon_msg), 0, (sockaddr*)&dest, sizeof(dest));
        std::cout << "Beacon sent to " << target_ip << ":" << target_port << std::endl;
        sleep(3);
    }

    close(sock);
    return 0;
}
