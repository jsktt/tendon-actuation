#!/bin/bash

apt-get update
apt-get upgrade -y

# Basic system installs
apt-get install -y build-essential gparted htop curl python3.8 python3.8-dev python3-pip

# Set  timezone
timedatectl set-timezone America/New_York

# Set locales
sudo update-locale LANG=en_US.UTF-8 LANGUAGE=en_US LC_ALL=en_US.UTF-8
echo 'export LC_ALL=en_US.UTF-8' >> ~/.profile
echo 'export LANG=en_US.UTF-8' >> ~/.profile
echo 'export PATH=$PATH:$HOME/.local/bin' >> ~/.profile
source ~/.profile

# Install VSCode and PlatformIO
curl -L https://aka.ms/linux-arm64-deb > code_arm64.deb
sudo apt install ./code_arm64.deb -y
rm code_arm64.deb
code --install-extentsion platformio.platformio-ide

# Add PlatformIO CLI to PATH
sudo mkdir -p /usr/local/bin
sudo ln -s ~/.platformio/penv/bin/platformio /usr/local/bin/platformio
sudo ln -s ~/.platformio/penv/bin/pio /usr/local/bin/pio
sudo ln -s ~/.platformio/penv/bin/piodebuggdb /usr/local/bin/piodebuggdb

# Create Python symlink
ln -s /usr/bin/python3.8 /usr/bin/py3

# Install VNC to launch at startup
apt install xfce4 xfce4-goodies -y
apt install tightvnc

# Install FTP server to launch at startup

# Upgrade PIP
py3 -m pip install --upgrade pip

# Add ${USER} to dialout group (requires restart)
sudo usermod -aG dialout ${USER}

# Prompt for github credentials

# Download batbot code from github repository

# Install batbot repo dependencies

# Upgrade python packages
