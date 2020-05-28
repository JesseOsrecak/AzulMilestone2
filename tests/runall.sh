#! /usr/bin/bash

testfiles=*.in

for file in $testfiles
do
  test=$(basename $file .in)
  echo "Trying: " $test
  ../azul < ./$test.moves > /dev/null
  echo $(diff -s ./$test.exp lasttest.out)
done
