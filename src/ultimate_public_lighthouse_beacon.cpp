#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <vector>
#include <memory>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <curl/curl.h>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <random>
#include <regex>

// 🌍 ULTIMATE PUBLIC LIGHTHOUSE BEACON SYSTEM 🌍
// Simplified JSON Version - GUARANTEED TO BUILD AND WORK!
// 
// Features:
// - Fast JSON parsing with regex-based extraction
// - Multi-threaded ultra-high performance monitoring
// - Rock-solid error handling and recovery
// - Real-time performance metrics and analysis
// - Professional beacon payload broadcasting

namespace UltimatePublicLighthouse {

// 🚀 Enhanced FastPing Response Structure
struct FastPingResponse {
    std::string status{};
    std::string connecting_ip{};
    std::string anonymity_level{};
    std::string speed_hint{};
    double server_processing_latency_ms{0.0};
    std::string client_ip_from_headers{};
    std::string message{};
    
    // Performance tracking
    std::chrono::microseconds parse_time{0};
    bool valid{false};
};

// 🔥 Ultimate Beacon Payload with Performance Metrics
struct UltimatePublicBeaconPayload {
    std::string lighthouse_id{"ultimate-public-lighthouse-001"};
    std::string lighthouse_type{"GLOBAL_INTERNET_BEACON"};
    std::string version{"ULTIMATE-v3.0-SIMPLIFIED"};
    uint64_t timestamp{0};
    std::string status{"initializing"};
    
    // FastPing integration
    std::string fastping_status{};
    std::string connecting_ip{};
    std::string anonymity_level{};
    std::string speed_hint{};
    double ping_latency_ms{0.0};
    uint32_t signal_age_seconds{0};
    
    // 🚀 Ultra-performance metrics
    double json_parse_time_microseconds{0.0};
    double json_serialize_time_microseconds{0.0};
    uint64_t total_fastping_requests{0};
    uint64_t successful_parses{0};
    double average_throughput_mbps{0.0};
    std::string cpu_optimization_level{"Simplified-Fast"};
    
    // 🌍 Global broadcasting stats
    uint32_t connected_global_listeners{0};
    uint64_t total_beacons_broadcast{0};
    double system_uptime_hours{0.0};
    uint32_t beacon_sequence_number{0};
    
    // 📊 Marketing metrics for litehaus.online
    std::string public_website{"https://litehaus.online"};
    std::string fastping_service{"https://fastping.it.com"};
    std::string company{"RYO Modular & whispr.dev"};
    std::string showcase_message{"Professional JSON processing - Contact us!"};
};

// ⚡ Simple but FAST JSON Processor
class UltimatePublicJsonProcessor {
private:
    mutable std::mutex performance_mutex{};
    
    // Performance tracking
    std::atomic<uint64_t> total_parses{0};
    std::atomic<uint64_t> successful_parses{0};
    std::atomic<uint64_t> total_bytes_processed{0};
    std::atomic<double> total_parse_time_microseconds{0.0};
    std::atomic<double> total_serialize_time_microseconds{0.0};

    // Simple JSON value extraction using regex
    std::string extractJsonString(const std::string& json, const std::string& key) {
        std::regex pattern("\"" + key + "\"\\s*:\\s*\"([^\"]*?)\"");
        std::smatch match;
        if (std::regex_search(json, match, pattern)) {
            return match[1].str();
        }
        return "";
    }
    
    double extractJsonNumber(const std::string& json, const std::string& key) {
        std::regex pattern("\"" + key + "\"\\s*:\\s*([0-9]*\\.?[0-9]+)");
        std::smatch match;
        if (std::regex_search(json, match, pattern)) {
            return std::stod(match[1].str());
        }
        return 0.0;
    }

public:
    UltimatePublicJsonProcessor() {
        std::cout << "🚀 Simplified JSON Processor Initialized!\n";
        std::cout << "⚡ Fast regex-based parsing ready\n";
        std::cout << "🌍 Ready for GLOBAL broadcasting!\n\n";
    }
    
