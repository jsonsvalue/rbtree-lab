#include <stdio.h>

int main(){

    // 이차원 배열을 생성
    // 이차원 배열 각 원소의 첫 주소를 생성
    char* str[2];
    str[0] = "hello!";
    str[1] = "jungler";
    

    // 출력하고자 하는 첫 문자열에서 마지막 문자열까지 보여주는 것
    printf("%s \n", str[0] + 1);
    
    // 이차원 배열 두 번째 배열의 첫 주소에서 2개 주소 떨어진 곳에서부터 출력 
    printf("%s \n", (str+1)[0] +2);
    

}