#!/bin/bash
cd /repo/packaging/arch
pacman -Sy --noconfirm --needed base-devel fuse fuse3 attr git
useradd user -u 1000
chown user -R ../..
sudo -u user makepkg --skipinteg --skipchecksums --skippgpcheck
rm -rf pkg src
mv $(ls -1 | grep cicpoffs-git- | head -1) cicpoffs.tar.zst