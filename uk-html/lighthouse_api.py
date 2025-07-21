#!/usr/bin/env python3
"""
Lighthouse API Bridge - Serves Real Data from Lighthouse Beacon
Connects to the lighthouse TCP stream and provides HTTP API for website
"""

import socket
import json
import time
import threading
from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs
import logging

# Configure logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)

class LighthouseDataCollector:
    def get_lighthouse_stats():
    try:
        with socket.create_connection((self, lighthouse_host="129.212.161.156", lighthouse_port=9876), timeout=1) as s:
            s.settimeout(2)
            # if your lighthouse expects input first, do something like:
            # s.sendall(b'PING\n')
            raw = s.recv(4096)
            if not raw:
                return {"error": "Empty response from lighthouse"}
            return json.loads(raw.decode())
    except socket.timeout:
        return {"error": "Lighthouse timed out"}
    except (ConnectionRefusedError, socket.error) as e:
        return {"error": f"Socket error: {e}"}
    except json.JSONDecodeError:
        return {"error": "Invalid JSON from lighthouse"}
        self.lighthouse_host = lighthouse_host
        self.lighthouse_port = lighthouse_port
        self.latest_data = None
        self.last_update = None
        self.connected = False
        self.running = False
        
    def connect_to_lighthouse(self):
        """Connect to lighthouse beacon and collect real data"""
        while self.running:
            try:
                logger.info(f"Connecting to lighthouse at {self.lighthouse_host}:{self.lighthouse_port}")
                
                with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
                    sock.settimeout(10)
                    sock.connect((self.lighthouse_host, self.lighthouse_port))
                    self.connected = True
                    logger.info("Connected to lighthouse beacon!")
                    
                    buffer = ""
                    
                    while self.running:
                        try:
                            data = sock.recv(4096).decode('utf-8')
                            if not data:
                                break
                                
                            buffer += data
                            
                            # Look for JSON blocks between separator lines
                            if '================================================================================' in buffer:
                                # Split by separator lines to find JSON blocks
                                parts = buffer.split('================================================================================')
                                
                                for part in parts:
                                    part = part.strip()
                                    if part and part.startswith('{'):
                                        try:
                                            # Try to parse as JSON
                                            beacon_data = json.loads(part)
                                            if 'lighthouse_id' in beacon_data:  # Validate it's lighthouse data
                                                self.latest_data = beacon_data
                                                self.last_update = time.time()
                                                logger.info(f"Updated data: Seq #{beacon_data.get('beacon_sequence_number', 'unknown')}")
                                                logger.info(f"Parse time: {beacon_data.get('json_parse_time_microseconds', 'unknown')}µs")
                                        except json.JSONDecodeError as e:
                                            logger.warning(f"Failed to parse JSON block: {e}")
                                
                                # Keep only the last part for next iteration
                                buffer = parts[-1] if parts else ""
                                        
                        except socket.timeout:
                            continue
                        except Exception as e:
                            logger.error(f"Error receiving data: {e}")
                            break
                            
            except Exception as e:
                logger.error(f"Connection error: {e}")
                self.connected = False
                
            if self.running:
                logger.info("Reconnecting in 10 seconds...")
                time.sleep(10)
                
    def start(self):
        """Start the data collection thread"""
        self.running = True
        self.thread = threading.Thread(target=self.connect_to_lighthouse, daemon=True)
        self.thread.start()
        
    def stop(self):
        """Stop data collection"""
        self.running = False
        self.connected = False
        
    def get_current_data(self):
        """Get the latest lighthouse data"""
        if not self.latest_data or not self.last_update:
            return None
            
        # Check if data is stale (older than 60 seconds)
        if time.time() - self.last_update > 60:
            return None
            
        return {
            **self.latest_data,
            'api_status': 'connected' if self.connected else 'disconnected',
            'last_update': self.last_update,
            'data_age_seconds': int(time.time() - self.last_update)
        }

