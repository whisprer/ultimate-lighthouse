#!/bin/bash

# 🏰 Ultimate Lighthouse Production Startup Script
# This script sets up and starts your lighthouse system to run permanently

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${PURPLE}"
echo "🏰 ═══════════════════════════════════════════════════════════════════ 🏰"
echo "   ULTIMATE LIGHTHOUSE PRODUCTION DEPLOYMENT SCRIPT"
echo "🚀 Setting up permanent background service 🚀"
echo "🏰 ═══════════════════════════════════════════════════════════════════ 🏰"
echo -e "${NC}"

# Check if running as root for systemd setup
if [[ $EUID -eq 0 && "$1" == "--systemd" ]]; then
    echo -e "${YELLOW}📋 Setting up systemd service...${NC}"
    
    # Create lighthouse user if it doesn't exist
    if ! id "lighthouse" &>/dev/null; then
        echo -e "${BLUE}👤 Creating lighthouse user...${NC}"
        useradd -r -s /bin/false lighthouse
    fi
    
    # Create directories
    mkdir -p /opt/ultimate-lighthouse
    mkdir -p /opt/ultimate-lighthouse/logs
    
    # Copy files (assuming script is run from project directory)
    cp -r * /opt/ultimate-lighthouse/
    chown -R lighthouse:lighthouse /opt/ultimate-lighthouse
    chmod +x /opt/ultimate-lighthouse/litehaus
    
    # Install and start systemd service
    cp ultimate-lighthouse.service /etc/systemd/system/
    systemctl daemon-reload
    systemctl enable ultimate-lighthouse
    systemctl start ultimate-lighthouse
    
    echo -e "${GREEN}✅ Systemd service installed and started!${NC}"
    echo -e "${CYAN}📊 Check status: systemctl status ultimate-lighthouse${NC}"
    echo -e "${CYAN}📝 View logs: journalctl -u ultimate-lighthouse -f${NC}"
    exit 0
fi

# Check for dependencies
echo -e "${BLUE}🔍 Checking dependencies...${NC}"

# Check if Node.js is installed
if ! command -v node &> /dev/null; then
    echo -e "${RED}❌ Node.js is not installed. Please install Node.js first.${NC}"
    echo -e "${YELLOW}💡 Install with: curl -fsSL https://deb.nodesource.com/setup_18.x | sudo -E bash - && sudo apt-get install -y nodejs${NC}"
    exit 1
fi

# Check if npm is installed
if ! command -v npm &> /dev/null; then
    echo -e "${RED}❌ npm is not installed. Please install npm first.${NC}"
    exit 1
fi

# Check if C++ executable exists
if [ ! -f "./litehaus" ]; then
    echo -e "${RED}❌ Lighthouse executable './litehaus' not found.${NC}"
    echo -e "${YELLOW}💡 Compile it first with: g++ -o litehaus litehaus.-executablecpp -lsimdjson${NC}"
    exit 1
fi

# Make executable if not already
chmod +x ./litehaus

# Install PM2 if not already installed
if ! command -v pm2 &> /dev/null; then
    echo -e "${YELLOW}📦 Installing PM2 process manager...${NC}"
    echo -e "${CYAN}💡 PM2 needs to be installed globally. Choose an option:${NC}"
    echo -e "${GREEN}1)${NC} Install globally with sudo: ${CYAN}sudo npm install -g pm2${NC}"
    echo -e "${GREEN}2)${NC} Install locally in project: ${CYAN}npm install pm2${NC}"
    echo -e "${GREEN}3)${NC} Skip PM2 and run normally: ${CYAN}node server.js${NC}"
    echo ""
    read -p "🤔 Which option? (1/2/3): " pm2_option
    
    case $pm2_option in
        1)
            echo -e "${BLUE}🔐 Installing PM2 globally (requires sudo)...${NC}"
            sudo npm install -g pm2
            ;;
        2)
            echo -e "${BLUE}📦 Installing PM2 locally...${NC}"
            npm install pm2
            # Use local PM2
            PM2_CMD="./node_modules/.bin/pm2"
            ;;
        3)
            echo -e "${BLUE}🚀 Starting without PM2...${NC}"
            echo -e "${YELLOW}⚠️  Note: Process will stop when terminal closes${NC}"
            node server.js &
            echo -e "${GREEN}✅ Lighthouse started! PID: $!${NC}"
            echo -e "${CYAN}🌐 Dashboard: http://localhost:3000${NC}"
            exit 0
            ;;
        *)
            echo -e "${RED}❌ Invalid option. Exiting.${NC}"
            exit 1
            ;;
    esac
