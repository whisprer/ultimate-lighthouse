#!/usr/bin/env node

const express = require('express');
const http = require('http');
const WebSocket = require('ws');
const cors = require('cors');
const helmet = require('helmet');
const compression = require('compression');
const morgan = require('morgan');
const path = require('path');
const { spawn, exec } = require('child_process');
const dgram = require('dgram');
const cron = require('node-cron');
require('dotenv').config();

// 🏰 ULTIMATE LIGHTHOUSE WEB SERVER CONFIGURATION 🏰
const CONFIG = {
    PORT: process.env.PORT || 3000,
    WS_PORT: process.env.WS_PORT || 3001,
    LIGHTHOUSE_EXECUTABLE: process.env.LIGHTHOUSE_EXECUTABLE || './litehaus',
    BEACON_IP: process.env.BEACON_IP || '138.68.142.181',
    BEACON_PORT: process.env.BEACON_PORT || 9876,
    LISTENER_PORT: process.env.LISTENER_PORT || 9876,
    UPDATE_INTERVAL: process.env.UPDATE_INTERVAL || 5000,
    LOG_LEVEL: process.env.LOG_LEVEL || 'info'
};

console.log(`
🏰 ═══════════════════════════════════════════════════════════════════ 🏰
   ULTIMATE LIGHTHOUSE WEB SERVER v3.0
🚀 Powered by Node.js + Express + WebSockets 🚀
⚡ Real-time SimdJSON Performance Monitoring ⚡
🔥 Ultra-Fast C++ Backend Integration 🔥
🏰 ═══════════════════════════════════════════════════════════════════ 🏰
`);

class UltimateLighthouseWebServer {
    constructor() {
        this.app = express();
        this.server = http.createServer(this.app);
        this.wss = new WebSocket.Server({ port: CONFIG.WS_PORT });
        
        this.lighthouseProcess = null;
        this.listenerProcess = null;
        this.udpListener = null;
        
        this.clients = new Set();
        this.systemStats = {
            uptime: Date.now(),
            totalRequests: 0,
            activeConnections: 0,
            lighthouseStatus: 'initializing',
            lastBeacon: null,
            performance: {
                avgParseTime: 0,
                throughput: 0,
                successRate: 100
            }
        };
        
        this.setupMiddleware();
        this.setupRoutes();
        this.setupWebSocket();
        this.setupUDPListener();
        
        console.log('🎯 Web Server Architecture Initialized');
        console.log(`📡 HTTP Server: http://localhost:${CONFIG.PORT}`);
        console.log(`🔌 WebSocket Server: ws://localhost:${CONFIG.WS_PORT}`);
        console.log(`🎧 UDP Listener: port ${CONFIG.LISTENER_PORT}`);
    }
    
    setupMiddleware() {
        // Security and performance middleware
        this.app.use(helmet({
            contentSecurityPolicy: {
                directives: {
                    defaultSrc: ["'self'"],
                    scriptSrc: ["'self'", "'unsafe-inline'"],
                    styleSrc: ["'self'", "'unsafe-inline'"],
                    connectSrc: ["'self'", `ws://localhost:${CONFIG.WS_PORT}`]
                }
            }
        }));
        
        this.app.use(compression());
        this.app.use(cors());
        this.app.use(morgan('combined'));
        this.app.use(express.json({ limit: '10mb' }));
        this.app.use(express.urlencoded({ extended: true }));
        this.app.use(express.static(path.join(__dirname, 'public')));
        
        // Request counter middleware
        this.app.use((req, res, next) => {
            this.systemStats.totalRequests++;
            next();
        });
    }
    
