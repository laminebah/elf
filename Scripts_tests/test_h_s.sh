#!/bin/sh
example="Examples_loader/*"
readelf -h Examples_loader/example1.o > Scripts_tests/tmp1
if [ $# -eq 0 ] 
then
	echo "Vous arguments sont pas suffisants pour exécuter le script"
	exit 0
else
	if [ $1 = "-h" ]
	then
		echo "*********Test entete ELF************"
		for file in $example
		do
			if [ -d $file ]
			then
				echo "$file -- error"
			else
				./readelf -h $file > Scripts_tests/tmp0.txt
				readelf -h $file > Scripts_tests/tmp1.txt
				if diff Scripts_tests/tmp0.txt Scripts_tests/tmp1.txt > /dev/null
				then 
					echo "$file -- success"
				else
					echo "$file -- error"
				fi
			fi
		done
	elif [ $1 = "-s" ]
	then
		echo "********Test table des symboles ELF********"
		for file in $example
		do
			if [ -d $file ]
			then
				echo "$file -- error"
			else 
				./readelf -s $file > Scripts_tests/tmp0.txt
				readelf -s $file > Scripts_tests/tmp1.txt
				if diff Scripts_tests/tmp0.txt Scripts_tests/tmp1.txt > /dev/null
				then 
					echo "$file -- success"
				else
					echo "$file -- error"
				fi
			fi
		done
	elif [ $1 = "-S" ]
	then
		echo "*********Test des entêtes de section***********"
		for file in $example
		do
			if [ -d $file ]
			then
				echo "$file -- error"
			else
				./readelf -S $file > Scripts_tests/tmp0.txt
				readelf -S $file > Scripts_tests/tmp1.txt
				if diff Scripts_tests/tmp0.txt Scripts_tests/tmp1.txt > /dev/null
				then 
					echo "$file -- success"
				else
					echo "$file -- error"
				fi
			fi
		done
	fi
	
	rm Scripts_tests/tmp0.txt Scripts_tests/tmp1.txt
fi
