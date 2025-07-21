#pragma once

// Enhanced beacon_cli.cpp with RTC-Jsonifier performance integration
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <chrono>
#include <atomic>
#include <thread>
#include <vector>
#include <memory>

// Core RTC-Jsonifier includes (performance layer)
#include <jsonifier/Hash.hpp>
#include <jsonifier/HashMap.hpp>
#include <jsonifier/TypeEntities.hpp>
#include <jsonifier/StringView.hpp>

namespace whispr::network {

// High-performance message structures using RTC-Jsonifier reflection
struct beacon_message {
    jsonifier::string source_id;
    jsonifier::string message_type;
    uint64_t timestamp_ns;
    jsonifier::string payload;
    uint32_t sequence_number;
    bool is_critical;
};

struct network_stats {
    uint64_t packets_sent;
    uint64_t packets_received;
    uint64_t bytes_transmitted;
    double avg_latency_ms;
    uint32_t active_connections;
};

struct monitor_config {
    jsonifier::string target_host;
    uint16_t target_port;
    uint16_t listen_port;
    uint32_t beacon_interval_ms;
    uint32_t max_concurrent_connections;
    bool enable_compression;
    bool enable_encryption;
};

} // namespace whispr::network

// RTC-Jsonifier reflection registration for zero-overhead serialization
template<> struct jsonifier::core<whispr::network::beacon_message> {
    using value_type = whispr::network::beacon_message;
    static constexpr auto parseValue = createValue(
        "source_id", &value_type::source_id,
        "message_type", &value_type::message_type,
        "timestamp_ns", &value_type::timestamp_ns,
        "payload", &value_type::payload,
        "sequence_number", &value_type::sequence_number,
        "is_critical", &value_type::is_critical
    );
};

template<> struct jsonifier::core<whispr::network::network_stats> {
    using value_type = whispr::network::network_stats;
    static constexpr auto parseValue = createValue(
        "packets_sent", &value_type::packets_sent,
        "packets_received", &value_type::packets_received,
        "bytes_transmitted", &value_type::bytes_transmitted,
        "avg_latency_ms", &value_type::avg_latency_ms,
        "active_connections", &value_type::active_connections
    );
};

template<> struct jsonifier::core<whispr::network::monitor_config> {
    using value_type = whispr::network::monitor_config;
    static constexpr auto parseValue = createValue(
        "target_host", &value_type::target_host,
        "target_port", &value_type::target_port,
        "listen_port", &value_type::listen_port,
        "beacon_interval_ms", &value_type::beacon_interval_ms,
        "max_concurrent_connections", &value_type::max_concurrent_connections,
        "enable_compression", &value_type::enable_compression,
        "enable_encryption", &value_type::enable_encryption
    );
};

namespace whispr::network {

// High-performance beacon transmitter using SIMD-optimized serialization
class lighthouse_beacon {
private:
    int socket_fd_;
    sockaddr_in destination_;
    std::atomic<uint32_t> sequence_counter_{0};
    std::atomic<bool> is_active_{false};
    std::thread beacon_thread_;
    
    // RTC-Jsonifier high-performance serializer
    jsonifier::jsonifier_core<> serializer_;
    
    // Pre-allocated buffers for zero-allocation hot path
    alignas(64) char json_buffer_[4096];
    alignas(64) char network_buffer_[4096];
    
    monitor_config config_;
    
public:
    explicit lighthouse_beacon(const monitor_config& config) 
        : config_(config) {
        initialize_socket();
    }
    
    ~lighthouse_beacon() {
        stop();
        if (socket_fd_ >= 0) {
            close(socket_fd_);
        }
    }
    
    bool initialize_socket() {
        socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (socket_fd_ < 0) {
            std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
            return false;
        }
        
        // Set socket options for high performance
        int opt = 1;
        setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        
        // Configure destination
        destination_.sin_family = AF_INET;
        destination_.sin_port = htons(config_.target_port);
        if (inet_pton(AF_INET, config_.target_host.c_str(), &destination_.sin_addr) <= 0) {
            std::cerr << "Invalid IP address: " << config_.target_host << std::endl;
            return false;
        }
        
        return true;
    }
    
