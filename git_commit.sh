#!/bin/bash

git add .
git commit -m "${1:-No message provided}"
git push

exit
