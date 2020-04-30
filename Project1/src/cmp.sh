(./simpledu ~/Documents/ -a -B 16 --max-depth=2 || echo $?) | sort -k2 > testeT_simpledu.txt
(du ~/Documents/ -a -B 16 -l --max-depth=2 || echo $?) | sort -k2 > testeT_du.txt
diff -q testeT_simpledu.txt testeT_du.txt > /dev/null 2>&1 && echo OK || echo FAILED
#compare du with simpledu