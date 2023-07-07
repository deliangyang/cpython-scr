#include <stdio.h>


int main(int argc, char* argv[]) {
    int a[10][2];
    int i;
    for (i = 0; i < 10; i++) {
        a[i][0] = 1;
        a[i][1] = 2;
        a[i][2] = 3;
    }
    printf("%d\n", *((*a)+1)); 
    return 0;
}