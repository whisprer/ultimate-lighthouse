// File: beacon_udp_sender.cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

// Configuration
const char* BASE_IP = "192.168.1.100";    // Starting IP address
const int TARGET_PORT = 9876;             // Target port
const int INTERVAL_MS = 10000;            // 10 seconds between pings
const int IP_COUNT = 10;                  // Number of sequential IPs to cycle through

class BeaconSender {
private:
    SOCKET sock;
    std::vector<std::string> target_ips;
    int current_ip_index;
    
public:
    BeaconSender() : current_ip_index(0) {
        // Initialize Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw std::runtime_error("WSAStartup failed");
        }
        
        // Create socket
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock == INVALID_SOCKET) {
            WSACleanup();
            throw std::runtime_error("Socket creation failed");
        }
        
        // Generate sequential IP addresses
        generate_ip_list();
    }
    
    ~BeaconSender() {
        closesocket(sock);
        WSACleanup();
    }
    
    void generate_ip_list() {
        // Parse base IP and generate sequential IPs
        std::string base_ip = BASE_IP;
        size_t last_dot = base_ip.find_last_of('.');
        
        if (last_dot == std::string::npos) {
            throw std::runtime_error("Invalid base IP format");
        }
        
        std::string ip_prefix = base_ip.substr(0, last_dot + 1);
        int last_octet = std::stoi(base_ip.substr(last_dot + 1));
        
        target_ips.clear();
        for (int i = 0; i < IP_COUNT; ++i) {
            int new_octet = last_octet + i;
            if (new_octet > 255) {
                new_octet = new_octet - 256; // Wrap around if needed
            }
            target_ips.push_back(ip_prefix + std::to_string(new_octet));
        }
        
        // Print the IP range we'll be cycling through
        std::cout << "🎯 Beacon will cycle through IPs:\n";
        for (size_t i = 0; i < target_ips.size(); ++i) {
            std::cout << "   " << (i + 1) << ": " << target_ips[i] << ":" << TARGET_PORT << "\n";
        }
        std::cout << "\n";
    }
    
    std::string create_beacon_message() {
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(
            now.time_since_epoch()).count();
        
        // Create JSON beacon with useful info
        std::string message = "{"
            "\"type\":\"BEACON_PING\","
            "\"timestamp\":" + std::to_string(timestamp) + ","
            "\"sequence\":" + std::to_string(current_ip_index + 1) + ","
            "\"target_ip\":\"" + target_ips[current_ip_index] + "\","
            "\"total_targets\":" + std::to_string(IP_COUNT) + ","
            "\"cycle_complete\":" + (current_ip_index == IP_COUNT - 1 ? "true" : "false") +
            "}";
        
        return message;
    }
    
    bool send_beacon() {
        std::string current_ip = target_ips[current_ip_index];
        std::string beacon_msg = create_beacon_message();
        
        sockaddr_in dest;
        dest.sin_family = AF_INET;
        dest.sin_port = htons(TARGET_PORT);
        dest.sin_addr.s_addr = inet_addr(current_ip.c_str());
        
        int sent = sendto(sock, beacon_msg.c_str(), beacon_msg.length(), 0, 
                         (sockaddr*)&dest, sizeof(dest));
        
        if (sent == SOCKET_ERROR) {
            std::cerr << "❌ Send failed to " << current_ip << ":" << TARGET_PORT 
                      << " (Error: " << WSAGetLastError() << ")\n";
            return false;
        } else {
            std::cout << "🚀 Beacon [" << (current_ip_index + 1) << "/" << IP_COUNT 
                      << "] sent to " << current_ip << ":" << TARGET_PORT;
            
            // Add cycle completion indicator
            if (current_ip_index == IP_COUNT - 1) {
                std::cout << " ✅ CYCLE COMPLETE";
            }
            std::cout << "\n";
            return true;
        }
    }
    
    void run() {
        std::cout << "🎯 Starting UDP Beacon Sender\n";
        std::cout << "📡 Interval: " << INTERVAL_MS << "ms\n";
        std::cout << "🔄 Cycling through " << IP_COUNT << " sequential IPs\n\n";
        
        int total_sent = 0;
        int cycles_completed = 0;
        
        while (true) {
            if (send_beacon()) {
                total_sent++;
            }
            
            // Move to next IP in sequence
            current_ip_index = (current_ip_index + 1) % IP_COUNT;
            
            // Track completed cycles
            if (current_ip_index == 0) {
                cycles_completed++;
                std::cout << "🔄 Completed cycle #" << cycles_completed 
                          << " (Total beacons sent: " << total_sent << ")\n\n";
            }
            
            // Sleep until next beacon
            std::this_thread::sleep_for(std::chrono::milliseconds(INTERVAL_MS));
        }
    }
};

// Utility function for JSON minification (from your original code)
std::string minify_json_avx2(const std::string& input) {
    std::string output;
    output.reserve(input.size());

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        if (c != ' ' && c != '\n' && c != '\t' && c != '\r') {
            output += c;
        }
    }
    return output;
}

int main() {
    try {
        BeaconSender beacon;
        beacon.run();
    }
    catch (const std::exception& e) {
        std::cerr << "💥 Fatal error: " << e.what() << std::endl;
        std::cout << "Press any key to exit...";
        std::cin.get();
        return 1;
    }
    
    return 0;
}