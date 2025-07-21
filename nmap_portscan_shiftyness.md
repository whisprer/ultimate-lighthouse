/home/wofl/nmap\_profiles/litehaus\_ghost.nmap



```

\# litehaus\_ghost.nmap — stealth-focused scan preset



--top-ports 100                 # Only scan top 100 ports

-sS                             # TCP SYN scan (stealthy)

-T2                             # Slow timing for stealth

--max-retries 2                 # Minimize packet resends

--max-rate 20                   # Throttle packet rate (low)

--min-rate 5                    # Absolute minimum speed

--randomize-hosts               # Avoid sequential IP scanning

--data-length 22                # Pad packets with random junk

--spoof-mac 0                  # Randomize MAC address (0 = vendor)

--source-port 53                # Use DNS source port (often allowed)

--badsum                        # Send a few bogus checksums (detect poorly filtered hosts)

--version-light                 # Light service detection

--disable-arp-ping              # Avoid ARP scan detection

--defeat-icmp-ratelimit        # Deal with ICMP-limited targets

```



* targets.txt

nmap -iL targets.txt -Pn -n -oA ghostscan -v -@ /home/wofl/nmap\_profiles/litehaus\_ghost.nmap



* single host

nmap 123.123.123.123 -Pn -n -oA ghostscan\_single -v -@ /home/wofl/nmap\_profiles/litehaus\_ghost.nmap



-Pn: no ping (assume host is up)

-n: no DNS resolution

-oA: outputs to all 3 formats (normal, XML, grepable)

-v: verbose (add -vv for more)





If you're scanning from a VPS like a Litehaus node, optionally route through a proxy or TOR to hide your origin further:

proxychains4 nmap 123.123.123.123 -Pn -n -@ litehaus\_ghost.nmap

Or add TOR SOCKS routing in proxychains.conf.





…





* 



