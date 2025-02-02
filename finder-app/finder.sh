#!/bin/sh

if [ $# -lt 2 ]
then
  echo "Two parameters required, file directory and search string."
  exit 1
else
  if [ -z "$1" ]; then
    echo "First parameter required, please specify a file directory."
    exit 1
  fi

  if [ -z "$2" ]; then
    echo "Second parameter required, please specify a search string."
    exit 1
  fi
fi

filesdirectory=$1

if [ ! -d "$filesdirectory" ]; then
  echo "Directory does not exist."
  exit 1
fi

search_str=$2

function skip() {
  # shellcheck disable=SC2005
  echo "$(find "$filesdirectory" -type f | wc -l)"
}

function lastcall() {
  # declare -i output=0
  output=0
  find "$filesdirectory" -type f -print0 | while read -r -d $'\0' file
  do
    count=$(grep -c "$search_str" "$file")
    # declare -i num_count="$count"


    output=$((output + count))
    echo "$output"
  done
}

num_files=$(skip)
matches_in_files=$(lastcall | tail -n 1)

echo "The number of files are $num_files and the number of matching lines are $matches_in_files"

exit 0
