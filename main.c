#include <stdio.h>

int a(float e)
{
    return (int)e;
}

int main(){
    float f = 0.5f;
    printf("%d",a(f));
    return 0;
}