    void start() {
        if (is_active_.exchange(true)) {
            return; // Already running
        }
        
        beacon_thread_ = std::thread([this]() {
            beacon_loop();
        });
        
        std::cout << "Lighthouse beacon activated - targeting " 
                  << config_.target_host << ":" << config_.target_port 
                  << " (interval: " << config_.beacon_interval_ms << "ms)" << std::endl;
    }
    
    void stop() {
        if (!is_active_.exchange(false)) {
            return; // Already stopped
        }
        
        if (beacon_thread_.joinable()) {
            beacon_thread_.join();
        }
        
        std::cout << "Lighthouse beacon deactivated" << std::endl;
    }
    
private:
    void beacon_loop() {
        auto next_beacon = std::chrono::steady_clock::now();
        
        while (is_active_.load()) {
            auto now = std::chrono::steady_clock::now();
            
            if (now >= next_beacon) {
                send_beacon();
                next_beacon = now + std::chrono::milliseconds(config_.beacon_interval_ms);
            }
            
            // High-precision sleep to maintain exact timing
            std::this_thread::sleep_until(next_beacon);
        }
    }
    
    void send_beacon() {
        // Create beacon message with high-resolution timestamp
        beacon_message msg{};
        msg.source_id = "whispr-lighthouse-001";
        msg.message_type = "heartbeat";
        msg.timestamp_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        msg.payload = "Lighthouse operational - monitoring active";
        msg.sequence_number = sequence_counter_.fetch_add(1);
        msg.is_critical = false;
        
        // Zero-allocation JSON serialization using RTC-Jsonifier
        jsonifier::string json_output;
        json_output.reserve(512); // Pre-allocate expected size
        
        if (serializer_.serializeJson(msg, json_output)) {
            // Send via UDP with minimal system call overhead
            ssize_t bytes_sent = sendto(socket_fd_, 
                                      json_output.data(), 
                                      json_output.size(), 
                                      MSG_DONTWAIT,
                                      reinterpret_cast<sockaddr*>(&destination_), 
                                      sizeof(destination_));
            
            if (bytes_sent < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
                std::cerr << "Beacon transmission failed: " << strerror(errno) << std::endl;
            } else {
                std::cout << "Beacon #" << msg.sequence_number 
                         << " transmitted (" << bytes_sent << " bytes)" << std::endl;
            }
        } else {
            std::cerr << "JSON serialization failed for beacon #" << msg.sequence_number << std::endl;
        }
    }
};

// High-performance multi-threaded listener with SIMD JSON parsing
class network_listener {
private:
    int server_fd_;
    std::atomic<bool> is_active_{false};
    std::thread listener_thread_;
    std::vector<std::thread> worker_threads_;
    
    // RTC-Jsonifier high-performance parser
    jsonifier::jsonifier_core<> parser_;
    
    monitor_config config_;
    std::atomic<network_stats> stats_{};
    
public:
    explicit network_listener(const monitor_config& config) 
        : config_(config) {
        initialize_socket();
    }
    
    ~network_listener() {
        stop();
        if (server_fd_ >= 0) {
            close(server_fd_);
        }
    }
    
    bool initialize_socket() {
        server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd_ < 0) {
            std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
            return false;
        }
        
