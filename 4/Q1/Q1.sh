#!/bin/bash

while true; do
    if ping -c 1 8.8.8.8 &> /dev/null; then
        echo "$(date) - Server is up" >> ping.log
    else
        echo "$(date) - Server is down" >> ping.log
        sleep 5
    fi
done