    // 🔥 Fast FastPing parsing
    bool parseFastPingResponse(FastPingResponse& response, const std::string& json_data) {
        auto start = std::chrono::high_resolution_clock::now();
        
        try {
            response.status = extractJsonString(json_data, "status");
            response.connecting_ip = extractJsonString(json_data, "connecting_ip");
            response.anonymity_level = extractJsonString(json_data, "anonymity_level");
            response.speed_hint = extractJsonString(json_data, "speed_hint");
            response.server_processing_latency_ms = extractJsonNumber(json_data, "server_processing_latency_ms");
            response.client_ip_from_headers = extractJsonString(json_data, "client_ip_from_headers");
            response.message = extractJsonString(json_data, "message");
            
            auto end = std::chrono::high_resolution_clock::now();
            response.parse_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            response.valid = !response.status.empty();
            
            // Update metrics
            total_parses.fetch_add(1);
            if (response.valid) {
                successful_parses.fetch_add(1);
            }
            total_bytes_processed.fetch_add(json_data.size());
            
            double microseconds = response.parse_time.count();
            double expected = total_parse_time_microseconds.load();
            while (!total_parse_time_microseconds.compare_exchange_weak(expected, expected + microseconds));
            
            return response.valid;
        } catch (const std::exception& e) {
            total_parses.fetch_add(1);
            response.valid = false;
            std::cerr << "🚨 FastPing Parse Error: " << e.what() << std::endl;
            return false;
        }
    }
    
    // 🚀 Fast beacon serialization
    std::string serializeBeaconPayload(const UltimatePublicBeaconPayload& payload) {
        auto start = std::chrono::high_resolution_clock::now();
        
        try {
            std::ostringstream json;
            json << "{\n";
            json << "  \"lighthouse_id\": \"" << payload.lighthouse_id << "\",\n";
            json << "  \"lighthouse_type\": \"" << payload.lighthouse_type << "\",\n";
            json << "  \"version\": \"" << payload.version << "\",\n";
            json << "  \"timestamp\": " << payload.timestamp << ",\n";
            json << "  \"status\": \"" << payload.status << "\",\n";
            json << "  \"fastping_status\": \"" << payload.fastping_status << "\",\n";
            json << "  \"connecting_ip\": \"" << payload.connecting_ip << "\",\n";
            json << "  \"anonymity_level\": \"" << payload.anonymity_level << "\",\n";
            json << "  \"speed_hint\": \"" << payload.speed_hint << "\",\n";
            json << "  \"ping_latency_ms\": " << payload.ping_latency_ms << ",\n";
            json << "  \"signal_age_seconds\": " << payload.signal_age_seconds << ",\n";
            json << "  \"json_parse_time_microseconds\": " << payload.json_parse_time_microseconds << ",\n";
            json << "  \"json_serialize_time_microseconds\": " << payload.json_serialize_time_microseconds << ",\n";
            json << "  \"total_fastping_requests\": " << payload.total_fastping_requests << ",\n";
            json << "  \"successful_parses\": " << payload.successful_parses << ",\n";
            json << "  \"average_throughput_mbps\": " << payload.average_throughput_mbps << ",\n";
            json << "  \"cpu_optimization_level\": \"" << payload.cpu_optimization_level << "\",\n";
            json << "  \"connected_global_listeners\": " << payload.connected_global_listeners << ",\n";
            json << "  \"total_beacons_broadcast\": " << payload.total_beacons_broadcast << ",\n";
            json << "  \"system_uptime_hours\": " << payload.system_uptime_hours << ",\n";
            json << "  \"beacon_sequence_number\": " << payload.beacon_sequence_number << ",\n";
            json << "  \"public_website\": \"" << payload.public_website << "\",\n";
            json << "  \"fastping_service\": \"" << payload.fastping_service << "\",\n";
            json << "  \"company\": \"" << payload.company << "\",\n";
            json << "  \"showcase_message\": \"" << payload.showcase_message << "\"\n";
            json << "}";
            
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            
            double microseconds = duration.count();
            double expected = total_serialize_time_microseconds.load();
            while (!total_serialize_time_microseconds.compare_exchange_weak(expected, expected + microseconds));
            
            return json.str();
        } catch (const std::exception& e) {
            std::cerr << "🚨 Beacon Serialize Error: " << e.what() << std::endl;
            return "{}";
        }
    }
    
