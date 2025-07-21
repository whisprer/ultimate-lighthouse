// Simple test version to verify your setup
// This uses standard libraries only to test the network code first

#include <iostream>
#include <string>
#include <cstring>
#include <chrono>
#include <thread>
#include <atomic>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    #define close closesocket
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif

class simple_beacon {
private:
    #ifdef _WIN32
        SOCKET socket_fd_;
        static bool wsa_initialized_;
    #else
        int socket_fd_;
    #endif
    sockaddr_in destination_;
    std::atomic<bool> running_{false};
    std::thread beacon_thread_;
    
public:
    simple_beacon(const std::string& target_ip, uint16_t port) {
        #ifdef _WIN32
            if (!wsa_initialized_) {
                WSADATA wsaData;
                if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                    throw std::runtime_error("WSAStartup failed");
                }
                wsa_initialized_ = true;
            }
            socket_fd_ = INVALID_SOCKET;
        #else
            socket_fd_ = -1;
        #endif
        
        initialize_socket(target_ip, port);
    }
    
    ~simple_beacon() {
        stop();
        #ifdef _WIN32
            if (socket_fd_ != INVALID_SOCKET) {
                closesocket(socket_fd_);
            }
        #else
            if (socket_fd_ >= 0) {
                close(socket_fd_);
            }
        #endif
    }
    
    void initialize_socket(const std::string& ip, uint16_t port) {
        socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
        #ifdef _WIN32
            if (socket_fd_ == INVALID_SOCKET) {
                throw std::runtime_error("Socket creation failed");
            }
        #else
            if (socket_fd_ < 0) {
                throw std::runtime_error("Socket creation failed");
            }
        #endif
        
        destination_.sin_family = AF_INET;
        destination_.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &destination_.sin_addr);
    }
    
    void start() {
        if (running_.exchange(true)) return;
        
        beacon_thread_ = std::thread([this]() {
            uint32_t counter = 0;
            while (running_) {
                std::string message = "{\"seq\":" + std::to_string(counter++) + 
                                    ",\"time\":" + std::to_string(
                                        std::chrono::system_clock::now().time_since_epoch().count()
                                    ) + "}";
                
                sendto(socket_fd_, message.c_str(), message.length(), 0,
                      (struct sockaddr*)&destination_, sizeof(destination_));
                
                std::cout << "Sent beacon #" << counter-1 << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });
        
        std::cout << "Simple beacon started!" << std::endl;
    }
    
    void stop() {
        if (!running_.exchange(false)) return;
        if (beacon_thread_.joinable()) {
            beacon_thread_.join();
        }
    }
};

#ifdef _WIN32
bool simple_beacon::wsa_initialized_ = false;
#endif

int main() {
    try {
        simple_beacon beacon("127.0.0.1", 9001);
        beacon.start();
        
        std::cout << "Press Enter to stop..." << std::endl;
        std::cin.get();
        
        beacon.stop();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}