    setupRoutes() {
        // Main dashboard route
        this.app.get('/', (req, res) => {
            res.send(this.generateDashboardHTML());
        });
        
        // API Routes
        this.app.get('/api/status', (req, res) => {
            res.json({
                ...this.systemStats,
                config: CONFIG,
                timestamp: Date.now()
            });
        });
        
        this.app.post('/api/lighthouse/start', (req, res) => {
            try {
                this.startLighthouse();
                res.json({ success: true, message: 'Lighthouse started' });
            } catch (error) {
                res.status(500).json({ success: false, error: error.message });
            }
        });
        
        this.app.post('/api/lighthouse/stop', (req, res) => {
            try {
                this.stopLighthouse();
                res.json({ success: true, message: 'Lighthouse stopped' });
            } catch (error) {
                res.status(500).json({ success: false, error: error.message });
            }
        });
        
        this.app.post('/api/listener/start', (req, res) => {
            try {
                this.startListener();
                res.json({ success: true, message: 'Listener started' });
            } catch (error) {
                res.status(500).json({ success: false, error: error.message });
            }
        });
        
        this.app.post('/api/listener/stop', (req, res) => {
            try {
                this.stopListener();
                res.json({ success: true, message: 'Listener stopped' });
            } catch (error) {
                res.status(500).json({ success: false, error: error.message });
            }
        });
        
        // Real-time metrics endpoint
        this.app.get('/api/metrics', (req, res) => {
            res.json({
                performance: this.systemStats.performance,
                uptime: Date.now() - this.systemStats.uptime,
                memory: process.memoryUsage(),
                connections: this.clients.size
            });
        });
        
        // Health check
        this.app.get('/health', (req, res) => {
            res.json({ 
                status: 'healthy', 
                timestamp: Date.now(),
                lighthouse: this.systemStats.lighthouseStatus
            });
        });
    }
    
    setupWebSocket() {
        this.wss.on('connection', (ws) => {
            this.clients.add(ws);
            this.systemStats.activeConnections = this.clients.size;
            
            console.log(`🔌 New WebSocket connection (${this.clients.size} total)`);
            
            // Send initial status
            ws.send(JSON.stringify({
                type: 'status',
                data: this.systemStats
            }));
            
            ws.on('close', () => {
                this.clients.delete(ws);
                this.systemStats.activeConnections = this.clients.size;
                console.log(`🔌 WebSocket disconnected (${this.clients.size} remaining)`);
            });
            
            ws.on('message', (message) => {
                try {
                    const data = JSON.parse(message);
                    this.handleWebSocketMessage(ws, data);
                } catch (error) {
                    console.error('🚨 WebSocket message error:', error);
                }
            });
        });
        
        console.log('🔌 WebSocket Server Ready');
    }
    
    setupUDPListener() {
        this.udpListener = dgram.createSocket('udp4');
        
        this.udpListener.on('message', (msg, rinfo) => {
            try {
                const beaconData = JSON.parse(msg.toString());
                this.handleBeaconData(beaconData, rinfo);
            } catch (error) {
                console.error('🚨 UDP message parse error:', error);
            }
        });
        
        this.udpListener.on('listening', () => {
            const address = this.udpListener.address();
            console.log(`🎧 UDP Listener active on ${address.address}:${address.port}`);
        });
        
        this.udpListener.bind(CONFIG.LISTENER_PORT);
    }
    
    handleBeaconData(beaconData, rinfo) {
        this.systemStats.lastBeacon = {
            ...beaconData,
            receivedFrom: rinfo.address,
            receivedAt: Date.now()
        };
        
        // Update performance metrics
        if (beaconData.json_parse_time_microseconds) {
            this.systemStats.performance.avgParseTime = beaconData.json_parse_time_microseconds;
        }
        if (beaconData.average_throughput_mbps) {
            this.systemStats.performance.throughput = beaconData.average_throughput_mbps;
        }
        
        // Calculate success rate
        if (beaconData.successful_parses && beaconData.total_requests_processed) {
            this.systemStats.performance.successRate = 
                (beaconData.successful_parses / beaconData.total_requests_processed) * 100;
        }
        
        // Update lighthouse status
        this.systemStats.lighthouseStatus = beaconData.status || 'unknown';
        
        // Broadcast to all WebSocket clients
        this.broadcast({
            type: 'beacon',
            data: this.systemStats.lastBeacon
        });
        
        console.log(`📡 Beacon received from ${rinfo.address}: ${beaconData.status} (${beaconData.beacon_sequence_number || 'N/A'})`);
    }
    
