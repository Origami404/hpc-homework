#!/usr/bin/bash

while true 
do
    xmake project -k compile_commands build
    sleep 60
done