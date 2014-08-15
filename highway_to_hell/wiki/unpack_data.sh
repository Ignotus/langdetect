#!/usr/bin/env sh

rm -rf af bg de en pl ru uk
unzip dataset.zip

for folder in af bg de en pl ru uk
do
    pushd $folder
        bzip2 -d *.bz2
        for file in wiki_*
        do
            mv $file "$folder-$file.txt"
        done
        mv * ../
    popd
done

rm -rf af bg de en pl ru uk
rm *.bz2
mkdir data
mv *wiki* data