    handleWebSocketMessage(ws, message) {
        switch (message.type) {
            case 'requestStatus':
                ws.send(JSON.stringify({
                    type: 'status',
                    data: this.systemStats
                }));
                break;
                
            case 'startLighthouse':
                this.startLighthouse();
                break;
                
            case 'stopLighthouse':
                this.stopLighthouse();
                break;
                
            case 'startListener':
                this.startListener();
                break;
                
            case 'stopListener':
                this.stopListener();
                break;
        }
    }
    
    startLighthouse() {
        if (this.lighthouseProcess) {
            console.log('⚠️  Lighthouse already running');
            return;
        }
        
        console.log('🚀 Starting Ultimate Lighthouse...');
        
        this.lighthouseProcess = spawn(CONFIG.LIGHTHOUSE_EXECUTABLE, [], {
            stdio: ['pipe', 'pipe', 'pipe']
        });
        
        this.lighthouseProcess.stdout.on('data', (data) => {
            const output = data.toString();
            console.log('🏰 Lighthouse:', output.trim());
            
            this.broadcast({
                type: 'lighthouse_output',
                data: output
            });
        });
        
        this.lighthouseProcess.stderr.on('data', (data) => {
            console.error('🚨 Lighthouse Error:', data.toString());
        });
        
        this.lighthouseProcess.on('close', (code) => {
            console.log(`🏰 Lighthouse process exited with code ${code}`);
            this.lighthouseProcess = null;
            this.systemStats.lighthouseStatus = 'stopped';
            
            this.broadcast({
                type: 'lighthouse_stopped',
                data: { code }
            });
        });
        
        this.systemStats.lighthouseStatus = 'running';
    }
    
    stopLighthouse() {
        if (this.lighthouseProcess) {
            console.log('🛑 Stopping Lighthouse...');
            this.lighthouseProcess.kill('SIGTERM');
            this.lighthouseProcess = null;
            this.systemStats.lighthouseStatus = 'stopped';
        }
    }
    
    startListener() {
        if (this.listenerProcess) {
            console.log('⚠️  Listener already running');
            return;
        }
        
        console.log('🎧 Starting Beacon Listener...');
        
        this.listenerProcess = spawn(CONFIG.LIGHTHOUSE_EXECUTABLE, ['--listener', CONFIG.LISTENER_PORT], {
            stdio: ['pipe', 'pipe', 'pipe']
        });
        
        this.listenerProcess.stdout.on('data', (data) => {
            const output = data.toString();
            console.log('🎧 Listener:', output.trim());
            
            this.broadcast({
                type: 'listener_output',
                data: output
            });
        });
        
        this.listenerProcess.stderr.on('data', (data) => {
            console.error('🚨 Listener Error:', data.toString());
        });
        
        this.listenerProcess.on('close', (code) => {
            console.log(`🎧 Listener process exited with code ${code}`);
            this.listenerProcess = null;
            
            this.broadcast({
                type: 'listener_stopped',
                data: { code }
            });
        });
    }
    
    stopListener() {
        if (this.listenerProcess) {
            console.log('🛑 Stopping Listener...');
            this.listenerProcess.kill('SIGTERM');
            this.listenerProcess = null;
        }
    }
    
    broadcast(message) {
        const messageStr = JSON.stringify(message);
        this.clients.forEach(client => {
            if (client.readyState === WebSocket.OPEN) {
                client.send(messageStr);
            }
        });
    }
    
