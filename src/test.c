#include <stdio.h>

int main(){
    int numArr[5] = {11, 22, 33, 44, 55};
    int *numPtrA;
    void *ptr;

    numPtrA = &numArr[2];
    ptr = numArr;
    
    //55 출력
    // numPtrA의 주소를 증가시켜서 값 출력
    numPtrA+=2;
    printf("%d \n", *numPtrA);
    
    // 22 출력
    // void ptr를 int 형 포인터로 바꿔주고, 주소를 증가시켜서 값 출력
    printf("%d \n", *(int *)ptr + 1);

}