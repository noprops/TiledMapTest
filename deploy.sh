#!/bin/sh

SCRIPT_DIR=`dirname $0`
cd $SCRIPT_DIR

message=$(date)
if [ $# -eq 1 ]; then
	message=$1
fi

git add .
git commit -m "${message}"
git push

