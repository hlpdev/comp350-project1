#!/bin/bash

if [ "$#" -eq 3 ]; then
  ./countmaster "$1" "$2" "$3"
elif [ "$#" -eq 2 ]; then
  ./countprimes "$1" "$2"
else
  echo "You must provide a range start, end, and an optional parallel process count!"
  echo "usage: $0 [start] [end] (proc_count)"
  exit 1
fi