    generateDashboardHTML() {
        return `
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>🏰 Ultimate Lighthouse Control Center</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: 'Courier New', monospace;
            background: linear-gradient(135deg, #0c0c0c 0%, #1a1a2e 100%);
            color: #00ff41;
            min-height: 100vh;
            padding: 20px;
        }
        .container {
            max-width: 1400px;
            margin: 0 auto;
            padding: 20px;
        }
        .header {
            text-align: center;
            margin-bottom: 30px;
            border: 2px solid #00ff41;
            padding: 20px;
            border-radius: 10px;
            background: rgba(0, 255, 65, 0.1);
        }
        .header h1 {
            font-size: 2.5em;
            margin-bottom: 10px;
            text-shadow: 0 0 10px #00ff41;
        }
        .grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(400px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }
        .panel {
            background: rgba(0, 255, 65, 0.05);
            border: 1px solid #00ff41;
            border-radius: 8px;
            padding: 20px;
            backdrop-filter: blur(10px);
        }
        .panel h3 {
            color: #00ff41;
            margin-bottom: 15px;
            font-size: 1.3em;
            border-bottom: 1px solid #00ff41;
            padding-bottom: 10px;
        }
        .controls {
            display: flex;
            gap: 10px;
            flex-wrap: wrap;
            margin-bottom: 15px;
        }
        .btn {
            background: linear-gradient(45deg, #00ff41, #00cc33);
            color: #000;
            border: none;
            padding: 10px 20px;
            border-radius: 5px;
            cursor: pointer;
            font-weight: bold;
            transition: all 0.3s ease;
            text-transform: uppercase;
        }
        .btn:hover {
            background: linear-gradient(45deg, #00cc33, #009922);
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(0, 255, 65, 0.4);
        }
        .btn.danger {
            background: linear-gradient(45deg, #ff4444, #cc0000);
            color: white;
        }
        .btn.danger:hover {
            background: linear-gradient(45deg, #cc0000, #990000);
        }
        .status {
            font-family: monospace;
            font-size: 0.9em;
            line-height: 1.6;
        }
        .status-item {
            display: flex;
            justify-content: space-between;
            margin-bottom: 8px;
            padding: 5px;
            background: rgba(0, 0, 0, 0.3);
            border-radius: 3px;
        }
        .output {
            background: #000;
            border: 1px solid #00ff41;
            border-radius: 5px;
            padding: 15px;
            height: 300px;
            overflow-y: auto;
            font-family: monospace;
            font-size: 0.8em;
            line-height: 1.4;
        }
        .metrics {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 15px;
        }
        .metric {
            text-align: center;
            padding: 15px;
            background: rgba(0, 0, 0, 0.5);
            border-radius: 8px;
            border: 1px solid #00ff41;
        }
        .metric-value {
            font-size: 1.8em;
            font-weight: bold;
            color: #00ff41;
            margin-bottom: 5px;
        }
        .metric-label {
            font-size: 0.9em;
            opacity: 0.8;
        }
        .beacon-info {
            background: rgba(0, 255, 65, 0.1);
            border: 1px solid #00ff41;
            border-radius: 5px;
            padding: 15px;
            margin: 10px 0;
        }
        @keyframes glow {
            0%, 100% { box-shadow: 0 0 5px #00ff41; }
            50% { box-shadow: 0 0 20px #00ff41, 0 0 30px #00ff41; }
        }
        .connected {
            animation: glow 2s infinite;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🏰 Ultimate Lighthouse Control Center</h1>
            <p>SimdJSON Powered Real-Time Monitoring System</p>
        </div>
        
        <div class="grid">
            <div class="panel">
                <h3>🚀 Lighthouse Control</h3>
                <div class="controls">
                    <button class="btn" onclick="startLighthouse()">Start Lighthouse</button>
                    <button class="btn danger" onclick="stopLighthouse()">Stop Lighthouse</button>
                </div>
                <div class="status" id="lighthouse-status">
                    <div class="status-item">
                        <span>Status:</span>
                        <span id="lighthouse-status-value">Initializing...</span>
                    </div>
                </div>
            </div>
            
            <div class="panel">
                <h3>🎧 Beacon Listener</h3>
                <div class="controls">
                    <button class="btn" onclick="startListener()">Start Listener</button>
                    <button class="btn danger" onclick="stopListener()">Stop Listener</button>
                </div>
                <div class="status" id="listener-status">
                    <div class="status-item">
                        <span>Listening on:</span>
                        <span>Port ${CONFIG.LISTENER_PORT}</span>
                    </div>
                </div>
            </div>
        </div>
        
        <div class="grid">
            <div class="panel">
                <h3>📊 Performance Metrics</h3>
                <div class="metrics">
                    <div class="metric">
                        <div class="metric-value" id="parse-time">0.00</div>
                        <div class="metric-label">Parse Time (μs)</div>
                    </div>
                    <div class="metric">
                        <div class="metric-value" id="throughput">0.0</div>
                        <div class="metric-label">Throughput (MB/s)</div>
                    </div>
                    <div class="metric">
                        <div class="metric-value" id="success-rate">100</div>
                        <div class="metric-label">Success Rate (%)</div>
                    </div>
                    <div class="metric">
                        <div class="metric-value" id="connections">0</div>
                        <div class="metric-label">Active Connections</div>
                    </div>
                </div>
            </div>
            
            <div class="panel">
                <h3>📡 Last Beacon</h3>
                <div id="beacon-info">
                    <p>Waiting for beacon data...</p>
                </div>
            </div>
        </div>
        
        <div class="panel">
            <h3>📟 System Output</h3>
            <div class="output" id="system-output">
                <p>🏰 Ultimate Lighthouse Web Interface Ready</p>
                <p>🔌 WebSocket connection: <span id="ws-status">Connecting...</span></p>
            </div>
        </div>
    </div>
    
    <script>
        const wsUrl = 'ws://localhost:${CONFIG.WS_PORT}';
        let ws = null;
        let reconnectInterval = null;
        
        function connect() {
            ws = new WebSocket(wsUrl);
            
            ws.onopen = () => {
                document.getElementById('ws-status').textContent = 'Connected ✅';
                document.getElementById('ws-status').className = 'connected';
                addOutput('🔌 WebSocket connected successfully');
                clearInterval(reconnectInterval);
            };
            
            ws.onmessage = (event) => {
                const message = JSON.parse(event.data);
                handleMessage(message);
            };
            
            ws.onclose = () => {
                document.getElementById('ws-status').textContent = 'Disconnected ❌';
                document.getElementById('ws-status').className = '';
                addOutput('🔌 WebSocket disconnected - attempting reconnect...');
                reconnectInterval = setInterval(connect, 5000);
            };
            
            ws.onerror = (error) => {
                addOutput('🚨 WebSocket error: ' + error);
            };
        }
        
        function handleMessage(message) {
            switch (message.type) {
                case 'status':
                    updateStatus(message.data);
                    break;
                case 'beacon':
                    updateBeacon(message.data);
                    break;
                case 'lighthouse_output':
                    addOutput('🏰 ' + message.data);
                    break;
                case 'listener_output':
                    addOutput('🎧 ' + message.data);
                    break;
                case 'lighthouse_stopped':
                    addOutput('🛑 Lighthouse stopped');
                    break;
                case 'listener_stopped':
                    addOutput('🛑 Listener stopped');
                    break;
            }
        }
        
        function updateStatus(data) {
            document.getElementById('lighthouse-status-value').textContent = data.lighthouseStatus;
            document.getElementById('parse-time').textContent = data.performance.avgParseTime.toFixed(2);
            document.getElementById('throughput').textContent = data.performance.throughput.toFixed(1);
            document.getElementById('success-rate').textContent = data.performance.successRate.toFixed(1);
            document.getElementById('connections').textContent = data.activeConnections;
        }
        
        function updateBeacon(beacon) {
            const beaconInfo = document.getElementById('beacon-info');
            const receivedTime = new Date(beacon.receivedAt).toLocaleTimeString();
            
            beaconInfo.innerHTML = \`
                <div class="beacon-info">
                    <strong>🚨 Beacon #\${beacon.beacon_sequence_number || 'N/A'}</strong><br>
                    <strong>Status:</strong> \${beacon.status}<br>
                    <strong>From:</strong> \${beacon.receivedFrom}<br>
                    <strong>Ping Status:</strong> \${beacon.last_ping_status}<br>
                    <strong>Latency:</strong> \${beacon.ping_latency_ms}ms<br>
                    <strong>Signal Age:</strong> \${beacon.signal_age_seconds}s<br>
                    <strong>Received:</strong> \${receivedTime}<br>
                    <strong>Version:</strong> \${beacon.lighthouse_version}
                </div>
            \`;
            
            addOutput(\`📡 Beacon received: \${beacon.status} from \${beacon.receivedFrom}\`);
        }
        
        function addOutput(text) {
            const output = document.getElementById('system-output');
            const time = new Date().toLocaleTimeString();
            output.innerHTML += \`<p>[\${time}] \${text}</p>\`;
            output.scrollTop = output.scrollHeight;
        }
        
        function startLighthouse() {
            if (ws && ws.readyState === WebSocket.OPEN) {
                ws.send(JSON.stringify({ type: 'startLighthouse' }));
            }
        }
        
        function stopLighthouse() {
            if (ws && ws.readyState === WebSocket.OPEN) {
                ws.send(JSON.stringify({ type: 'stopLighthouse' }));
            }
        }
        
        function startListener() {
            if (ws && ws.readyState === WebSocket.OPEN) {
                ws.send(JSON.stringify({ type: 'startListener' }));
            }
        }
        
        function stopListener() {
            if (ws && ws.readyState === WebSocket.OPEN) {
                ws.send(JSON.stringify({ type: 'stopListener' }));
            }
        }
        
        // Initialize connection
        connect();
        
        // Request initial status
        setTimeout(() => {
            if (ws && ws.readyState === WebSocket.OPEN) {
                ws.send(JSON.stringify({ type: 'requestStatus' }));
            }
        }, 1000);
    </script>
</body>
</html>
        `;
    }
    
