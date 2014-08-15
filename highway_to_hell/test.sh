#!/usr/bin/env bash

rm -rf testset
mkdir testset

cp ../test.zip testset
pushd testset
  unzip test.zip
  rm test.zip
popd

correct=0
processed=0
for file in testset/*.txt;
do
  let processed=processed+1
  LAN1=$(echo $file | sed 's/[\/-]/ /g' | awk '{print tolower($2)}')
  LAN2=$(./trigram_calc -detect-one-answer $file)
  echo $LAN1 $LAN2
  if [ "$LAN1" == "$LAN2" ]
  then
    let correct=correct+1
  fi
  echo "$correct/$processed" | bc -l
done

echo "Result: "
echo "$correct/1000" | bc -l
