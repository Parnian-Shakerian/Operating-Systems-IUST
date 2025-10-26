#!/bin/bash

input_file="websites.txt"  # input file
output_file="modified_websites.txt"  # output file

while IFS= read -r website; do
    modified_website=$(echo "$website" | sed 's/\.ir$/.com/')
    echo "$modified_website" >> "$output_file"
done < "$input_file"

echo "Website addresses modified and saved to $output_file."
