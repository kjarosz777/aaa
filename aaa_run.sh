#!/bin/sh

insmod /lib/modules/5.15.54-yocto-standard/extra/aaa_kernel.ko
mkfifo /var/log/test_file
aaa /var/log/test_file &
