text=$(sh word_counter.sh)

echo -e $text | awk '{print $2}' | paste -sd+ | bc
echo -e $text | awk '{print $3}' | paste -sd+ | bc
echo -e $text | awk '{print $4}' | paste -sd+ | bc
echo -e $text | awk '{print $5}' | paste -sd+ | bc
echo -e $text | awk '{print $6}' | paste -sd+ | bc
echo -e $text | awk '{print $7}' | paste -sd+ | bc
echo -e $text | awk '{print $8}' | paste -sd+ | bc

