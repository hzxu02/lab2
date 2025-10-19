#!/bin/bash
#
#count.sh : 현재 디렉토리의 파일 개수를 출력하는 스크립트

# 'ls -l'의 출력 결과에서 첫 글자가 '-'로 시작하는 행(파일)만 grep으로 필터링한 후,
# 'wc -l'을 통해 그 행의 수를 카운트 


COUNT=$(ls -l | grep '^-' | wc -l)

echo " 현재 디렉토리의 파일 개수 : $COUNT"
