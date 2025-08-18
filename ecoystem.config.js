module.exports = {
  apps: [
    {
      name: 'ultimate-lighthouse-web',
      script: 'server.js',
      cwd: __dirname,
      instances: 1,
      autorestart: true,
      watch: false,
      max_memory_restart: '1G',
      env: {
        NODE_ENV: 'production',
        PORT: 3000,
        WS_PORT: 3001,
        LIGHTHOUSE_EXECUTABLE: './litehaus',
        BEACON_IP: '138.68.142.181',
        BEACON_PORT: 9876,
        LISTENER_PORT: 9876,
        UPDATE_INTERVAL: 5000,
        LOG_LEVEL: 'info'
      },
      env_development: {
        NODE_ENV: 'development',
        PORT: 3000,
        WS_PORT: 3001,
        LOG_LEVEL: 'debug'
      },
      error_file: './logs/lighthouse-error.log',
      out_file: './logs/lighthouse-out.log',
      log_file: './logs/lighthouse-combined.log',
      time: true,
      log_date_format: 'YYYY-MM-DD HH:mm:ss Z',
      merge_logs: true,
      kill_timeout: 5000,
      wait_ready: true,
      listen_timeout: 10000,
      
      // Advanced PM2 features
      exec_mode: 'fork',
      min_uptime: '10s',
      max_restarts: 10,
      restart_delay: 4000,
      
      // Monitoring
      pmx: true,
      
      // Custom startup hook
      post_update: ['npm install', 'echo "🏰 Ultimate Lighthouse updated successfully"']
    },
    
    // Optional: Run lighthouse and listener as separate processes
    {
      name: 'lighthouse-beacon',
      script: 'server.js',
      args: '--mode=lighthouse',
      cwd: __dirname,
      instances: 1,
      autorestart: true,
      watch: false,
      max_memory_restart: '512M',
      env: {
        NODE_ENV: 'production',
        PORT: 3001,
        WS_PORT: 3002,
        LIGHTHOUSE_EXECUTABLE: './litehaus'
      },
      error_file: './logs/beacon-error.log',
      out_file: './logs/beacon-out.log',
      log_file: './logs/beacon-combined.log',
      time: true,
      merge_logs: true,
      
      // This one auto-starts the lighthouse process
      kill_timeout: 5000,
      min_uptime: '10s',
      max_restarts: 5,
      restart_delay: 2000
    },
    
    {
      name: 'lighthouse-listener', 
      script: 'server.js',
      args: '--mode=listener',
      cwd: __dirname,
      instances: 1,
      autorestart: true,
      watch: false,
      max_memory_restart: '512M',
      env: {
        NODE_ENV: 'production',
        PORT: 3002,
        WS_PORT: 3003,
        LIGHTHOUSE_EXECUTABLE: './litehaus',
        LISTENER_PORT: 9876
      },
      error_file: './logs/listener-error.log',
      out_file: './logs/listener-out.log', 
      log_file: './logs/listener-combined.log',
      time: true,
      merge_logs: true,
      
      kill_timeout: 5000,
      min_uptime: '10s',
      max_restarts: 5,
      restart_delay: 2000
    }
  ],

  deploy: {
    production: {
      user: 'lighthouse',
      host: 'your-server.com',
      ref: 'origin/main',
      repo: 'git@github.com:whisprer/ultimate-lighthouse.git',
      path: '/var/www/ultimate-lighthouse',
      'pre-deploy-local': '',
      'post-deploy': 'npm install && pm2 reload ecosystem.config.js --env production',
      'pre-setup': '',
      env: {
        NODE_ENV: 'production'
      }
    }
  }
};w