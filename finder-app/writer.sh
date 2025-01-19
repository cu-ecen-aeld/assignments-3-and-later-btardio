#!/bin/bash

if [ $# -lt 2 ]
then
  echo "Two parameters required, file and write string."
  exit 1
else
  if [ -z "$1" ]; then
    echo "First parameter required, please specify a file."
    exit 1
  fi

  if [ -z "$2" ]; then
    echo "Second parameter required, please specify a string to write to file."
    exit 1
  fi
fi

file=$1

#if [ -f "$file" ]; then
#  echo "File exists, refusing to overwrite."
#  exit 1
#fi
directory=$(dirname "$file")
mkdir -p "$directory"

touch "$file"
echo "$2" > $file