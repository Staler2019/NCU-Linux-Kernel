#include <unistd.h>
#include <stdio.h>

int main() {
    int ret = syscall(8787);
    printf("%d\n", ret);
    return 0;
}

// gcc -m32 test_helloworld.c -o test_helloworld.out && ./test_helloworld.out && dmesg | tail
