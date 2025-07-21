// Minimal lighthouse with basic JSON parsing
// We'll use nlohmann/json which is header-only and easier to set up

#include <iostream>
#include <string>
#include <cstring>
#include <chrono>
#include <atomic>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    #define close closesocket
    typedef int socklen_t;
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netinet/tcp.h>
#endif

// Let's start with a simple JSON parser
// You can download this single header from: https://github.com/nlohmann/json/releases
// Just grab json.hpp and put it in your project folder
#include "json.hpp"
using json = nlohmann::json;

namespace whispr::network {

// Platform initialization
class platform_init {
public:
    platform_init() {
        #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw std::runtime_error("Failed to initialize Windows sockets");
        }
        #endif
    }
    
    ~platform_init() {
        #ifdef _WIN32
        WSACleanup();
        #endif
    }
};

static platform_init platform_initializer;

// Simple message structures
struct beacon_message {
    std::string source_id;
    std::string message_type;
    uint64_t timestamp_ns;
    std::string payload;
    uint32_t sequence_number;
    bool is_critical;
    double parse_time_us;
};

// Convert to/from JSON
void to_json(json& j, const beacon_message& msg) {
    j = json{
        {"source_id", msg.source_id},
        {"message_type", msg.message_type},
        {"timestamp_ns", msg.timestamp_ns},
        {"payload", msg.payload},
        {"sequence_number", msg.sequence_number},
        {"is_critical", msg.is_critical},
        {"parse_time_us", msg.parse_time_us}
    };
}

void from_json(const json& j, beacon_message& msg) {
    j.at("source_id").get_to(msg.source_id);
    j.at("message_type").get_to(msg.message_type);
    j.at("timestamp_ns").get_to(msg.timestamp_ns);
    j.at("payload").get_to(msg.payload);
    j.at("sequence_number").get_to(msg.sequence_number);
    j.at("is_critical").get_to(msg.is_critical);
    msg.parse_time_us = j.value("parse_time_us", 0.0);
}

// Simple thread-safe queue
template<typename T>
class safe_queue {
private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    
public:
    void push(T item) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::move(item));
    }
    
    bool pop(T& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) return false;
        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }
    
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }
};

// Simplified beacon transmitter
class lighthouse_beacon {
private:
    #ifdef _WIN32
        SOCKET socket_fd_;
    #else
        int socket_fd_;
    #endif
    sockaddr_in destination_;
    std::atomic<uint32_t> sequence_counter_{0};
    std::atomic<bool> is_active_{false};
    std::thread beacon_thread_;
    std::string target_host_;
    uint16_t target_port_;
    uint32_t interval_ms_;
    
public:
    lighthouse_beacon(const std::string& host, uint16_t port, uint32_t interval) 
        : target_host_(host), target_port_(port), interval_ms_(interval),
          socket_fd_(
        #ifdef _WIN32
            INVALID_SOCKET
        #else
            -1
        #endif
        ) {
        initialize_socket();
    }
    
    ~lighthouse_beacon() {
        stop();
        #ifdef _WIN32
            if (socket_fd_ != INVALID_SOCKET) closesocket(socket_fd_);
        #else
            if (socket_fd_ >= 0) close(socket_fd_);
        #endif
    }
    
    bool initialize_socket() {
        socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
        #ifdef _WIN32
            if (socket_fd_ == INVALID_SOCKET) {
                std::cerr << "Socket creation failed\n";
                return false;
            }
        #else
            if (socket_fd_ < 0) {
                std::cerr << "Socket creation failed\n";
                return false;
            }
        #endif
        
        destination_.sin_family = AF_INET;
        destination_.sin_port = htons(target_port_);
        if (inet_pton(AF_INET, target_host_.c_str(), &destination_.sin_addr) <= 0) {
            std::cerr << "Invalid IP address: " << target_host_ << std::endl;
            return false;
        }
        
        return true;
    }
    
    void start() {
        if (is_active_.exchange(true)) return;
        
        beacon_thread_ = std::thread([this]() {
            beacon_loop();
        });
        
        std::cout << "Lighthouse beacon started - Target: " 
                  << target_host_ << ":" << target_port_ << std::endl;
    }
    
