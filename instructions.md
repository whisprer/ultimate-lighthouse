edit ips in:
ultimate-lighthouse.service
server.js
ecosystem.config.js
.env
--------


create folder:

mkdir litehaus



-------



copy across files:
Required Files for New Droplet:

Core Files:



litehaus (your C++ executable)

server.js (the main web server)

package.json (dependencies)

ecosystem.config.js (PM2 configuration)



Setup Files:



start-lighthouse.sh (automated deployment script)

.env (environment configuration - optional)

ultimate-lighthouse.service (systemd service - optional)



--------



sudo apt install npm



npm install



\# Make the scripts executable

chmod +x litehaus

chmod +x start-lighthouse.sh



\# Run the magical deployment script

./start-lighthouse.sh



-------



\# Set up PM2 auto-start (the command that failed)

sudo pm2 startup



\# Now start your lighthouse

pm2 start server.js --name lighthouse-web



\# Save the configuration

pm2 save



\# Check it's running

pm2 list



-------



you should now have an up n running perma litehaus!

