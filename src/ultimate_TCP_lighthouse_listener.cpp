#include <jsonifier/Index.hpp>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <iomanip>
#include <sstream>

// 🎧 ULTIMATE TCP LIGHTHOUSE LISTENER
// Enhanced to parse and display the ultra-fast JSON data
// from the Ultimate Public Lighthouse Beacon System

struct UltimateBeaconPayload {
    jsonifier::string lighthouse_id{};
    jsonifier::string lighthouse_type{};
    jsonifier::string version{};
    uint64_t timestamp{0};
    jsonifier::string status{};
    
    // FastPing integration
    jsonifier::string fastping_status{};
    jsonifier::string connecting_ip{};
    jsonifier::string anonymity_level{};
    jsonifier::string speed_hint{};
    double ping_latency_ms{0.0};
    uint32_t signal_age_seconds{0};
    
    // Ultra-performance metrics
    double json_parse_time_microseconds{0.0};
    double json_serialize_time_microseconds{0.0};
    uint64_t total_fastping_requests{0};
    uint64_t successful_parses{0};
    double average_throughput_mbps{0.0};
    jsonifier::string cpu_optimization_level{};
    
    // Global broadcasting stats
    uint32_t connected_global_listeners{0};
    uint64_t total_beacons_broadcast{0};
    double system_uptime_hours{0.0};
    uint32_t beacon_sequence_number{0};
    
    // Marketing info
    jsonifier::string public_website{};
    jsonifier::string fastping_service{};
    jsonifier::string company{};
    jsonifier::string showcase_message{};
};

class UltimateTcpListener {
private:
    jsonifier::jsonifier_core<> json_core{};
    std::string server_ip;
    int server_port;
    int packet_count{0};
    
