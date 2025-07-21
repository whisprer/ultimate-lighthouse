* 
* 
* 
* ssh root@your\_droplet\_ip
* adduser wofl
* usermod -aG sudo wofl
* nano /etc/ssh/sshd\_config

`PermitRootLogin no`

* ssh woflfren@your\_droplet\_ip
* sudo timedatectl set-timezone Europe/London    # or local timezone
sudo timedatectl set-timezone America/New_York
sudo timedatectl set-timezone Australia/Sydney
sudo timedatectl set-timezone Asia/Singapore
* sudo apt update \&\& sudo apt upgrade -y



* sudo apt install -y git curl wget htop ufw unzip fail2ban build-essential



* sudo ufw allow OpenSSH
* sudo ufw enable
* sudo ufw status



* sudo nano /etc/ssh/sshd\_config

```port xx

PermitRootLogin no

PasswordAuthentication no

MaxAuthTries 3

LoginGraceTime 45

AllowUsers wofl```

* sudo systemctl restart ssh
* sudo ufw default deny incoming
* sudo ufw default allow outgoing
* sudo ufw allow OpenSSH
* sudo ufw allow 9876/tcp    # assuming your Litehaus node listens here
* sudo ufw enable
* sudo ufw status verbose



* sudo cp /etc/fail2ban/jail.conf /etc/fail2ban/jail.local
* sudo nano /etc/fail2ban/jail.local

```\[sshd]

enabled = true

port    = ssh

maxretry = 3

findtime = 600

bantime = 3600```

* sudo systemctl restart fail2ban



…



* sudo apt install -y tmux screen zip unzip net-tools dnsutils python3-pip
* sudo apt install -y unattended-upgrades
* sudo dpkg-reconfigure --priority=low unattended-upgrades
* sudo apt install -y watchdog
* sudo nano /etc/watchdog.conf

```watchdog-device = /dev/watchdog

max-load-1 = 24```

* sudo systemctl enable watchdog
* sudo systemctl start watchdog



...



3\. Performance Tuning (for network and concurrency)

* sudo nano /etc/sysctl.conf

```# Network tuning for Litehaus

net.core.somaxconn = 1024

net.ipv4.tcp\_syncookies = 1

net.ipv4.tcp\_max\_syn\_backlog = 2048

fs.file-max = 100000



\# Reduce TIME\_WAIT

net.ipv4.tcp\_fin\_timeout = 15

net.ipv4.tcp\_tw\_reuse = 1

net.ipv4.ip\_local\_port\_range = 1024 65000```



* sudo sysctl -p
* ulimit -n 100000
* sudo nano /etc/security/limits.conf

```\* hard nofile 100000

\* soft nofile 100000```



…

4\. Service \& Litehaus Preparation

* sudo nano /etc/systemd/system/lighthouse-api.service

```\[Unit]

Description=Lighthouse API Bridge

After=network.target



\[Service]

User=woflfren

WorkingDirectory=/home/woflfren/litehaus

ExecStart=/usr/bin/python3 api\_server.py

Restart=always

RestartSec=5



\[Install]

WantedBy=multi-user.target```



* sudo systemctl daemon-reexec
* sudo systemctl enable lighthouse-api
* sudo systemctl start lighthouse-api





Final checklist

Task	Status

SSH hardened and root disabled	✅

New user with sudo	✅

UFW firewall active \& configured	✅

Fail2Ban running	✅

Auto-updates enabled	✅

System performance tuned	✅

Litehaus service defined	✅

Logs being written?	🚧 (confirm path)

Beacon listener/port tested?	🚧 (run test)

