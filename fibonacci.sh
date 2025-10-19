#!/bin/bash
#
#fibonacci.sh : 피보나치 수열을 출력하는 스크립트

#초기값 설정
a=0
b=1
N=10

echo "피보나치 수열(첫 $N 개 항):"
for((i=0; i<N; i++))
do
	echo -n "$a "
	fn=$(expr $a + $b)
	a=$b
	b=$fn
done
echo""
