SUM=`sh sum_counter.sh`

sh mark_counter.sh | while read -r line ; do echo -ne $line"\n"$SUM".0\n" | paste -sd/ | bc -l; done 

