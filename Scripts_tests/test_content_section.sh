#!/bin/sh

# Tests unitaires sur le contenu d'une section

error="NOTE : Cette section a des réadressages mais ils n'ont PAS été appliqués à cette vidange." 
difference="difference.txt"
tmp1="tmp1.txt"
tmp2="tmp2.txt"
tmp3="tmp3.txt"
options=$1
nom_section=$2
files="Examples_loader/*"
make > $tmp1
for file in $files
do  
    ./readelf $options $nom_section $file > $tmp1 
    readelf $options $nom_section $file > $tmp2 
    if [ $? -eq 0 ]
    then
        readelf $options $nom_section $file > $tmp2
    else
        readelf $options $nom_section $file 2> $tmp2 
    fi

    while read ligne
	do
		if [ "$ligne" != "$error" ] 
		then
				echo " $ligne" >> $tmp3
		fi
	done < $tmp2

    diff -bB $tmp1 $tmp3 > $difference 
    NB_LIGNES=$(wc -l $difference | cut -f1 -d' ') 

    if [ $NB_LIGNES -eq 0 ] 
    then 
        echo $file:"Test succès" 
    else 
        echo $file:"Test échoué" 
    fi
    
    rm  $tmp1 $tmp2 $tmp3 $difference  2> /dev/null


done