else
    PM2_CMD="pm2"
fi

# Install project dependencies
echo -e "${BLUE}📦 Installing project dependencies...${NC}"
npm install

# Create logs directory
mkdir -p logs

# Setup PM2 startup script (so it starts on boot)
if [ "$PM2_CMD" != "./node_modules/.bin/pm2" ]; then
    echo -e "${YELLOW}🔧 Setting up PM2 to start on boot...${NC}"
    echo -e "${CYAN}💡 This requires sudo. Run manually later if needed:${NC}"
    echo -e "${CYAN}   sudo ${PM2_CMD} startup${NC}"
    read -p "🤔 Set up auto-start now? (y/n): " setup_startup
    if [[ $setup_startup =~ ^[Yy]$ ]]; then
        sudo ${PM2_CMD} startup
    fi
else
    echo -e "${YELLOW}⚠️  Local PM2 installed - auto-startup not available${NC}"
fi

echo -e "${GREEN}✅ Setup complete!${NC}"
echo ""
echo -e "${PURPLE}🚀 DEPLOYMENT OPTIONS:${NC}"
echo ""
echo -e "${CYAN}Option 1 - Single Web Interface (Recommended):${NC}"
echo -e "  ${GREEN}pm2 start ecosystem.config.js --only ultimate-lighthouse-web${NC}"
echo ""
echo -e "${CYAN}Option 2 - All Separate Processes:${NC}"
echo -e "  ${GREEN}pm2 start ecosystem.config.js${NC}"
echo ""
echo -e "${CYAN}Option 3 - Quick Start (Web + Auto Lighthouse):${NC}"
echo -e "  ${GREEN}pm2 start server.js --name lighthouse-web${NC}"
echo ""

read -p "🤔 Which option would you like? (1/2/3): " option

case $option in
    1)
        echo -e "${BLUE}🚀 Starting single web interface...${NC}"
        $PM2_CMD start ecosystem.config.js --only ultimate-lighthouse-web
        ;;
    2)
        echo -e "${BLUE}🚀 Starting all processes...${NC}"
        $PM2_CMD start ecosystem.config.js
        ;;
    3)
        echo -e "${BLUE}🚀 Quick starting web interface...${NC}"
        $PM2_CMD start server.js --name lighthouse-web
        ;;
    *)
        echo -e "${YELLOW}⚠️  Invalid option. Starting default (option 1)...${NC}"
        $PM2_CMD start ecosystem.config.js --only ultimate-lighthouse-web
        ;;
esac

# Save PM2 configuration
$PM2_CMD save

echo ""
echo -e "${GREEN}🎉 ULTIMATE LIGHTHOUSE IS NOW RUNNING PERMANENTLY! 🎉${NC}"
echo ""
echo -e "${PURPLE}📊 USEFUL COMMANDS:${NC}"
echo -e "  ${CYAN}View processes:${NC} pm2 list"
echo -e "  ${CYAN}View logs:${NC} pm2 logs"
echo -e "  ${CYAN}Monitor:${NC} pm2 monit"
echo -e "  ${CYAN}Restart:${NC} pm2 restart all"
echo -e "  ${CYAN}Stop:${NC} pm2 stop all"
echo -e "  ${CYAN}Delete:${NC} pm2 delete all"
echo ""
echo -e "${PURPLE}🌐 ACCESS YOUR LIGHTHOUSE:${NC}"
echo -e "  ${GREEN}Dashboard:${NC} http://localhost:3000"
echo -e "  ${GREEN}API:${NC} http://localhost:3000/api/status"
echo ""
echo -e "${YELLOW}💡 TIP: Your lighthouse will automatically restart on server reboot!${NC}"
echo ""

# Show current status
$PM2_CMD list