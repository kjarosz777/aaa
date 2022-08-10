#!/bin/sh

insmod /lib/modules/5.15.54-yocto-standard/extra/aaa_kernel.ko bufferSize=32
mkfifo /var/log/test_file
aaa /var/log/test_file &
