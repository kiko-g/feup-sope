(./simpledu ~/Programs -a -S  -B 65 || echo $?) | sort -k2 > testeT_simpledu.txt
(du ~/Programs -l -a -S  -B 65 || echo $?) | sort -k2 > testeT_du.txt
diff -q testeT_simpledu.txt testeT_du.txt > /dev/null 2>&1 && echo OK || echo FAILED
