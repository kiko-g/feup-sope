(./simpledu ~/Documents/feup/3o_ano -a --max-depth=0 || echo $?) | sort -k2 > testeT_simpledu.txt
(du ~/Documents/feup/3o_ano -l -a --max-depth=0 || echo $?) | sort -k2 > testeT_du.txt
diff -q testeT_simpledu.txt testeT_du.txt > /dev/null 2>&1 && echo OK || echo FAILED
