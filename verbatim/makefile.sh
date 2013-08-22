#!/usr/bin/sh

infile="$1"

cpp "$infile" | fgrep "=" | sed -e "s/^ *//g; s/, *//g;"