    start() {
        this.server.listen(CONFIG.PORT, () => {
            console.log(`🚀 Ultimate Lighthouse Web Server running on port ${CONFIG.PORT}`);
            console.log(`🌐 Dashboard: http://localhost:${CONFIG.PORT}`);
            console.log(`📊 API: http://localhost:${CONFIG.PORT}/api/status`);
            console.log(`🔌 WebSocket: ws://localhost:${CONFIG.WS_PORT}`);
            
            // Setup periodic broadcasts
            setInterval(() => {
                this.broadcast({
                    type: 'status',
                    data: this.systemStats
                });
            }, CONFIG.UPDATE_INTERVAL);
        });
    }
    
    shutdown() {
        console.log('🛑 Shutting down Ultimate Lighthouse Web Server...');
        
        this.stopLighthouse();
        this.stopListener();
        
        if (this.udpListener) {
        this.udpListener.close();
        }
        
        this.wss.close();
        this.server.close();
        
        console.log('🏰 Ultimate Lighthouse Web Server secured!');
        process.exit(0);
    }
}

// Handle command line arguments
const args = process.argv.slice(2);
const mode = args.find(arg => arg.startsWith('--mode='))?.split('=')[1];

// Create and start the web server
const lighthouseWeb = new UltimateLighthouseWebServer();

// Auto-start based on mode
if (mode === 'lighthouse') {
    console.log('🚀 Auto-starting in Lighthouse mode...');
    setTimeout(() => lighthouseWeb.startLighthouse(), 2000);
} else if (mode === 'listener') {
    console.log('🎧 Auto-starting in Listener mode...');
    setTimeout(() => lighthouseWeb.startListener(), 2000);
}

// Graceful shutdown handling
process.on('SIGINT', () => {
    console.log('\n🛑 Received SIGINT signal');
    lighthouseWeb.shutdown();
});

process.on('SIGTERM', () => {
    console.log('\n🛑 Received SIGTERM signal');
    lighthouseWeb.shutdown();
});

process.on('uncaughtException', (error) => {
    console.error('🚨 Uncaught Exception:', error);
    lighthouseWeb.shutdown();
});

process.on('unhandledRejection', (reason, promise) => {
    console.error('🚨 Unhandled Rejection at:', promise, 'reason:', reason);
});

// Start the web server
lighthouseWeb.start();

// Export for testing or module usage
module.exports = UltimateLighthouseWebServer;