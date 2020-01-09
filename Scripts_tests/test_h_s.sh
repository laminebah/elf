#!/bin/sh

TesTop () {

	if [ $1 = "-h" ]
	then
		echo "*********Test entete ELF********************"
	elif [ $1 = "-s" ]
	then
		echo "*********Test table des symboles ELF********"
	elif [ $1 = "-S" ]
	then
		echo "*********Test des entêtes de section********"
	elif [ $1 = "-r" ]
	then
		echo "*********Test relocation table *************"
	elif [ $1 = "-x" ]
	then
		echo "----------- TEST CONTENT SECTION ---------"
	fi
}

example="Examples_loader/*"
readelf -h Examples_loader/example1.o > Scripts_tests/tmp1
if [ $# -eq 0 ] 
then
	echo "Vous arguments sont pas suffisants pour exécuter le script"
	exit 0
else
	TesTop $1
	if [ $1 = "-x" ]
	then
		if [ $# -eq 2 ]
		then
			./Scripts_tests/test_content_section.sh -x $1
		else
			echo "Donner un nom ou un numero de section "
		fi 
	else
		for file in $example
		do
				if [ -d $file ]
				then
					echo "$file -- error"
				else 
						if [ $1 != "-x" ]
						then
							./readelf $1 $file > Scripts_tests/tmp0.txt
							readelf $1 $file > Scripts_tests/tmp1.txt
							nbl1=$(wc -l Scripts_tests/tmp0.txt | cut -d ' ' -f1)
							if diff Scripts_tests/tmp0.txt Scripts_tests/tmp1.txt > /dev/null
							then 
								if [ $nbl1 -ne 0 ]
								then
										echo "$file -- succes"
								else
										echo "$file --error"
								fi
							else
								echo "$file -- error"
							fi
							
						fi
					
				fi
		done
	fi
fi	
