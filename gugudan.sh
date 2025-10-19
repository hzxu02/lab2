#!/bin/bash
#
#gugudan.sh : for, while, until 반복문을 사용하여 구구단 출력하는 스크립트


echo "-- for 문 구구단 2단 출력 --"
for((i=1; i<=9; i++))
do
	echo "2*$i=$((2*i))"
done


echo "" 

echo "--- while 문으로 구구단 3단 출력 ---"
j=1
while [ $j -le 9 ]; do
    echo "3 * $j = $((3*j))"
    let j++
done

echo "" # 줄 바꿈

echo "--- until 문으로 구구단 4단 출력 ---"
k=1
until [ $k -gt 9 ]; do
    echo "4 * $k = $((4*k))"
    let k++
done
