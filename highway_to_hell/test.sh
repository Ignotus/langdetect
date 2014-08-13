#!/usr/bin/env sh

rm -rf test
mkdir -p test
cd test
unzip ../../test.zip

cd ..

while read -r line ; do
  ./start.sh test/$line;
done << $(ls -l test | grep -v "^d" | tail -n +2| awk '{print $9}' )

