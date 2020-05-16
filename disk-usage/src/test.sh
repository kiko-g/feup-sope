
for (( i=1; i <= 1024; ++i ))
do
	(./simpledu ~/Documents/feup/3o_ano -a -B $i || echo $?) | sort -k2 > testeT_simpledu.txt
	(du ~/Documents/feup/3o_ano -l -a -B $i || echo $?) | sort -k2 > testeT_du.txt
	echo $i
	diff -q testeT_simpledu.txt testeT_du.txt > /dev/null 2>&1 && echo OK || echo FAILED
done
