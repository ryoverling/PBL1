#include <stdio.h>
int main() {
    int a, b;
    printf("Nhap so a: ");
    scanf("%d", &a);
    printf("Nhap so b: ");
    scanf("%d", &b);
    
    int sum = a + b;
    printf("Tong cua %d va %d la: %d\n", a, b, sum);
    
    return 0;
}