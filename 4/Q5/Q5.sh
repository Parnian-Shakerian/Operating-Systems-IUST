#!/bin/bash

search_dir="."
extension="txt"

total_files=0
total_size=0

recursive_search() {
    local dir="$1"
    local ext="$2"

    for file in "$dir"/*; do
        if [ -f "$file" ] && [[ "$file" == *"$ext" ]]; then
            file_size=$(du -b "$file" | cut -f1)
            ((total_files++))
            ((total_size += file_size))
        elif [ -d "$file" ]; then
            recursive_search "$file" "$ext"
        fi
    done
}

recursive_search "$search_dir" "$extension"

echo "Total files found: $total_files"
echo "Combined size of all files: $total_size bytes"