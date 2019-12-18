#!/bin/sh
example="Examples_loader/*"
readelf -h Examples_loader/example1.o > script_test/tmp1
if [ $# -eq 0 ] 
then
	echo "Vous arguments sont pas suffisants pour exécuter le script"
	exit 0
else
	rm script_test/tmp0.txt script_test/tmp1.txt
	if [ $1 = "-h" ]
	then
		for file in $example
		do
			./readelf -h $file > script_test/tmp0.txt
			readelf -h $file > script_test/tmp1.txt
			if diff script_test/tmp0.txt script_test/tmp1.txt > /dev/null
			then 
				echo "$file success"
			else
				echo "$file error"
			fi
		done
	elif [ $1 = "-s" ]
	then
		for file in $example
		do
			./readelf -s $file > script_test/tmp0.txt
			readelf -s $file > script_test/tmp1.txt
			if diff script_test/tmp0.txt script_test/tmp1.txt > /dev/null
			then 
				echo "$file success"
			else
				echo "$file error"
			fi
		done
	else
		echo "l'argument doit etre -h | -s "
	fi
		
	
fi
