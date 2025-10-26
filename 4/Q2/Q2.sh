#!/bin/bash

echo "Enter the filename."
read filename

if [ ! -f "$filename" ]; then
    echo "Error: file not found."
    exit 1
fi

echo "Please enter your choice (1 or 2)"
echo "1. Count the number of characters in the file."
echo "2. Replace characters in the file."
read choice

case "$choice" in
    1)
        count=$(cat $filename | wc -c)
        echo "The file contains of $count characters"
        ;;
    2)
        echo "Enter your character to replace"
        read oldchar
        echo "Enter the new character"
        read newchar
        sed -i "s/$oldchar/$newchar/g" $filename
        echo "operasion done successfully"
        ;;
    *)
        echo "Invalid choice."
        ;;
esac