    void stop() {
        if (!is_active_.exchange(false)) return;
        if (beacon_thread_.joinable()) {
            beacon_thread_.join();
        }
    }
    
private:
    void beacon_loop() {
        while (is_active_.load()) {
            beacon_message msg;
            msg.source_id = "whispr-lighthouse-minimal";
            msg.message_type = "heartbeat";
            msg.timestamp_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            msg.payload = "Seq:" + std::to_string(sequence_counter_.load());
            msg.sequence_number = sequence_counter_.fetch_add(1);
            msg.is_critical = (msg.sequence_number % 10 == 0);
            msg.parse_time_us = 0.0;
            
            // Convert to JSON
            json j = msg;
            std::string json_str = j.dump();
            
            // Send it
            int bytes_sent = sendto(socket_fd_, 
                                  json_str.c_str(), 
                                  static_cast<int>(json_str.length()), 
                                  0,
                                  reinterpret_cast<sockaddr*>(&destination_), 
                                  sizeof(destination_));
            
            if (bytes_sent > 0) {
                std::cout << "Beacon #" << msg.sequence_number 
                         << " sent (" << bytes_sent << " bytes)" << std::endl;
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms_));
        }
    }
};

// Simplified listener
class network_listener {
private:
    #ifdef _WIN32
        SOCKET server_fd_;
    #else
        int server_fd_;
    #endif
    uint16_t listen_port_;
    std::atomic<bool> is_active_{false};
    std::thread listener_thread_;
    std::vector<std::thread> worker_threads_;
    safe_queue<std::pair<std::string, std::string>> parse_queue_;
    
    // Stats
    std::atomic<uint64_t> packets_received_{0};
    std::atomic<uint64_t> parse_errors_{0};
    
public:
    network_listener(uint16_t port) : listen_port_(port), server_fd_(
        #ifdef _WIN32
            INVALID_SOCKET
        #else
            -1
        #endif
        ) {
        initialize_socket();
    }
    
    ~network_listener() {
        stop();
        #ifdef _WIN32
            if (server_fd_ != INVALID_SOCKET) closesocket(server_fd_);
        #else
            if (server_fd_ >= 0) close(server_fd_);
        #endif
    }
    
    bool initialize_socket() {
        server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        #ifdef _WIN32
            if (server_fd_ == INVALID_SOCKET) {
                std::cerr << "Socket creation failed\n";
                return false;
            }
        #else
            if (server_fd_ < 0) {
                std::cerr << "Socket creation failed\n";
                return false;
            }
        #endif
        
        int opt = 1;
        setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, 
                  reinterpret_cast<const char*>(&opt), sizeof(opt));
        
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(listen_port_);
        
        if (bind(server_fd_, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) {
            std::cerr << "Bind failed\n";
            return false;
        }
        
        if (listen(server_fd_, 10) < 0) {
            std::cerr << "Listen failed\n";
            return false;
        }
        
        return true;
    }
    
    void start() {
        if (is_active_.exchange(true)) return;
        
        // Start parser threads
        for (int i = 0; i < 2; ++i) {
            worker_threads_.emplace_back([this, i]() {
                parser_worker(i);
            });
        }
        
        listener_thread_ = std::thread([this]() {
            accept_loop();
        });
        
        std::cout << "Network listener started - Port: " << listen_port_ << std::endl;
    }
    