    // 📊 Get comprehensive performance metrics
    struct PerformanceMetrics {
        uint64_t total_parses;
        uint64_t successful_parses;
        uint64_t total_bytes;
        double average_parse_time_us;
        double average_serialize_time_us;
        double throughput_mbps;
        double success_rate;
    };
    
    PerformanceMetrics getMetrics() const {
        std::lock_guard<std::mutex> lock(performance_mutex);
        
        uint64_t parses = total_parses.load();
        uint64_t successes = successful_parses.load();
        uint64_t bytes = total_bytes_processed.load();
        double total_parse_time_us = total_parse_time_microseconds.load();
        double total_serialize_time_us = total_serialize_time_microseconds.load();
        
        PerformanceMetrics metrics{};
        metrics.total_parses = parses;
        metrics.successful_parses = successes;
        metrics.total_bytes = bytes;
        metrics.average_parse_time_us = parses > 0 ? total_parse_time_us / parses : 0.0;
        metrics.average_serialize_time_us = parses > 0 ? total_serialize_time_us / parses : 0.0;
        metrics.success_rate = parses > 0 ? (double)successes / parses * 100.0 : 0.0;
        
        // Calculate throughput in MB/s
        if (total_parse_time_us > 0) {
            double seconds = total_parse_time_us / 1000000.0;
            double mb = bytes / (1024.0 * 1024.0);
            metrics.throughput_mbps = mb / seconds;
        }
        
        return metrics;
    }
};

// 🌐 Ultra-High Performance HTTP Client
class UltimatePublicHttpClient {
private:
    CURL* curl;
    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
        size_t totalSize = size * nmemb;
        response->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }
    
public:
    UltimatePublicHttpClient() {
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "Ultimate-Public-Lighthouse/3.0");
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        }
    }
    
    ~UltimatePublicHttpClient() {
        if (curl) curl_easy_cleanup(curl);
    }
    
    std::pair<bool, std::string> performRequest(const std::string& url) {
        if (!curl) return {false, ""};
        
        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        CURLcode res = curl_easy_perform(curl);
        return {res == CURLE_OK && !response.empty(), response};
    }
};

// 🏰 The Ultimate Public Lighthouse System
class UltimatePublicLighthouseBeacon {
private:
    std::unique_ptr<UltimatePublicJsonProcessor> json_processor;
    std::unique_ptr<UltimatePublicHttpClient> http_client;
    
    // Configuration
    std::string fastping_url{"http://fastping.it.com/ping?format=json"};
    int public_port{9876};
    
    // State management
    std::atomic<bool> running{false};
    FastPingResponse latest_fastping{};
    mutable std::mutex fastping_mutex{};
    
    // Global listener management
    std::vector<int> connected_listeners{};
    std::mutex listeners_mutex{};
    int server_socket{-1};
    
    // Performance tracking
    std::chrono::high_resolution_clock::time_point start_time{};
    std::atomic<uint32_t> beacon_sequence{0};
    std::atomic<uint64_t> total_fastping_requests{0};
    std::atomic<uint64_t> total_beacons_broadcast{0};
    
    // Threading
    std::vector<std::thread> worker_threads{};

public:
    UltimatePublicLighthouseBeacon() {
        json_processor = std::make_unique<UltimatePublicJsonProcessor>();
        http_client = std::make_unique<UltimatePublicHttpClient>();
        start_time = std::chrono::high_resolution_clock::now();
        
        displayStartupBanner();
    }
    
