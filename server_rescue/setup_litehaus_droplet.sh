#!/bin/bash

### USER SETUP ###
USERNAME="wofl"
PORT="37"  # change if needed

adduser --disabled-password --gecos "" $USERNAME
usermod -aG sudo $USERNAME

### TIMEZONE ###
timedatectl set-timezone Europe/London

### SYSTEM UPDATE & BASE TOOLS ###
apt update && apt upgrade -y
apt install -y git curl wget htop ufw unzip fail2ban build-essential tmux screen zip net-tools dnsutils python3-pip unattended-upgrades watchdog

### SSH HARDENING ###
sed -i "s/^#Port.*/Port $PORT/" /etc/ssh/sshd_config
sed -i "s/^PermitRootLogin.*/PermitRootLogin no/" /etc/ssh/sshd_config
sed -i "s/^#PasswordAuthentication.*/PasswordAuthentication no/" /etc/ssh/sshd_config
echo -e "\nMaxAuthTries 3\nLoginGraceTime 45\nAllowUsers $USERNAME" >> /etc/ssh/sshd_config
systemctl restart ssh

### FIREWALL ###
ufw default deny incoming
ufw default allow outgoing
ufw allow OpenSSH
ufw allow ${PORT}/tcp
ufw allow 9876/tcp  # litehaus
ufw --force enable

### FAIL2BAN ###
cp /etc/fail2ban/jail.conf /etc/fail2ban/jail.local
cat <<EOF >> /etc/fail2ban/jail.local

[sshd]
enabled = true
port = ssh
maxretry = 3
findtime = 600
bantime = 3600
EOF
systemctl restart fail2ban

### UNATTENDED-UPGRADES ###
dpkg-reconfigure --priority=low unattended-upgrades

### WATCHDOG ###
sed -i 's|#watchdog-device.*|watchdog-device = /dev/watchdog|' /etc/watchdog.conf
echo -e "max-load-1 = 24" >> /etc/watchdog.conf
systemctl enable watchdog
systemctl start watchdog

### SYSCTL TUNING ###
cat <<EOF >> /etc/sysctl.conf

# Network tuning for Litehaus
net.core.somaxconn = 1024
net.ipv4.tcp_syncookies = 1
net.ipv4.tcp_max_syn_backlog = 2048
fs.file-max = 100000
net.ipv4.tcp_fin_timeout = 15
net.ipv4.tcp_tw_reuse = 1
net.ipv4.ip_local_port_range = 1024 65000
EOF
sysctl -p

### FILE LIMITS ###
ulimit -n 100000
echo -e "* soft nofile 100000\n* hard nofile 100000" >> /etc/security/limits.conf

echo "🎉 Base droplet config complete. Now copy your litehaus code into /home/$USERNAME/litehaus and define systemd service manually."