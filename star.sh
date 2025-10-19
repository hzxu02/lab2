#!/bin/bash
#
#star.sh : 별 찍기를 출력하는 스크립트

#별 찍는 줄 개수
for((i=5; i>=1; i--))
do
	#별이 찍히는 개수
	for((j=1; j<=i; j++))
	do
		echo -n "*" #-n 옵션으로 줄바꿈 없이 별 찍기
	done
	echo "" #한 줄 별 찍기가 끝나면 줄 바꿈
done

