#!/bin/bash

mkdir runnable-c-only-packages  runnable-partially-c-packages  runnable-unknown-language-packages

cd /dataset-packages/runnable-c-only-packages
for i in $(cat /dataset-packages/runnable-c-only-packages.txt)
do
   echo -n "Downloading sources of $i..."
   apt-get source $i
   echo "done."
done

cd /dataset-packages/runnable-partially-c-packages
for i in $(cat /dataset-packages/runnable-partially-c-packages.txt)
do
  echo -n "Downloading sources of $i..."
  apt-get source $i
  echo "done."
done

cd /dataset-packages/runnable-unknown-language-packages
for i in $(cat /dataset-packages/runnable-unknown-language-packages.txt)
do
  echo -n "Downloading sources of $i..."
  apt-get source $i
  echo "done."
done
