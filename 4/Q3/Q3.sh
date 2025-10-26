#!/bin/bash

cpu_threshold=90.0
memory_threshold=90.0

admin_email="email.com" # Replace with the optional email address
email_subject="System Resource Alert"
email_body="CPU or memory usage has exceeded the threshold on the system."

send_email() {
    echo "$email_body" | mail -s "$email_subject" "$admin_email"
}

while true; do
    cpu_usage=$(top -bn1 | awk '/Cpu/ {print $2}' | awk -F"%" '{print $1}')
    memory_usage=$(free | awk '/Mem:/ {print $3/$2 * 100.0}')

    # Check if CPU or memory usage exceeds the thresholds
    if (( $(echo "$cpu_usage > $cpu_threshold" | bc -l) )) || (( $(echo "$memory_usage > $memory_threshold" | bc -l) )); then
        send_email
    fi

    # Sleep for 1 minute before checking again
    sleep 60
done