    void stop() {
        if (!is_active_.exchange(false)) return;
        
        if (listener_thread_.joinable()) {
            listener_thread_.join();
        }
        
        for (auto& worker : worker_threads_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        
        std::cout << "\nListener Stats:\n";
        std::cout << "  Packets received: " << packets_received_.load() << "\n";
        std::cout << "  Parse errors: " << parse_errors_.load() << "\n";
    }
    
private:
    void accept_loop() {
        while (is_active_.load()) {
            sockaddr_in client_addr{};
            socklen_t client_len = sizeof(client_addr);
            
            #ifdef _WIN32
                SOCKET client_fd = accept(server_fd_, 
                                        reinterpret_cast<sockaddr*>(&client_addr), 
                                        &client_len);
                if (client_fd != INVALID_SOCKET) {
            #else
                int client_fd = accept(server_fd_, 
                                     reinterpret_cast<sockaddr*>(&client_addr), 
                                     &client_len);
                if (client_fd >= 0) {
            #endif
                char client_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
                
                std::thread([this, client_fd, client_ip]() {
                    handle_client(client_fd, std::string(client_ip));
                }).detach();
            }
        }
    }
    
    void handle_client(
    #ifdef _WIN32
        SOCKET client_fd,
    #else  
        int client_fd,
    #endif
        const std::string& client_ip) {
        
        char buffer[4096];
        std::string message_buffer;
        
        while (is_active_.load()) {
            int bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
            if (bytes > 0) {
                buffer[bytes] = '\0';
                message_buffer += buffer;
                
                // Simple JSON extraction (look for complete objects)
                size_t start = 0;
                int braces = 0;
                for (size_t i = 0; i < message_buffer.size(); ++i) {
                    if (message_buffer[i] == '{') braces++;
                    else if (message_buffer[i] == '}') {
                        braces--;
                        if (braces == 0) {
                            std::string json_msg = message_buffer.substr(start, i - start + 1);
                            parse_queue_.push({json_msg, client_ip});
                            packets_received_++;
                            start = i + 1;
                        }
                    }
                }
                
                if (start < message_buffer.size()) {
                    message_buffer = message_buffer.substr(start);
                } else {
                    message_buffer.clear();
                }
            } else {
                break;
            }
        }
        
        #ifdef _WIN32
            closesocket(client_fd);
        #else
            close(client_fd);
        #endif
    }
    
    void parser_worker(int thread_id) {
        std::cout << "Parser thread " << thread_id << " started\n";
        
        while (is_active_.load()) {
            std::pair<std::string, std::string> job;
            
            if (parse_queue_.pop(job)) {
                auto start = std::chrono::high_resolution_clock::now();
                
                try {
                    json j = json::parse(job.first);
                    beacon_message msg = j;
                    
                    auto end = std::chrono::high_resolution_clock::now();
                    double parse_us = std::chrono::duration_cast<std::chrono::nanoseconds>(
                        end - start).count() / 1000.0;
                    
                    std::cout << "[Thread " << thread_id << "] "
                             << "[" << job.second << "] Beacon #" << msg.sequence_number
                             << " (Parse: " << parse_us << "μs)" << std::endl;
                    
                } catch (const json::exception& e) {
                    parse_errors_++;
                    std::cerr << "[Thread " << thread_id << "] Parse error: " << e.what() << "\n";
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }
};

} // namespace whispr::network

int main(int argc, char* argv[]) {
    try {
        // Default config
        std::string target_host = "127.0.0.1";
        uint16_t target_port = 9001;
        uint16_t listen_port = 9000;
        uint32_t interval_ms = 1000;
        
        // Simple arg parsing
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "--target" && i + 1 < argc) {
                target_host = argv[++i];
            } else if (arg == "--listen-port" && i + 1 < argc) {
                listen_port = static_cast<uint16_t>(std::stoi(argv[++i]));
            } else if (arg == "--help") {
                std::cout << "Usage: " << argv[0] << " [options]\n"
                          << "  --target HOST      Target host (default: 127.0.0.1)\n"
                          << "  --listen-port PORT Listen port (default: 9000)\n";
                return 0;
            }
        }
        
        std::cout << "=== WHISPR LIGHTHOUSE (Minimal) ===\n";
        std::cout << "Using nlohmann/json for parsing\n";
        std::cout << "===================================\n\n";
        
        // Create components
        whispr::network::lighthouse_beacon beacon(target_host, target_port, interval_ms);
        whispr::network::network_listener listener(listen_port);
        
        // Start them
        beacon.start();
        listener.start();
        
        // Wait for user
        std::cout << "\nPress Enter to stop...\n";
        std::cin.get();
        
        // Clean shutdown
        beacon.stop();
        listener.stop();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}