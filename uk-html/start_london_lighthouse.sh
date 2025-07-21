#!/bin/bash
cd /home/wofl/ultimate_lighthouse/build
tmux new-session -d -s lighthouse "./ultimate_public_lighthouse"
sleep 5
cd /home/wofl
tmux new-session -d -s lighthouse-api "python3 london_lighthouse_api.py"