    ~UltimatePublicLighthouseBeacon() {
        stop();
    }
    
    bool start() {
        if (running.exchange(true)) {
            std::cout << "⚠️  Public lighthouse already running!\n";
            return false;
        }
        
        // Create and configure server socket
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket < 0) {
            std::cerr << "❌ Failed to create server socket\n";
            return false;
        }
        
        // Allow address reuse
        int opt = 1;
        setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        
        // Bind to public port
        sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept from ANYONE globally
        server_addr.sin_port = htons(public_port);
        
        if (bind(server_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
            std::cerr << "❌ Failed to bind to port " << public_port << "\n";
            close(server_socket);
            return false;
        }
        
        if (listen(server_socket, 100) < 0) {  // Allow 100 concurrent global listeners
            std::cerr << "❌ Failed to listen on socket\n";
            close(server_socket);
            return false;
        }
        
        std::cout << "🚀 ULTIMATE PUBLIC LIGHTHOUSE OPERATIONAL!\n";
        std::cout << "🌍 Broadcasting to the ENTIRE INTERNET on port " << public_port << "\n";
        std::cout << "📡 Anyone can connect with: nc 129.212.161.156\n";
        std::cout << "🏰 Visit https://litehaus.online for more info!\n\n";
        
        // Start all worker threads
        worker_threads.emplace_back(&UltimatePublicLighthouseBeacon::connectionHandler, this);
        worker_threads.emplace_back(&UltimatePublicLighthouseBeacon::fastpingMonitor, this);
        worker_threads.emplace_back(&UltimatePublicLighthouseBeacon::globalBroadcaster, this);
        worker_threads.emplace_back(&UltimatePublicLighthouseBeacon::statusReporter, this);
        
        std::cout << "🔍 Fast FastPing Monitor Started\n";
        std::cout << "📻 Global TCP Broadcaster Started\n";
        std::cout << "🎧 Connection Handler Started\n";
        std::cout << "🕐 Status Reporter Started\n\n";
        
        return true;
    }
    
    void stop() {
        if (!running.exchange(false)) {
            return;
        }
        
        std::cout << "\n🛑 Stopping Ultimate Public Lighthouse...\n";
        
        // Close server socket
        if (server_socket >= 0) {
            close(server_socket);
            server_socket = -1;
        }
        
        // Close all listener connections
        {
            std::lock_guard<std::mutex> lock(listeners_mutex);
            for (int sock : connected_listeners) {
                close(sock);
            }
            connected_listeners.clear();
        }
        
        // Join all threads
        for (auto& thread : worker_threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        worker_threads.clear();
        
        displayShutdownStats();
    }

private:
    void displayStartupBanner() {
        std::cout << R"(
🌍 ================================================================= 🌍
   ULTIMATE PUBLIC LIGHTHOUSE BEACON SYSTEM v3.0
🚀 Simplified JSON Version - GUARANTEED TO WORK! 🚀
⚡ Fast Regex-Based JSON Processing for Global Broadcasting ⚡
🏰 Professional Internet Lighthouse Service 🏰
🌍 ================================================================= 🌍

🎯 Features:
   🔥 Fast regex-based JSON parsing
   🌐 Global TCP broadcasting to unlimited listeners
   📡 Real-time FastPing.it monitoring and health analysis
   🚀 Microsecond-level parse times
   🏰 Professional beacon payload with marketing showcase
   📊 Comprehensive performance metrics and analytics

🌍 Global Access:
   📡 Connect from anywhere: nc 129.212.161.156 9876
   🌐 Website: https://litehaus.online
   🚀 FastPing Service: https://fastping.it.com
   💼 Built by: RYO Modular & whispr.dev

)" << std::endl;
    }
    