    void print_timestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::cout << "[" << std::put_time(std::localtime(&time_t), "%H:%M:%S") << "] ";
    }
    
    void display_beacon_data(const UltimateBeaconPayload& beacon) {
        auto beacon_time = std::chrono::system_clock::from_time_t(beacon.timestamp);
        auto beacon_time_t = std::chrono::system_clock::to_time_t(beacon_time);
        
        std::cout << "\n┌─────────────────────────────────────────────────────────────┐\n";
        std::cout << "│ 🏰 ULTIMATE LIGHTHOUSE BEACON RECEIVED                     │\n";
        std::cout << "├─────────────────────────────────────────────────────────────┤\n";
        
        std::cout << "│ 🆔 ID: " << std::left << std::setw(50) << beacon.lighthouse_id << " │\n";
        std::cout << "│ 📦 Type: " << std::left << std::setw(48) << beacon.lighthouse_type << " │\n";
        std::cout << "│ 🚀 Version: " << std::left << std::setw(45) << beacon.version << " │\n";
        
        // Health status with icon
        std::string status_display = beacon.status;
        if (beacon.status == "healthy") status_display = "✅ " + beacon.status;
        else if (beacon.status == "warning") status_display = "⚠️  " + beacon.status;
        else if (beacon.status == "critical") status_display = "❌ " + beacon.status;
        
        std::cout << "│ 💚 Status: " << std::left << std::setw(46) << status_display << " │\n";
        std::cout << "│ 🌐 FastPing: " << std::left << std::setw(44) << beacon.fastping_status << " │\n";
        std::cout << "│ 📍 IP: " << std::left << std::setw(50) << beacon.connecting_ip << " │\n";
        std::cout << "│ 🔒 Anonymity: " << std::left << std::setw(42) << beacon.anonymity_level << " │\n";
        std::cout << "│ ⚡ Speed: " << std::left << std::setw(47) << beacon.speed_hint << " │\n";
        
        std::ostringstream latency_str;
        latency_str << std::fixed << std::setprecision(1) << beacon.ping_latency_ms << "ms";
        std::cout << "│ 📡 Ping Latency: " << std::left << std::setw(38) << latency_str.str() << " │\n";
        
        std::ostringstream timestamp_str;
        timestamp_str << std::put_time(std::localtime(&beacon_time_t), "%Y-%m-%d %H:%M:%S");
        std::cout << "│ 🕒 Timestamp: " << std::left << std::setw(41) << timestamp_str.str() << " │\n";
        
        std::cout << "├─────────────────────────────────────────────────────────────┤\n";
        std::cout << "│ 🚀 ULTRA-PERFORMANCE METRICS                               │\n";
        std::cout << "├─────────────────────────────────────────────────────────────┤\n";
        
        std::ostringstream parse_time_str;
        parse_time_str << std::fixed << std::setprecision(3) << beacon.json_parse_time_microseconds << "µs";
        std::cout << "│ ⚡ Parse Time: " << std::left << std::setw(42) << parse_time_str.str() << " │\n";
        
        std::ostringstream serialize_time_str;
        serialize_time_str << std::fixed << std::setprecision(3) << beacon.json_serialize_time_microseconds << "µs";
        std::cout << "│ 📤 Serialize Time: " << std::left << std::setw(36) << serialize_time_str.str() << " │\n";
        
        std::ostringstream throughput_str;
        throughput_str << std::fixed << std::setprecision(1) << beacon.average_throughput_mbps << " MB/s";
        std::cout << "│ 🔥 Throughput: " << std::left << std::setw(41) << throughput_str.str() << " │\n";
        
        std::cout << "│ 🖥️  CPU Optimization: " << std::left << std::setw(32) << beacon.cpu_optimization_level << " │\n";
        std::cout << "│ 📊 Total Requests: " << std::left << std::setw(36) << std::to_string(beacon.total_fastping_requests) << " │\n";
        std::cout << "│ ✅ Successful Parses: " << std::left << std::setw(32) << std::to_string(beacon.successful_parses) << " │\n";
        
        double success_rate = beacon.total_fastping_requests > 0 ? 
                            (double)beacon.successful_parses / beacon.total_fastping_requests * 100.0 : 0.0;
        std::ostringstream success_str;
        success_str << std::fixed << std::setprecision(1) << success_rate << "%";
        std::cout << "│ 📈 Success Rate: " << std::left << std::setw(38) << success_str.str() << " │\n";
        
        std::cout << "├─────────────────────────────────────────────────────────────┤\n";
        std::cout << "│ 🌍 GLOBAL BROADCASTING STATISTICS                          │\n";
        std::cout << "├─────────────────────────────────────────────────────────────┤\n";
        
        std::cout << "│ 🎧 Connected Listeners: " << std::left << std::setw(30) << std::to_string(beacon.connected_global_listeners) << " │\n";
        std::cout << "│ 📡 Total Beacons Sent: " << std::left << std::setw(30) << std::to_string(beacon.total_beacons_broadcast) << " │\n";
        std::cout << "│ 🔢 Beacon Sequence: #" << std::left << std::setw(32) << std::to_string(beacon.beacon_sequence_number) << " │\n";
        
        std::ostringstream uptime_str;
        uptime_str << std::fixed << std::setprecision(1) << beacon.system_uptime_hours << " hours";
        std::cout << "│ ⏰ System Uptime: " << std::left << std::setw(37) << uptime_str.str() << " │\n";
        
        std::cout << "├─────────────────────────────────────────────────────────────┤\n";
        std::cout << "│ 🏢 COMPANY & SERVICES                                      │\n";
        std::cout << "├─────────────────────────────────────────────────────────────┤\n";
        
        if (!beacon.public_website.empty()) {
            std::cout << "│ 🌐 Website: " << std::left << std::setw(45) << beacon.public_website << " │\n";
        }
        if (!beacon.fastping_service.empty()) {
            std::cout << "│ 🚀 FastPing: " << std::left << std::setw(43) << beacon.fastping_service << " │\n";
        }
        if (!beacon.company.empty()) {
            std::cout << "│ 🏢 Company: " << std::left << std::setw(44) << beacon.company << " │\n";
        }
        if (!beacon.showcase_message.empty()) {
            std::cout << "│ 💼 Message: " << std::left << std::setw(44) << beacon.showcase_message << " │\n";
        }
        
        std::cout << "└─────────────────────────────────────────────────────────────┘\n";
        
        // Performance analysis
        if (beacon.json_parse_time_microseconds < 0.1) {
            std::cout << "🚀 INCREDIBLE! Sub-0.1µs parsing - this is theoretical maximum performance!\n";
        } else if (beacon.json_parse_time_microseconds < 0.5) {
            std::cout << "⚡ AMAZING! Sub-0.5µs parsing - this is elite-tier performance!\n";
        } else if (beacon.json_parse_time_microseconds < 1.0) {
            std::cout << "🔥 EXCELLENT! Sub-1.0µs parsing - this is professional-grade performance!\n";
        } else {
            std::cout << "✅ GOOD! " << std::fixed << std::setprecision(2) << beacon.json_parse_time_microseconds << "µs parsing - solid performance!\n";
        }
        
        if (beacon.average_throughput_mbps > 300) {
            std::cout << "💎 PHENOMENAL THROUGHPUT! " << std::fixed << std::setprecision(1) << beacon.average_throughput_mbps << " MB/s - approaching memory bandwidth!\n";
        } else if (beacon.average_throughput_mbps > 200) {
            std::cout << "🚀 OUTSTANDING THROUGHPUT! " << std::fixed << std::setprecision(1) << beacon.average_throughput_mbps << " MB/s - this is world-class!\n";
        } else if (beacon.average_throughput_mbps > 100) {
            std::cout << "⚡ EXCELLENT THROUGHPUT! " << std::fixed << std::setprecision(1) << beacon.average_throughput_mbps << " MB/s - professional grade!\n";
        }
        
        std::cout << "\n";
    }
    
