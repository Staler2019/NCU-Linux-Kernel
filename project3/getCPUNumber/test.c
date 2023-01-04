#include <dlfcn.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#define N_THREADS 30

int get_CPU_number() {
	return syscall(8790);
}
void* child(void *p){
    // sleep(2);
    while(1){}  // need to manually send SIGINT to stop
}
int main(){
    pthread_t thread[N_THREADS];
    for(int i=0;i<N_THREADS;i++)
        pthread_create(&thread[i], NULL, child, NULL);
    sleep(1);
    printf("%d\n", get_CPU_number());
    for(int i=0;i<N_THREADS;i++)
        pthread_join(thread[i], NULL);
    printf("%d\n", get_CPU_number());
    return 0;
}

// gcc test.c -m32 -lpthread && ./a.out
