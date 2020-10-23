#!/bin/bash

TYPES=("runnable-c-only-packages" "runnable-partially-c-packages" "runnable-unknown-language-packages")
cd /dataset-packages/

function download_source() {
	FOLDER=$1
	FILE=$1.txt
	cnt=0
	total=$(wc -l $FILE | awk '{print $1}')

	mkdir $FOLDER
	pushd $FOLDER

	for pname in $(cat ../$FILE);
	do
		echo \($cnt/$total\) $FOLDER/$pname
		cnt=$(($cnt+1))

		apt-get source $pname 2>/dev/null 1>/dev/null
	done

	popd
}


for i in ${TYPES[@]};
do
	download_source $i
done
