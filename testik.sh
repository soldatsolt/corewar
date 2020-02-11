#! /bin/bash

FILES=tests/asm/*/*.s
MY_ASM="./asm"
MOTHERS_FRIEND_SON="./vm_champs/asm"
TRACES="./traces"

TEST_NUMBER=1
rm -rf $TRACES
mkdir -p $TRACES

for file in $FILES
do
	echo "$TEST_NUMBER: $file:"
	$MY_ASM $file > /dev/null
	$MOTHERS_FRIEND_SON $file > /dev/null
	CORFILE=${file%.s}.cor
	echo $CORFILE
	if [ -f $CORFILE -a -f MYFILE ]
	then
		if test -z "$(diff -U3 MYFILE $CORFILE)"
		then
			echo "\x1b[32;1m >>>>    IT IS OKAY !!!!!  \x1b[0m"
		else
			echo "\x1b[31;1m >>>>    EXECUTE ERROR !!!!!  \x1b[0m"
			hexdump -vC MYFILE > $TRACES/${TEST_NUMBER}_my.hd
			hexdump -vC $CORFILE > $TRACES/${TEST_NUMBER}_or.hd
			diff -U3 $TRACES/${TEST_NUMBER}_my.hd $TRACES/${TEST_NUMBER}_or.hd > $TRACES/${TEST_NUMBER}.trace
			rm $TRACES/${TEST_NUMBER}_my.hd $TRACES/${TEST_NUMBER}_or.hd
		fi
	else
		if [ -f MYFILE ]
		then
			echo "\x1b[31;1m >>>>    VALID ERROR !!!!! \x1b[0m"
		else
			echo "\x1b[32;1m >>>>    IT IS OKAY !!!!! \x1b[0m"
		fi
	fi
	rm MYFILE
	((TEST_NUMBER += 1))
	echo
done