class LighthouseAPIHandler(BaseHTTPRequestHandler):
    def __init__(self, *args, data_collector=None, **kwargs):
        self.data_collector = data_collector
        super().__init__(*args, **kwargs)
        
    def do_GET(self):
        """Handle GET requests"""
        parsed_path = urlparse(self.path)
        
        # Enable CORS for web browsers
        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET')
        self.send_header('Cache-Control', 'no-cache, no-store, must-revalidate')
        self.end_headers()
        
        if parsed_path.path == '/api/lighthouse':
            # Return current lighthouse data
            data = self.data_collector.get_current_data()
            
            if data:
                response = {
                    'status': 'success',
                    'data': data,
                    'timestamp': time.time()
                }
            else:
                response = {
                    'status': 'no_data',
                    'message': 'No recent lighthouse data available',
                    'timestamp': time.time(),
                    'connected': self.data_collector.connected
                }
                
        elif parsed_path.path == '/api/status':
            # Return API status
            response = {
                'status': 'online',
                'lighthouse_connected': self.data_collector.connected,
                'last_update': self.data_collector.last_update,
                'has_data': self.data_collector.latest_data is not None,
                'timestamp': time.time()
            }
            
        elif parsed_path.path == '/':
            # Simple status page
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.end_headers()
            
            status_html = f"""
            <!DOCTYPE html>
            <html>
            <head><title>Lighthouse API Bridge</title></head>
            <body>
                <h1>🏰 Lighthouse API Bridge</h1>
                <p><strong>Status:</strong> {"🟢 Connected" if self.data_collector.connected else "🔴 Disconnected"}</p>
                <p><strong>Last Update:</strong> {time.ctime(self.data_collector.last_update) if self.data_collector.last_update else "Never"}</p>
                <p><strong>Has Data:</strong> {"Yes" if self.data_collector.latest_data else "No"}</p>
                
                <h2>API Endpoints:</h2>
                <ul>
                    <li><a href="/api/lighthouse">/api/lighthouse</a> - Current lighthouse data</li>
                    <li><a href="/api/status">/api/status</a> - API status</li>
                </ul>
                
                <p><em>This API provides real-time data from the lighthouse beacon at {self.data_collector.lighthouse_host}:{self.data_collector.lighthouse_port}</em></p>
            </body>
            </html>
            """
            self.wfile.write(status_html.encode())
            return
            
        else:
            response = {
                'status': 'error',
                'message': 'Endpoint not found',
                'available_endpoints': ['/api/lighthouse', '/api/status']
            }
            
        self.wfile.write(json.dumps(response, indent=2).encode())
        
    def log_message(self, format, *args):
        """Custom logging to reduce noise"""
        pass  # Disable default request logging

def create_handler(data_collector):
    """Create a handler with the data collector injected"""
    def handler(*args, **kwargs):
        return LighthouseAPIHandler(*args, data_collector=data_collector, **kwargs)
    return handler

def main():
    # Configuration
    API_HOST = '0.0.0.0'  # Listen on all interfaces
    API_PORT = 8080
    LIGHTHOUSE_HOST = '129.212.161.156'  # Your lighthouse server
    LIGHTHOUSE_PORT = 9876
    
    print("🏰 Starting Lighthouse API Bridge...")
    print(f"📡 Will connect to lighthouse at {LIGHTHOUSE_HOST}:{LIGHTHOUSE_PORT}")
    print(f"🌐 API will be available at http://{API_HOST}:{API_PORT}")
    
    # Start data collector
    collector = LighthouseDataCollector(LIGHTHOUSE_HOST, LIGHTHOUSE_PORT)
    collector.start()
    
    # Start HTTP server
    handler = create_handler(collector)
    server = HTTPServer((API_HOST, API_PORT), handler)
    
    try:
        print(f"🚀 Lighthouse API Bridge running on port {API_PORT}")
        print("📊 Visit http://localhost:8080 to see status")
        print("🔗 API endpoint: http://localhost:8080/api/lighthouse")
        print("Press Ctrl+C to stop...")
        server.serve_forever()
    except KeyboardInterrupt:
        print("\n🛑 Stopping Lighthouse API Bridge...")
        collector.stop()
        server.shutdown()
        print("✅ Stopped!")

if __name__ == "__main__":
    main()