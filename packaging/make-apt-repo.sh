#!/bin/bash
# build a flat apt repo under ./apt-repo from *.deb in cwd
set -e
mkdir -p apt-repo/pool apt-repo/dists/stable/main/binary-amd64
cp ./*.deb apt-repo/pool/
cd apt-repo
dpkg-scanpackages --arch amd64 pool /dev/null > dists/stable/main/binary-amd64/Packages
gzip -9c dists/stable/main/binary-amd64/Packages > dists/stable/main/binary-amd64/Packages.gz
cat > dists/stable/Release <<REL
Origin: msr-sck
Suite: stable
Codename: stable
Architectures: amd64
Components: main
Date: $(date -Ru)
REL
printf '<h1>msr-sck apt repository</h1><p>See <a href="https://github.com/SkyWalkerAMD/msr-sck">GitHub</a> for install instructions.</p>' > index.html
echo "apt repo ready under apt-repo/"