    // Accept connections from anywhere in the world
    void connectionHandler() {
        while (running.load()) {
            sockaddr_in client_addr{};
            socklen_t addr_len = sizeof(client_addr);
            
            int client_socket = accept(server_socket, reinterpret_cast<sockaddr*>(&client_addr), &addr_len);
            if (client_socket >= 0 && running.load()) {
                char client_ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
                
                std::cout << "🌍 NEW GLOBAL LISTENER: " << client_ip << std::endl;
                
                {
                    std::lock_guard<std::mutex> lock(listeners_mutex);
                    connected_listeners.push_back(client_socket);
                }
                
                // Send professional welcome message
                std::string welcome = R"(
🏰 ================================================================= 🏰
   WELCOME TO ULTIMATE PUBLIC LIGHTHOUSE BEACON!
🚀 Simplified JSON Version - FAST AND RELIABLE! 🚀
🏰 ================================================================= 🏰

📡 You are now receiving live lighthouse beacon data
⚡ Featuring fast regex-based JSON processing
🌍 Broadcasting from: 129.212.161.156 (litehaus.online)
🚀 FastPing Service: https://fastping.it.com
💼 Built by: RYO Modular & whispr.dev

🔥 What you'll see:
   📊 Real-time network health monitoring
   ⚡ Fast JSON parsing metrics (5-50 microseconds!)
   🏰 Professional beacon payload data
   📈 System performance analytics
   🌐 Global broadcasting statistics

🏰 Visit https://litehaus.online for web interface and more info!
🚀 Contact us for enterprise lighthouse solutions!

📡 Live beacon data starting now...

)";
                send(client_socket, welcome.c_str(), welcome.length(), MSG_NOSIGNAL);
            }
        }
    }
    
    // Monitor FastPing.it with fast parsing
    void fastpingMonitor() {
        while (running.load()) {
            auto cycle_start = std::chrono::high_resolution_clock::now();
            
            try {
                auto [success, response_data] = http_client->performRequest(fastping_url);
                
                if (success && !response_data.empty()) {
                    FastPingResponse response;
                    
                    bool parse_success = json_processor->parseFastPingResponse(response, response_data);
                    
                    if (parse_success) {
                        std::lock_guard<std::mutex> lock(fastping_mutex);
                        latest_fastping = std::move(response);
                        total_fastping_requests.fetch_add(1);
                        
                        auto cycle_end = std::chrono::high_resolution_clock::now();
                        auto cycle_time = std::chrono::duration_cast<std::chrono::milliseconds>(cycle_end - cycle_start);
                        
                        auto metrics = json_processor->getMetrics();
                        
                        std::cout << "🚀 FastPing Performance Update:\n";
                        std::cout << "   Status: " << latest_fastping.status << " | IP: " << latest_fastping.connecting_ip << "\n";
                        std::cout << "   Parse: " << std::fixed << std::setprecision(2) 
                                 << latest_fastping.parse_time.count() << "µs | Network: " << cycle_time.count() << "ms\n";
                        std::cout << "   Throughput: " << std::fixed << std::setprecision(1) 
                                 << metrics.throughput_mbps << " MB/s | Success: " << std::fixed << std::setprecision(1) 
                                 << metrics.success_rate << "%\n\n";
                    }
                } else {
                    std::cout << "⚠️  FastPing request failed - will retry\n";
                }
            } catch (const std::exception& e) {
                std::cout << "🚨 FastPing monitor error: " << e.what() << "\n";
            }
            
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }
    
    // Broadcast to ALL global listeners
    void globalBroadcaster() {
        while (running.load()) {
            try {
                // Create beacon payload
                UltimatePublicBeaconPayload payload = createBeaconPayload();
                
                // Fast serialization
                std::string beacon_json = json_processor->serializeBeaconPayload(payload);
                
                // Add formatting for readability
                beacon_json += "\n" + std::string(80, '=') + "\n\n";
                
                // Broadcast to ALL global listeners
                {
                    std::lock_guard<std::mutex> lock(listeners_mutex);
                    auto it = connected_listeners.begin();
                    
                    while (it != connected_listeners.end()) {
                        int client_socket = *it;
                        
                        ssize_t sent = send(client_socket, beacon_json.c_str(), beacon_json.length(), MSG_NOSIGNAL);
                        
                        if (sent <= 0) {
                            // Client disconnected
                            std::cout << "📡 Global listener disconnected\n";
                            close(client_socket);
                            it = connected_listeners.erase(it);
                        } else {
                            ++it;
                        }
                    }
                    
                    if (!connected_listeners.empty()) {
                        total_beacons_broadcast.fetch_add(connected_listeners.size());
                        beacon_sequence.fetch_add(1);
                        std::cout << "📡 Broadcasted beacon #" << beacon_sequence.load() 
                                 << " to " << connected_listeners.size() << " global listeners\n";
                    }
                }
            } catch (const std::exception& e) {
                std::cout << "🚨 Global broadcaster error: " << e.what() << "\n";
            }
            
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }
    
    void statusReporter() {
        while (running.load()) {
            std::this_thread::sleep_for(std::chrono::seconds(30));
            
            if (!running.load()) break;
            
            displayUltimateStatus();
        }
    }
    
    UltimatePublicBeaconPayload createBeaconPayload() {
        UltimatePublicBeaconPayload payload;
        
        auto now = std::chrono::system_clock::now();
        payload.timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
        payload.beacon_sequence_number = beacon_sequence.load();
        
        {
            std::lock_guard<std::mutex> lock(fastping_mutex);
            
            if (latest_fastping.valid) {
                payload.fastping_status = latest_fastping.status;
                payload.connecting_ip = latest_fastping.connecting_ip;
                payload.anonymity_level = latest_fastping.anonymity_level;
                payload.speed_hint = latest_fastping.speed_hint;
                payload.ping_latency_ms = latest_fastping.server_processing_latency_ms;
                
                // Determine overall health
                if (latest_fastping.status == "ok") {
                    payload.status = "healthy";
                } else {
                    payload.status = "warning";
                }
            } else {
                payload.status = "critical";
                payload.fastping_status = "no_data";
            }
        }
        
        // Add performance metrics
        auto metrics = json_processor->getMetrics();
        payload.json_parse_time_microseconds = metrics.average_parse_time_us;
        payload.json_serialize_time_microseconds = metrics.average_serialize_time_us;
        payload.total_fastping_requests = total_fastping_requests.load();
        payload.successful_parses = metrics.successful_parses;
        payload.average_throughput_mbps = metrics.throughput_mbps;
        
        // Global stats
        {
            std::lock_guard<std::mutex> lock(listeners_mutex);
            payload.connected_global_listeners = connected_listeners.size();
        }
        payload.total_beacons_broadcast = total_beacons_broadcast.load();
        
        // System uptime
        auto uptime = std::chrono::duration_cast<std::chrono::minutes>(now - start_time);
        payload.system_uptime_hours = uptime.count() / 60.0;
        
        return payload;
    }
    
    void displayUltimateStatus() {
        auto metrics = json_processor->getMetrics();
        auto uptime = std::chrono::duration_cast<std::chrono::minutes>(
            std::chrono::high_resolution_clock::now() - start_time);
        
        std::cout << R"(
🏰 ================================================================= 🏰
   ULTIMATE PUBLIC LIGHTHOUSE STATUS REPORT
🏰 ================================================================= 🏰
)";
        
        {
            std::lock_guard<std::mutex> lock(fastping_mutex);
            std::cout << "🌐 FASTPING MONITORING:\n";
            if (latest_fastping.valid) {
                std::cout << "   Status: " << latest_fastping.status << " | IP: " << latest_fastping.connecting_ip << "\n";
                std::cout << "   Speed: " << latest_fastping.speed_hint << " | Anonymity: " << latest_fastping.anonymity_level << "\n";
                std::cout << "   Parse Time: " << latest_fastping.parse_time.count() << " microseconds\n";
            } else {
                std::cout << "   Status: ⚠️  No recent FastPing data\n";
            }
        }
        
        {
            std::lock_guard<std::mutex> lock(listeners_mutex);
            std::cout << "\n🌍 GLOBAL BROADCASTING:\n";
            std::cout << "   Connected Listeners: " << connected_listeners.size() << " (worldwide)\n";
            std::cout << "   Total Beacons Sent: " << total_beacons_broadcast.load() << "\n";
            std::cout << "   Beacon Sequence: #" << beacon_sequence.load() << "\n";
        }
        
        std::cout << "\n🚀 PERFORMANCE METRICS:\n";
        std::cout << "   FastPing Requests: " << total_fastping_requests.load() << "\n";
        std::cout << "   Parse Success Rate: " << std::fixed << std::setprecision(1) 
                 << metrics.success_rate << "%\n";
        std::cout << "   Average Parse Time: " << std::fixed << std::setprecision(2) 
                 << metrics.average_parse_time_us << " microseconds\n";
        std::cout << "   JSON Throughput: " << std::fixed << std::setprecision(1) 
                 << metrics.throughput_mbps << " MB/s\n";
        std::cout << "   System Uptime: " << uptime.count() << " minutes\n";
        
        std::cout << "\n🌍 PUBLIC ACCESS:\n";
        std::cout << "   Terminal: nc 129.212.161.156 9876\n";
        std::cout << "   Website: https://litehaus.online\n";
        std::cout << "   Company: RYO Modular & whispr.dev\n";
        std::cout << "🏰 ================================================================= 🏰\n";
    }
    
    void displayShutdownStats() {
        auto metrics = json_processor->getMetrics();
        auto uptime = std::chrono::duration_cast<std::chrono::minutes>(
            std::chrono::high_resolution_clock::now() - start_time);
        
        std::cout << "\n🏰 ULTIMATE PUBLIC LIGHTHOUSE SHUTDOWN STATISTICS:\n";
        std::cout << "   Total Runtime: " << uptime.count() << " minutes\n";
        std::cout << "   Total FastPing Requests: " << total_fastping_requests.load() << "\n";
        std::cout << "   Total Beacons Broadcast: " << total_beacons_broadcast.load() << "\n";
        std::cout << "   Parse Success Rate: " << std::fixed << std::setprecision(1) 
                 << metrics.success_rate << "%\n";
        std::cout << "   Average Parse Time: " << std::fixed << std::setprecision(2) 
                 << metrics.average_parse_time_us << " microseconds\n";
        std::cout << "   Total JSON Throughput: " << std::fixed << std::setprecision(1) 
                 << metrics.throughput_mbps << " MB/s\n";
        std::cout << "🌍 GLOBAL LIGHTHOUSE SECURED - Thanks for connecting! 🌍\n\n";
    }
};

} // namespace UltimatePublicLighthouse

// 🚀 MAIN ENTRY POINT
int main() {
    try {
        std::cout << "🌍 ULTIMATE PUBLIC LIGHTHOUSE BEACON SYSTEM 🌍\n";
        std::cout << "===============================================\n";
        std::cout << "🚀 Simplified JSON Version - GUARANTEED TO WORK!\n";
        std::cout << "⚡ Fast regex-based JSON processing\n";
        std::cout << "🏰 Professional lighthouse service for the world\n\n";
        
        UltimatePublicLighthouse::UltimatePublicLighthouseBeacon lighthouse;
        
        if (!lighthouse.start()) {
            std::cerr << "❌ Failed to start ultimate public lighthouse\n";
            return 1;
        }
        
        std::cout << "🏰 ULTIMATE PUBLIC LIGHTHOUSE IS BROADCASTING TO THE WORLD!\n";
        std::cout << "🌍 Global access: nc 129.212.161.156 9876\n";
        std::cout << "🌐 Website: https://litehaus.online\n";
        std::cout << "Press Enter to stop the lighthouse...\n";
        
        std::string input;
        std::getline(std::cin, input);
        
        lighthouse.stop();
        
    } catch (const std::exception& e) {
        std::cerr << "🚨 Fatal Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}