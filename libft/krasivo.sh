#!bin/bash

i=0
sp=( Loading. lOading.. loAding... loaDing.... loadIng..... loadiNg...... loadinG....... DONE)
echo ' '
for i in "${sp[@]}\n"
do
    printf "\r\t\t\t\t\033[K\033[32m$i\033[m"
    sleep 0.2
done