#!/bin/sh

mkfifo /var/log/test_file
aaa /var/log/test_file &
