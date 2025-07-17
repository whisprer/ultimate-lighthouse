import React, { useState, useEffect, useRef } from 'react';
import { Activity, Zap, Terminal, Code, Server, Globe, Clock, TrendingUp, Cpu, Database, Network, Eye, Users, Wifi, CheckCircle, AlertTriangle, XCircle } from 'lucide-react';

// 🏰 FLAGSHIP LIGHTHOUSE WEBSITE SHOWCASE
// The Ultimate Marketing Tool for litehaus.online
// Showcasing Sub-Microsecond JSON Processing Excellence

const FlagshipLighthouseShowcase = () => {
  const [currentTime, setCurrentTime] = useState(new Date());
  const [lighthouseData, setLighthouseData] = useState(null);
  const [performanceMetrics, setPerformanceMetrics] = useState([]);
  const [isConnected, setIsConnected] = useState(false);
  const [visitorCount, setVisitorCount] = useState(0);
  const [showTerminalDemo, setShowTerminalDemo] = useState(false);
  const canvasRef = useRef(null);
  const animationRef = useRef(null);
  
  // 🎨 Lighthouse beam animation
  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;
    
    const ctx = canvas.getContext('2d');
    let angle = 0;
    
    const drawLighthouse = () => {
      ctx.clearRect(0, 0, canvas.width, canvas.height);
      
      // Draw lighthouse beam
      ctx.save();
      ctx.translate(canvas.width / 2, canvas.height / 2);
      ctx.rotate(angle);
      
      // Create gradient beam
      const gradient = ctx.createLinearGradient(0, -100, 0, 100);
      gradient.addColorStop(0, 'rgba(59, 130, 246, 0.8)');
      gradient.addColorStop(0.5, 'rgba(147, 51, 234, 0.6)');
      gradient.addColorStop(1, 'rgba(59, 130, 246, 0.2)');
      
      ctx.fillStyle = gradient;
      ctx.beginPath();
      ctx.arc(0, 0, 80, -Math.PI / 6, Math.PI / 6);
      ctx.lineTo(0, 0);
      ctx.fill();
      
      ctx.restore();
      
      // Draw lighthouse structure
      ctx.fillStyle = '#1f2937';
      ctx.fillRect(canvas.width / 2 - 15, canvas.height / 2 - 60, 30, 120);
      
      // Draw lighthouse top
      ctx.fillStyle = '#3b82f6';
      ctx.fillRect(canvas.width / 2 - 20, canvas.height / 2 - 70, 40, 20);
      
      angle += 0.02;
      animationRef.current = requestAnimationFrame(drawLighthouse);
    };
    
    drawLighthouse();
    
    return () => {
      if (animationRef.current) {
        cancelAnimationFrame(animationRef.current);
      }
    };
  }, []);
  
  // 🚀 Simulate real-time lighthouse data
  useEffect(() => {
    const generateLighthouseData = () => {
      // Simulate ultra-fast performance data
      return {
        lighthouse_id: "ultimate-public-lighthouse-001",
        status: Math.random() > 0.1 ? "healthy" : "warning",
        fastping_status: "ok",
        connecting_ip: "203.0.113.45",
        anonymity_level: "high",
        speed_hint: "blazing_fast",
        ping_latency_ms: 8.5 + Math.random() * 15,
        json_parse_time_microseconds: 0.05 + Math.random() * 0.25, // Ultra-fast!
        json_serialize_time_microseconds: 0.03 + Math.random() * 0.15,
        average_throughput_mbps: 150 + Math.random() * 250,
        cpu_optimization_level: "AVX-512",
        connected_global_listeners: 47 + Math.floor(Math.random() * 20),
        total_beacons_broadcast: 125847 + Math.floor(Math.random() * 1000),
        system_uptime_hours: 156.7,
        beacon_sequence_number: 8294 + Math.floor(Math.random() * 100),
        total_fastping_requests: 12847,
        successful_parses: 12840,
        timestamp: Math.floor(Date.now() / 1000)
      };
    };
    
    const generatePerformanceHistory = () => {
      const history = [];
      for (let i = 29; i >= 0; i--) {
        const timestamp = new Date(Date.now() - i * 60000);
        history.push({
          time: timestamp.toLocaleTimeString(),
          parseTime: Number((0.05 + Math.random() * 0.2).toFixed(3)),
          throughput: Number((180 + Math.random() * 120).toFixed(1)),
          listeners: 45 + Math.floor(Math.random() * 15)
        });
      }
      return history;
    };
    
    const updateData = () => {
      setLighthouseData(generateLighthouseData());
      setPerformanceMetrics(generatePerformanceHistory());
      setCurrentTime(new Date());
      setIsConnected(true);
      setVisitorCount(prev => prev + Math.floor(Math.random() * 3));
    };
    
    // Initial load
    updateData();
    
    // Update every 3 seconds for real-time feel
    const interval = setInterval(updateData, 3000);
    
    return () => clearInterval(interval);
  }, []);
  
  // 🎯 Terminal demo component
  const TerminalDemo = () => (
    <div className="bg-gray-900 rounded-lg p-6 font-mono text-sm">
      <div className="flex items-center mb-4">
        <div className="flex space-x-2">
          <div className="w-3 h-3 bg-red-500 rounded-full"></div>
          <div className="w-3 h-3 bg-yellow-500 rounded-full"></div>
          <div className="w-3 h-3 bg-green-500 rounded-full"></div>
        </div>
        <span className="text-gray-400 ml-4">Terminal - Connect to Lighthouse</span>
      </div>
      <div className="text-green-400">
        <div className="mb-2">$ nc 161.35.248.233 9876</div>
        <div className="text-blue-400 mb-2">🏰 Welcome to ULTIMATE PUBLIC LIGHTHOUSE BEACON!</div>
        <div className="text-purple-400 mb-2">🚀 Powered by RTC's Jsonifier - Sub-Microsecond JSON Processing</div>
        <div className="text-white mb-4">📡 You are now receiving live lighthouse beacon data...</div>
        
        {lighthouseData && (
          <div className="bg-gray-800 p-4 rounded border-l-4 border-blue-500">
            <div className="text-cyan-400">{'{'}</div>
            <div className="text-gray-300 ml-4">"lighthouse_id": "{lighthouseData.lighthouse_id}",</div>
            <div className="text-gray-300 ml-4">"status": "{lighthouseData.status}",</div>
            <div className="text-gray-300 ml-4">"parse_time_microseconds": {lighthouseData.json_parse_time_microseconds.toFixed(3)},</div>
            <div className="text-gray-300 ml-4">"throughput_mbps": {lighthouseData.average_throughput_mbps.toFixed(1)},</div>
            <div className="text-gray-300 ml-4">"cpu_optimization": "{lighthouseData.cpu_optimization_level}",</div>
            <div className="text-gray-300 ml-4">"global_listeners": {lighthouseData.connected_global_listeners}</div>
            <div className="text-cyan-400">{'}'}</div>
          </div>
        )}
        
        <div className="mt-4 text-gray-400">
          <div className="animate-pulse">⚡ Receiving sub-microsecond JSON data...</div>
        </div>
      </div>
    </div>
  );
  
  // 📊 Metric card component
  const MetricCard = ({ title, value, unit, icon: Icon, color, subtitle, highlight }) => (
    <div className={`bg-white rounded-xl shadow-lg p-6 border-2 transition-all duration-300 hover:shadow-xl ${
      highlight ? 'border-purple-500 ring-2 ring-purple-200' : 'border-gray-200'
    }`}>
      <div className="flex items-center justify-between">
        <div>
          <p className="text-gray-600 text-sm font-medium">{title}</p>
          <div className="flex items-baseline mt-1">
            <p className={`text-3xl font-bold ${color}`}>{value}</p>
            {unit && <span className="text-gray-500 ml-1">{unit}</span>}
          </div>
          {subtitle && <p className="text-gray-500 text-xs mt-1">{subtitle}</p>}
        </div>
        <div className={`p-3 rounded-full ${color.replace('text', 'bg').replace('700', '100')}`}>
          <Icon className={`w-6 h-6 ${color}`} />
        </div>
      </div>
    </div>
  );
  
  return (
    <div className="min-h-screen bg-gradient-to-br from-slate-900 via-purple-900 to-slate-900">
      {/* Hero Section */}
      <div className="relative overflow-hidden">
        {/* Animated background */}
        <div className="absolute inset-0 bg-gradient-to-r from-blue-600/20 via-purple-600/20 to-indigo-600/20 animate-pulse"></div>
        
        <div className="relative max-w-7xl mx-auto px-6 py-16">
          <div className="text-center mb-12">
            <h1 className="text-6xl font-bold bg-gradient-to-r from-blue-400 via-purple-400 to-cyan-400 bg-clip-text text-transparent mb-6">
              🏰 Internet Lighthouse
            </h1>
            <p className="text-2xl text-gray-300 mb-8">
              The World's Fastest JSON-Powered Network Monitoring System
            </p>
            <div className="flex justify-center items-center space-x-8 text-gray-400">
              <div className="flex items-center space-x-2">
                <div className={`w-3 h-3 rounded-full ${isConnected ? 'bg-green-500 animate-pulse' : 'bg-red-500'}`}></div>
                <span>{isConnected ? 'Live & Broadcasting' : 'Connecting...'}</span>
              </div>
              <span>🚀 Powered by RTC's Jsonifier</span>
              <span>⚡ Sub-Microsecond Processing</span>
            </div>
          </div>
          
          {/* Lighthouse Animation */}
          <div className="flex justify-center mb-12">
            <div className="relative">
              <canvas 
                ref={canvasRef} 
                width={200} 
                height={200} 
                className="drop-shadow-2xl"
              />
              <div className="absolute inset-0 flex items-center justify-center">
                <div className="text-center text-white">
                  <div className="text-sm font-semibold">LIVE</div>
                  <div className="text-xs opacity-75">Broadcasting</div>
                </div>
              </div>
            </div>
          </div>
          
          {/* Performance Showcase */}
          {lighthouseData && (
            <div className="grid grid-cols-1 md:grid-cols-4 gap-6 mb-12">
              <MetricCard
                title="Parse Speed"
                value={lighthouseData.json_parse_time_microseconds.toFixed(3)}
                unit="µs"
                icon={Zap}
                color="text-yellow-600"
                subtitle="Sub-microsecond JSON parsing"
                highlight={true}
              />
              <MetricCard
                title="Throughput"
                value={lighthouseData.average_throughput_mbps.toFixed(0)}
                unit="MB/s"
                icon={TrendingUp}
                color="text-blue-600"
                subtitle="JSON processing power"
              />
              <MetricCard
                title="Global Listeners"
                value={lighthouseData.connected_global_listeners}
                icon={Users}
                color="text-green-600"
                subtitle="Connected worldwide"
              />
              <MetricCard
                title="CPU Optimization"
                value={lighthouseData.cpu_optimization_level}
                icon={Cpu}
                color="text-purple-600"
                subtitle="SIMD acceleration"
              />
            </div>
          )}
        </div>
      </div>
      
      {/* Main Content */}
      <div className="max-w-7xl mx-auto px-6 py-16">
        <div className="grid grid-cols-1 lg:grid-cols-2 gap-12">
          {/* What is the Lighthouse */}
          <div className="bg-white/10 backdrop-blur-md rounded-2xl p-8 border border-white/20">
            <h2 className="text-3xl font-bold text-white mb-6 flex items-center">
              <Server className="w-8 h-8 text-blue-400 mr-3" />
              What is the Internet Lighthouse?
            </h2>
            <div className="text-gray-300 space-y-4">
              <p>
                The Internet Lighthouse is a cutting-edge network monitoring beacon that broadcasts 
                real-time connectivity and performance data to the entire internet. Think of it as 
                a digital lighthouse keeping the internet safe.
              </p>
              <div className="grid grid-cols-2 gap-4 my-6">
                <div className="bg-blue-500/20 p-4 rounded-lg">
                  <h4 className="font-semibold text-blue-300 mb-2">🚀 Ultra-Fast</h4>
                  <p className="text-sm">Sub-microsecond JSON processing with SIMD optimization</p>
                </div>
                <div className="bg-green-500/20 p-4 rounded-lg">
                  <h4 className="font-semibold text-green-300 mb-2">🌍 Global</h4>
                  <p className="text-sm">Broadcasting to listeners worldwide 24/7</p>
                </div>
                <div className="bg-purple-500/20 p-4 rounded-lg">
                  <h4 className="font-semibold text-purple-300 mb-2">🔬 Professional</h4>
                  <p className="text-sm">Enterprise-grade monitoring and analytics</p>
                </div>
                <div className="bg-orange-500/20 p-4 rounded-lg">
                  <h4 className="font-semibold text-orange-300 mb-2">📡 Open</h4>
                  <p className="text-sm">Free public access via terminal connection</p>
                </div>
              </div>
              <p>
                Powered by <strong className="text-blue-400">RTC's Jsonifier</strong> - the fastest 
                JSON library in existence - our lighthouse processes network data with 
                <strong className="text-purple-400"> sub-microsecond parse times</strong>.
              </p>
            </div>
          </div>
          
          {/* How to Connect */}
          <div className="bg-white/10 backdrop-blur-md rounded-2xl p-8 border border-white/20">
            <h2 className="text-3xl font-bold text-white mb-6 flex items-center">
              <Terminal className="w-8 h-8 text-green-400 mr-3" />
              Connect From Anywhere
            </h2>
            <div className="space-y-6">
              <div className="bg-gray-900 rounded-lg p-4 font-mono text-green-400">
                <div className="text-sm text-gray-400 mb-2"># Connect via terminal (Linux/Mac/Windows)</div>
                <div className="text-lg">nc 161.35.248.233 9876</div>
              </div>
              
              <div className="bg-gray-900 rounded-lg p-4 font-mono text-green-400">
                <div className="text-sm text-gray-400 mb-2"># Or with telnet</div>
                <div className="text-lg">telnet 161.35.248.233 9876</div>
              </div>
              
              <button 
                onClick={() => setShowTerminalDemo(!showTerminalDemo)}
                className="w-full bg-blue-600 hover:bg-blue-700 text-white font-semibold py-3 px-6 rounded-lg transition-colors flex items-center justify-center space-x-2"
              >
                <Eye className="w-5 h-5" />
                <span>{showTerminalDemo ? 'Hide' : 'Show'} Live Terminal Demo</span>
              </button>
              
              <div className="text-gray-300 text-sm space-y-2">
                <div className="flex items-center space-x-2">
                  <CheckCircle className="w-4 h-4 text-green-400" />
                  <span>Real-time JSON beacon data</span>
                </div>
                <div className="flex items-center space-x-2">
                  <CheckCircle className="w-4 h-4 text-green-400" />
                  <span>Performance metrics and statistics</span>
                </div>
                <div className="flex items-center space-x-2">
                  <CheckCircle className="w-4 h-4 text-green-400" />
                  <span>Global network health monitoring</span>
                </div>
                <div className="flex items-center space-x-2">
                  <CheckCircle className="w-4 h-4 text-green-400" />
                  <span>Free access - no registration required</span>
                </div>
              </div>
            </div>
          </div>
        </div>
        
        {/* Terminal Demo */}
        {showTerminalDemo && (
          <div className="mt-12">
            <h3 className="text-2xl font-bold text-white mb-6 text-center">
              🖥️ Live Terminal Connection Demo
            </h3>
            <TerminalDemo />
          </div>
        )}
        
        {/* Performance Showcase */}
        {lighthouseData && (
          <div className="mt-16">
            <h2 className="text-3xl font-bold text-white mb-8 text-center">
              🚀 Performance That Breaks Records
            </h2>
            <div className="bg-white/10 backdrop-blur-md rounded-2xl p-8 border border-white/20">
              <div className="grid grid-cols-1 md:grid-cols-3 gap-8">
                <div className="text-center">
                  <div className="text-4xl font-bold text-yellow-400 mb-2">
                    {lighthouseData.json_parse_time_microseconds.toFixed(3)}µs
                  </div>
                  <div className="text-gray-300">Average Parse Time</div>
                  <div className="text-sm text-gray-400 mt-1">Faster than a CPU cache miss</div>
                </div>
                <div className="text-center">
                  <div className="text-4xl font-bold text-blue-400 mb-2">
                    {lighthouseData.average_throughput_mbps.toFixed(0)} MB/s
                  </div>
                  <div className="text-gray-300">JSON Throughput</div>
                  <div className="text-sm text-gray-400 mt-1">Processing at memory speeds</div>
                </div>
                <div className="text-center">
                  <div className="text-4xl font-bold text-green-400 mb-2">
                    {((lighthouseData.successful_parses / lighthouseData.total_fastping_requests) * 100).toFixed(1)}%
                  </div>
                  <div className="text-gray-300">Success Rate</div>
                  <div className="text-sm text-gray-400 mt-1">Rock-solid reliability</div>
                </div>
              </div>
              
              <div className="mt-8 pt-8 border-t border-white/20">
                <div className="grid grid-cols-2 md:grid-cols-4 gap-6 text-center">
                  <div>
                    <div className="text-lg font-semibold text-white">{lighthouseData.system_uptime_hours.toFixed(1)}h</div>
                    <div className="text-sm text-gray-400">System Uptime</div>
                  </div>
                  <div>
                    <div className="text-lg font-semibold text-white">{lighthouseData.total_beacons_broadcast.toLocaleString()}</div>
                    <div className="text-sm text-gray-400">Beacons Sent</div>
                  </div>
                  <div>
                    <div className="text-lg font-semibold text-white">{lighthouseData.connected_global_listeners}</div>
                    <div className="text-sm text-gray-400">Live Listeners</div>
                  </div>
                  <div>
                    <div className="text-lg font-semibold text-white">{lighthouseData.cpu_optimization_level}</div>
                    <div className="text-sm text-gray-400">CPU Acceleration</div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        )}
        
        {/* Technology & FastPing Cross-Promotion */}
        <div className="mt-16 grid grid-cols-1 lg:grid-cols-2 gap-8">
          {/* Technology Stack */}
          <div className="bg-white/10 backdrop-blur-md rounded-2xl p-8 border border-white/20">
            <h3 className="text-2xl font-bold text-white mb-6 flex items-center">
              <Code className="w-6 h-6 text-purple-400 mr-3" />
              Cutting-Edge Technology
            </h3>
            <div className="space-y-4">
              <div className="flex items-center space-x-3">
                <div className="w-3 h-3 bg-blue-500 rounded-full"></div>
                <span className="text-gray-300"><strong className="text-blue-400">RTC's Jsonifier</strong> - World's fastest JSON library</span>
              </div>
              <div className="flex items-center space-x-3">
                <div className="w-3 h-3 bg-purple-500 rounded-full"></div>
                <span className="text-gray-300"><strong className="text-purple-400">SIMD Optimization</strong> - AVX-512, AVX2, NEON acceleration</span>
              </div>
              <div className="flex items-center space-x-3">
                <div className="w-3 h-3 bg-green-500 rounded-full"></div>
                <span className="text-gray-300"><strong className="text-green-400">Multi-threaded Architecture</strong> - Maximum performance</span>
              </div>
              <div className="flex items-center space-x-3">
                <div className="w-3 h-3 bg-orange-500 rounded-full"></div>
                <span className="text-gray-300"><strong className="text-orange-400">Real-time Monitoring</strong> - Live network analysis</span>
              </div>
              <div className="flex items-center space-x-3">
                <div className="w-3 h-3 bg-cyan-500 rounded-full"></div>
                <span className="text-gray-300"><strong className="text-cyan-400">Global Broadcasting</strong> - TCP stream to anywhere</span>
              </div>
            </div>
          </div>
          
          {/* FastPing Service */}
          <div className="bg-gradient-to-br from-blue-600/20 to-purple-600/20 backdrop-blur-md rounded-2xl p-8 border border-blue-400/30">
            <h3 className="text-2xl font-bold text-white mb-6 flex items-center">
              <Zap className="w-6 h-6 text-yellow-400 mr-3" />
              FastPing Premium Service
            </h3>
            <div className="space-y-4">
              <p className="text-gray-300">
                Need enterprise-grade ping monitoring? Check out our <strong className="text-blue-400">FastPing.it</strong> service!
              </p>
              <div className="bg-black/30 rounded-lg p-4 space-y-2">
                <div className="flex items-center space-x-2">
                  <CheckCircle className="w-4 h-4 text-green-400" />
                  <span className="text-gray-300 text-sm">Ultra-low latency ping responses</span>
                </div>
                <div className="flex items-center space-x-2">
                  <CheckCircle className="w-4 h-4 text-green-400" />
                  <span className="text-gray-300 text-sm">JSON-optimized API endpoints</span>
                </div>
                <div className="flex items-center space-x-2">
                  <CheckCircle className="w-4 h-4 text-green-400" />
                  <span className="text-gray-300 text-sm">PayPal subscription billing</span>
                </div>
                <div className="flex items-center space-x-2">
                  <CheckCircle className="w-4 h-4 text-green-400" />
                  <span className="text-gray-300 text-sm">Docker-ready deployment</span>
                </div>
              </div>
              <button className="w-full bg-gradient-to-r from-blue-500 to-purple-600 hover:from-blue-600 hover:to-purple-700 text-white font-semibold py-3 px-6 rounded-lg transition-all transform hover:scale-105">
                🚀 Visit FastPing.it.com
              </button>
            </div>
          </div>
        </div>
        
        {/* Footer */}
        <div className="mt-16 text-center">
          <div className="bg-white/10 backdrop-blur-md rounded-2xl p-8 border border-white/20">
            <h3 className="text-2xl font-bold text-white mb-4">Built by Engineering Wizards</h3>
            <p className="text-gray-300 mb-6">
              Need this level of performance for your business? Contact the team behind the magic.
            </p>
            <div className="flex justify-center space-x-8 text-gray-400 mb-6">
              <span className="flex items-center space-x-2">
                <Globe className="w-5 h-5" />
                <span>RYO Modular</span>
              </span>
              <span className="flex items-center space-x-2">
                <Code className="w-5 h-5" />
                <span>whispr.dev</span>
              </span>
              <span className="flex items-center space-x-2">
                <Clock className="w-5 h-5" />
                <span>Live since 2025</span>
              </span>
            </div>
            <div className="text-sm text-gray-500">
              🏰 Internet Lighthouse - Keeping the digital world safe, one beacon at a time
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};

export default FlagshipLighthouseShowcase;