public:
    UltimateTcpListener(const std::string& ip, int port) : server_ip(ip), server_port(port) {
        std::cout << "🎧 Ultimate TCP Lighthouse Listener v3.0\n";
        std::cout << "==========================================\n";
        std::cout << "🚀 Enhanced with RTC's Jsonifier parsing\n";
        std::cout << "⚡ Ready for ultra-fast JSON data analysis\n";
        std::cout << "🏰 Professional beacon display and metrics\n\n";
    }
    
    bool connect_and_listen() {
        print_timestamp();
        std::cout << "📡 Connecting to Ultimate Lighthouse: " << server_ip << ":" << server_port << "\n";
        
        // Create TCP socket
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            std::cerr << "❌ Socket creation failed\n";
            return false;
        }
        
        // Connect to lighthouse server
        sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(server_port);
        
        if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) <= 0) {
            std::cerr << "❌ Invalid IP address: " << server_ip << "\n";
            close(sock);
            return false;
        }
        
        if (connect(sock, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
            std::cerr << "❌ Connection to lighthouse failed\n";
            std::cerr << "   Make sure Ultimate Lighthouse is running on " << server_ip << ":" << server_port << "\n";
            close(sock);
            return false;
        }
        
        print_timestamp();
        std::cout << "✅ Connected to Ultimate Public Lighthouse!\n";
        std::cout << "📡 Receiving ultra-fast JSON beacon data...\n";
        std::cout << "🚀 Powered by RTC's Jsonifier for maximum parsing performance\n\n";
        
        // Receive and display data
        std::string buffer;
        char chunk[4096];
        
        while (true) {
            int bytes_received = recv(sock, chunk, sizeof(chunk) - 1, 0);
            
            if (bytes_received <= 0) {
                print_timestamp();
                std::cout << "📡 Lighthouse connection closed\n";
                break;
            }
            
            chunk[bytes_received] = '\0';
            buffer += std::string(chunk, bytes_received);
            
            // Process complete JSON objects
            size_t json_start = 0;
            size_t json_end = buffer.find('\n');
            
            while (json_end != std::string::npos) {
                std::string json_line = buffer.substr(json_start, json_end - json_start);
                
                // Skip separator lines
                if (json_line.find("═") == std::string::npos && 
                    json_line.find("{") != std::string::npos) {
                    
                    process_json_beacon(json_line);
                }
                
                json_start = json_end + 1;
                json_end = buffer.find('\n', json_start);
            }
            
            // Keep remaining data for next iteration
            buffer = buffer.substr(json_start);
        }
        
        close(sock);
        return true;
    }
    
