#!/usr/bin/env bash

#xorg
sudo apt install -y xorg xbacklight xbindkeys xvkbd xinput

#basic essentials
sudo apt install -y build-essential

#intel microcode
sudo apt install -y intel-microcode
#AMD microcode
#sudo apt install -y amd64-microcode

#network manager
#change managed to true in /etc/NetworkManager/NetworkManager.conf
#after desktop install follow the switching to IWD guide on debian.org
sudo apt install -y network-manager network-manager-gnome gnome-control-center

#appearance management
sudo apt install -y lxappearance

#file manager
sudo apt install -y thunar thunar-archive-plugin policykit-1-gnome
#terminal
sudo apt install -y kitty

#network file tools/system events
sudo apt install -y dialog mtools dosfstools avahi-daemon acpi acpid gvfs-backends
sudo systemctl enable avahi-daemon
sudo systemctl enable acpid

#sound
#add execution on startup: exec --no-startup-id i3-msg 'exec /usr/bin/pulseaudio --start'
sudo apt install -y pulseaudio alsa-utils pavucontrol volumeicon-alsa

#i3
sudo apt install -y i3-wm

#lightdm
sudo apt install -y lightdm lightdm-gtk-greeter-settings

#media codes and tools
sudo apt-add-repository non-free #add contrib and non-free
sudo apt install -y libavcodec-extra vlc

#utils
sudo apt install -y neofetch htop curl python3 python3-pip gnome-disk-utility shotwell
sudo apt install -y maim unzip timeshift zathura
#notifications
sudo apt install -y dunst libnotify-bin
#application runners
sudo apt install -y rofi drun
#compositor
sudo apt install -y picom

#printing and bluetooth
sudo apt install -y cups system-config-printer simple-scan
sudo apt install -y bluez blueman
sudo systemctl enable cups
sudo systemctl enable bluetooth

#office suite
sudo apt install -y libreoffice libreoffice-gtk3

#browser
sudo curl -fsSLo /usr/share/keyrings/brave-browser-archive-keyring.gpg https://brave-browser-apt-release.s3.brave.com/brave-browser-archive-keyring.gpg
echo "deb [signed-by=/usr/share/keyrings/brave-browser-archive-keyring.gpg] https://brave-browser-apt-release.s3.brave.com/ stable main"|sudo tee /etc/apt/sources.list.d/brave-browser-release.list
sudo apt update
sudo apt install -y brave-browser

#spotify
curl -sS --insecure https://download.spotify.com/debian/pubkey_7A3A762FAFD4A51F.gpg | sudo gpg --dearmor --yes -o /etc/apt/trusted.gpg.d/spotify.gpg
echo "deb http://repository.spotify.com stable non-free" | sudo tee /etc/apt/sources.list.d/spotify.list
sudo apt-get update && sudo apt-get install -y spotify-client

#download fonts
sudo apt install -y fonts-recommended fonts-ubuntu fonts-font-awesome fonts-terminus
#also get nerdfonts: FiraCode, FiraMono

printf "REMINDER: Install framework drivers!!"
printf "You can reboot now!"
