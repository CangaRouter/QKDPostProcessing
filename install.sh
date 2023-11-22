#!/bin/bash

# Update package list
sudo apt-get update

# Install necessary dependencies
sudo apt-get install -y build-essential wget libboost-dev libboost-system-dev libboost-thread-dev libevent-dev librabbitmq-dev git python3 python3-pip libssl-dev snapd

sudo snap install cmake --classic

# Clone AMQP-CPP repository and build
git clone https://github.com/CopernicaMarketingSoftware/AMQP-CPP.git
cd AMQP-CPP
mkdir build
cd build
cmake .. -DAMQP-CPP_BUILD_SHARED=ON -DAMQP-CPP_LINUX_TCP=ON
cmake --build . --target install

# Install Python dependencies
pip3 install --no-cache-dir -r requirements.txt

# Run ldconfig
sudo ldconfig

# Build error_correction and privacy_amplification
cd error_correction
cmake .
make
cd ..
cd privacy_amplification
cmake .
make
