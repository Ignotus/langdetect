#!/usr/bin/env bash

mkdir -p ../data

filename=enwiki-20140811-pages-articles7.xml-p000305002p000464996.bz2
if [ ! -f $filename ]
then
  wget https://dumps.wikimedia.org/enwiki/20140811/$filename
fi

bzip2 -dc $filename | python WikiExtractor.py -cb 250K -o ../data/en &


filename=afwiki-20140811-pages-articles.xml.bz2
if [ ! -f $filename ]
then
  wget https://dumps.wikimedia.org/afwiki/20140811/$filename
fi

bzip2 -dc $filename | python WikiExtractor.py -cb 250K -o ../data/af &


filename=ruwiki-20140727-pages-articles1.xml.bz2
if [ ! -f $filename ]
then
  wget https://dumps.wikimedia.org/ruwiki/20140727/$filename
fi

bzip2 -dc $filename | python WikiExtractor.py -cb 250K -o ../data/ru &


filename=ukwiki-20140722-pages-articles.xml.bz2
if [ ! -f $filename ]
then
  wget https://dumps.wikimedia.org/ukwiki/20140722/$filename
fi

bzip2 -dc $filename | python WikiExtractor.py -cb 250K -o ../data/uk &


filename=dewiki-20140725-pages-articles3.xml.bz2
if [ ! -f $filename ]
then
  wget https://dumps.wikimedia.org/dewiki/20140725/$filename
fi

bzip2 -dc $filename | python WikiExtractor.py -cb 250K -o ../data/de &


filename=plwiki-20140802-pages-articles3.xml.bz2
if [ ! -f $filename ]
then
  wget https://dumps.wikimedia.org/plwiki/20140802/$filename
fi

bzip2 -dc $filename | python WikiExtractor.py -cb 250K -o ../data/pl &


filename=bgwiki-20140728-pages-articles.xml.bz2
if [ ! -f $filename ]
then
  wget https://dumps.wikimedia.org/bgwiki/20140728/$filename
fi

bzip2 -dc $filename | python WikiExtractor.py -cb 250K -o ../data/bg &