private:
    void process_json_beacon(const std::string& json_data) {
        auto parse_start = std::chrono::high_resolution_clock::now();
        
        try {
            UltimateBeaconPayload beacon;
            auto result = json_core.parseJson(beacon, json_data);
            
            auto parse_end = std::chrono::high_resolution_clock::now();
            auto parse_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(parse_end - parse_start);
            double listener_parse_time_us = parse_duration.count() / 1000.0;
            
            packet_count++;
            
            print_timestamp();
            std::cout << "🏰 LIGHTHOUSE BEACON #" << packet_count << " PARSED SUCCESSFULLY\n";
            std::cout << "📦 " << json_data.size() << " bytes | ";
            std::cout << "⚡ Listener parse: " << std::fixed << std::setprecision(3) << listener_parse_time_us << "µs | ";
            std::cout << "🚀 Lighthouse parse: " << std::fixed << std::setprecision(3) << beacon.json_parse_time_microseconds << "µs\n";
            
            // Compare parse times
            if (listener_parse_time_us < beacon.json_parse_time_microseconds) {
                std::cout << "🎯 Our listener parsed FASTER than the lighthouse! (Local vs Network optimized)\n";
            } else {
                double speedup = listener_parse_time_us / beacon.json_parse_time_microseconds;
                std::cout << "🏰 Lighthouse is " << std::fixed << std::setprecision(1) << speedup << "x faster than our listener!\n";
            }
            
            display_beacon_data(beacon);
            
            std::cout << "═══════════════════════════════════════════════════════════════\n";
            
        } catch (const std::exception& e) {
            packet_count++;
            print_timestamp();
            std::cout << "🚨 Failed to parse beacon #" << packet_count << "\n";
            std::cout << "📄 Parse Error: " << e.what() << "\n";
            std::cout << "📄 Raw JSON: " << json_data.substr(0, 200) << "...\n\n";
        }
    }
};

void display_help(const char* program_name) {
    std::cout << R"(
🎧 Ultimate TCP Lighthouse Listener v3.0
Usage: )" << program_name << R"( [SERVER_IP] [PORT]

Arguments:
   SERVER_IP    IP address of the Ultimate Lighthouse (default: 161.35.248.233)
   PORT         Port number (default: 9876)

Examples:
   )" << program_name << R"(                           # Connect to public lighthouse
   )" << program_name << R"( 161.35.248.233 9876      # Connect to specific IP:port
   )" << program_name << R"( localhost 9876           # Connect to local lighthouse

Features:
   🚀 Ultra-fast JSON parsing with RTC's Jsonifier
   🏰 Professional beacon display and analysis
   ⚡ Real-time performance comparison
   📊 Comprehensive metrics and statistics
   🌍 Connect to global lighthouse network

🎯 The Ultimate Network Monitoring Client! 🎯
)";
}

int main(int argc, char* argv[]) {
    try {
        std::string server_ip = "161.35.248.233";  // Default public lighthouse
        int server_port = 9876;
        
        // Parse command line arguments
        if (argc > 1) {
            if (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
                display_help(argv[0]);
                return 0;
            }
            server_ip = argv[1];
        }
        
        if (argc > 2) {
            server_port = std::stoi(argv[2]);
        }
        
        UltimateTcpListener listener(server_ip, server_port);
        
        if (!listener.connect_and_listen()) {
            std::cerr << "❌ Failed to connect to Ultimate Lighthouse\n";
            std::cerr << "💡 Try: " << argv[0] << " --help for usage information\n";
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "🚨 Fatal Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}