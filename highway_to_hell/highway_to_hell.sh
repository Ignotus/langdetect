sh result.sh > res
echo "{"$(paste -d : name res | xargs)"}" |sed 's/ /, /g'

rm res

