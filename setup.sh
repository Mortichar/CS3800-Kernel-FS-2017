#!/bin/bash

if [[ $(whoami) != "root" ]]; then
	echo "Please run this script as root, e.g.,"
	echo "sudo $0"
	exit 1
fi

echo "deb http://ftp.us.debian.org/debian testing main" > /etc/apt/sources.list.d/testing.list
cat > /etc/apt/preferences.d/testing <<EOF
Package: *
Pin: release a=testing
Pin-Priority: 100
EOF

apt update -y
apt install -y -t testing gcc gdb qemu
