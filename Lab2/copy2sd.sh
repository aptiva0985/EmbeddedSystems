#!/bin/bash
# 
sudo losetup /dev/loop1 ./sdcard.img
sudo kpartx -a /dev/loop1
sudo mount /dev/mapper/loop1p1 /media/bootfs

for var
do
   sudo cp $var /media/bootfs
done
sync;sync;sync
sudo umount /media/bootfs
sudo kpartx -d /dev/loop1
sudo losetup -d /dev/loop1
