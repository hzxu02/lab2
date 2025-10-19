#include "calc.h"

//devide.c : 나눗셈 함수 구현

double divide(double a, double b){
        //0으로 나누는 경우 방지 
	if(b==0){
		return 0.0;
	}
	return a/b;
}

