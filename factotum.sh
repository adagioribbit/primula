#!/bin/sh

# Google sheets : maximum of 5 million cells over 18 278 columns
# echo "pouet" >> file_to_append_new_line_to.txt

max=1000
for i in `seq 101 $max`
do
	./sieve -f $i -p >> factotum.txt
done