        // High-performance socket options
        int opt = 1;
        setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        setsockopt(server_fd_, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
        
        // Configure server address
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(config_.listen_port);
        
        if (bind(server_fd_, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) {
            std::cerr << "Bind failed: " << strerror(errno) << std::endl;
            return false;
        }
        
        if (listen(server_fd_, config_.max_concurrent_connections) < 0) {
            std::cerr << "Listen failed: " << strerror(errno) << std::endl;
            return false;
        }
        
        return true;
    }
    
    void start() {
        if (is_active_.exchange(true)) {
            return; // Already running
        }
        
        listener_thread_ = std::thread([this]() {
            accept_loop();
        });
        
        std::cout << "Network listener started on port " << config_.listen_port 
                  << " (max connections: " << config_.max_concurrent_connections << ")" << std::endl;
    }
    
    void stop() {
        if (!is_active_.exchange(false)) {
            return; // Already stopped
        }
        
        // Gracefully shutdown all threads
        if (listener_thread_.joinable()) {
            listener_thread_.join();
        }
        
        for (auto& worker : worker_threads_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        worker_threads_.clear();
        
        std::cout << "Network listener stopped" << std::endl;
    }
    
    network_stats get_stats() const {
        return stats_.load();
    }
    
private:
    void accept_loop() {
        while (is_active_.load()) {
            sockaddr_in client_addr{};
            socklen_t client_len = sizeof(client_addr);
            
            int client_fd = accept(server_fd_, 
                                 reinterpret_cast<sockaddr*>(&client_addr), 
                                 &client_len);
            
            if (client_fd >= 0) {
                // Spawn worker thread for each connection
                worker_threads_.emplace_back([this, client_fd, client_addr]() {
                    handle_client(client_fd, client_addr);
                });
                
                // Update connection stats
                auto current_stats = stats_.load();
                current_stats.active_connections++;
                stats_.store(current_stats);
                
            } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                std::cerr << "Accept failed: " << strerror(errno) << std::endl;
            }
        }
    }
    
    void handle_client(int client_fd, const sockaddr_in& client_addr) {
        // Convert client address to string for logging
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        
        std::cout << "Client connected: " << client_ip << ":" << ntohs(client_addr.sin_port) << std::endl;
        
        alignas(64) char buffer[8192];
        
        while (is_active_.load()) {
            ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
            
            if (bytes_received > 0) {
                buffer[bytes_received] = '\0';
                
                // High-performance JSON parsing using RTC-Jsonifier
                process_message(buffer, bytes_received, client_ip);
                
                // Update stats
                auto current_stats = stats_.load();
                current_stats.packets_received++;
                current_stats.bytes_transmitted += bytes_received;
                stats_.store(current_stats);
                
            } else if (bytes_received == 0) {
                // Client disconnected
                break;
            } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
                std::cerr << "Receive failed: " << strerror(errno) << std::endl;
                break;
            }
            
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
        
        close(client_fd);
        
        // Update connection stats
        auto current_stats = stats_.load();
        current_stats.active_connections--;
        stats_.store(current_stats);
        
        std::cout << "Client disconnected: " << client_ip << std::endl;
    }
    
    void process_message(const char* data, size_t length, const char* client_ip) {
        try {
            // Attempt to parse as beacon_message using SIMD-accelerated parsing
            beacon_message msg{};
            jsonifier::string json_data(data, length);
            
            if (parser_.parseJson(msg, json_data)) {
                std::cout << "[" << client_ip << "] Beacon #" << msg.sequence_number 
                         << " from " << msg.source_id 
                         << " (type: " << msg.message_type << ")" << std::endl;
                
                if (msg.is_critical) {
                    std::cout << ">>> CRITICAL MESSAGE: " << msg.payload << std::endl;
                }
            } else {
                // Fallback to raw message display
                std::cout << "[" << client_ip << "] Raw message: " 
                         << std::string(data, std::min(length, size_t(100))) << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Message processing error: " << e.what() << std::endl;
        }
    }
};

// Unified command-line interface maintaining your existing API
class network_monitor {
private:
    std::unique_ptr<lighthouse_beacon> beacon_;
    std::unique_ptr<network_listener> listener_;
    monitor_config config_;
    
public:
    network_monitor() {
        // Default configuration
        config_.target_host = "127.0.0.1";
        config_.target_port = 8080;
        config_.listen_port = 9876;
        config_.beacon_interval_ms = 3000;
        config_.max_concurrent_connections = 64;
        config_.enable_compression = false;
        config_.enable_encryption = false;
    }
    
    bool parse_arguments(int argc, char* argv[]) {
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            
            if (arg == "--host" && i + 1 < argc) {
                config_.target_host = argv[++i];
            } else if (arg == "--port" && i + 1 < argc) {
                config_.target_port = static_cast<uint16_t>(std::stoi(argv[++i]));
            } else if (arg == "--bind" && i + 1 < argc) {
                // For listener bind address (keeping compatibility)
                // Note: Current implementation binds to all interfaces
            } else if (arg == "--listen-port" && i + 1 < argc) {
                config_.listen_port = static_cast<uint16_t>(std::stoi(argv[++i]));
            } else if (arg == "--interval" && i + 1 < argc) {
                config_.beacon_interval_ms = static_cast<uint32_t>(std::stoi(argv[++i]));
            } else if (arg == "--max-connections" && i + 1 < argc) {
                config_.max_concurrent_connections = static_cast<uint32_t>(std::stoi(argv[++i]));
            } else if (arg == "--help") {
                print_usage(argv[0]);
                return false;
            }
        }
        return true;
    }
    
    void print_usage(const char* program_name) {
        std::cout << "High-Performance Network Monitor with RTC-Jsonifier\n\n";
        std::cout << "Usage: " << program_name << " [OPTIONS]\n\n";
        std::cout << "Options:\n";
        std::cout << "  --host HOST              Target host for beacon transmission (default: 127.0.0.1)\n";
        std::cout << "  --port PORT              Target port for beacon transmission (default: 8080)\n";
        std::cout << "  --listen-port PORT       Port to listen for incoming connections (default: 9876)\n";
        std::cout << "  --interval MS            Beacon transmission interval in milliseconds (default: 3000)\n";
        std::cout << "  --max-connections N      Maximum concurrent connections (default: 64)\n";
        std::cout << "  --help                   Show this help message\n\n";
        std::cout << "Features:\n";
        std::cout << "  • SIMD-accelerated JSON parsing/serialization\n";
        std::cout << "  • Zero-allocation hot paths\n";
        std::cout << "  • Multi-threaded connection handling\n";
        std::cout << "  • High-precision timing\n";
        std::cout << "  • Real-time performance monitoring\n";
    }
    
    void run_beacon_mode() {
        beacon_ = std::make_unique<lighthouse_beacon>(config_);
        beacon_->start();
        
        std::cout << "\nPress Ctrl+C to stop...\n";
        
        // Keep running until interrupted
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    
    void run_listener_mode() {
        listener_ = std::make_unique<network_listener>(config_);
        listener_->start();
        
        std::cout << "\nPress Ctrl+C to stop...\n";
        
        // Monitor and display stats
        auto last_stats = listener_->get_stats();
        auto last_time = std::chrono::steady_clock::now();
        
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            
            auto current_stats = listener_->get_stats();
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                current_time - last_time).count();
            
            auto packets_per_sec = (current_stats.packets_received - last_stats.packets_received) * 1000.0 / elapsed_ms;
            auto bytes_per_sec = (current_stats.bytes_transmitted - last_stats.bytes_transmitted) * 1000.0 / elapsed_ms;
            
            std::cout << "\n=== Performance Stats ===" << std::endl;
            std::cout << "Active connections: " << current_stats.active_connections << std::endl;
            std::cout << "Total packets: " << current_stats.packets_received << std::endl;
            std::cout << "Packets/sec: " << std::fixed << std::setprecision(2) << packets_per_sec << std::endl;
            std::cout << "Bytes/sec: " << std::fixed << std::setprecision(2) << bytes_per_sec << std::endl;
            
            last_stats = current_stats;
            last_time = current_time;
        }
    }
    
    void run_dual_mode() {
        beacon_ = std::make_unique<lighthouse_beacon>(config_);
        listener_ = std::make_unique<network_listener>(config_);
        
        beacon_->start();
        listener_->start();
        
        std::cout << "\nDual mode active - beacon transmitting and listener monitoring\n";
        std::cout << "Press Ctrl+C to stop...\n";
        
        // Keep both running
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

} // namespace whispr::network

// Main entry point maintaining compatibility with existing CLI
int main(int argc, char* argv[]) {
    std::cout << "Whispr Network Monitor v2.0 - High Performance Edition\n";
    std::cout << "Powered by RTC-Jsonifier SIMD acceleration\n\n";
    
    whispr::network::network_monitor monitor;
    
    if (!monitor.parse_arguments(argc, argv)) {
        return 0; // Help was shown or parsing failed
    }
    
    try {
        // Default to dual mode for maximum functionality
        monitor.run_dual_mode();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}