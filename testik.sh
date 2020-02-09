#! /bin/bash

./asm 42.s
./vm_champs/asm 42.s

if [ $(diff -U3 MYFILE 42.cor) ];
then
echo "\x1b[31;01mTHEY ARE DIFFERENT"
else
echo "\x1b[32;01m[[[[[IT IS OKAY]]]]]"
fi