#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

// declare variables to print later
int data = 0x8763;
int bss = 0;
int *heap;  // initialized in main
__thread long tls;
void text() {}
void *lib;  // initialized in main
int *tmp; // initialized in main

// thread data
typedef struct {
	int id;
	int sleep;
} task_t;

long get_phy_addr(void *p) {
	return syscall(8788, p);
}

typedef struct {
	uint64_t start;
	uint64_t end;
	uint64_t perm;
	char name[0x100];
} segment_t;

int get_segment(void *ptr, segment_t *out) {
	return syscall(8789, ptr, out);
}
void print_segment(segment_t *seg) {
	printf("%llx-%llx %llu %s\n", seg->start, seg->end, seg->perm, seg->name);
}

typedef struct {
	const char *name;
	void *addr;
} child_info_t;

int shm_get()
{
	int fd = open("./t.txt", O_RDWR );
	if(fd<0){
		printf("Failed to Open File\n");
		return 0;
	}
	tmp = mmap ( NULL, 10*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
	if(tmp == MAP_FAILED){
		printf("Mapping Failed\n");
		return 0;
	}
	*tmp=87638763;

	printf("\n");
	return 1;
}

void *child(){
	long stack;

	printf("tmp=%i\n",*tmp);

	child_info_t addrs[] = {
	    {"stack", &stack}, {"tls", &tls},   {"data", &data}, {"bss", &bss},
	    {"heap", heap},    {"text", &text}, {"lib", lib}, {"mmap", tmp}
	};
	for (int i = 0; i < sizeof(addrs) / sizeof(addrs[0]); i++) {
		segment_t seg;
		printf("%s addr=%p phy=%p\n", addrs[i].name, addrs[i].addr,
		       (void *)get_phy_addr(addrs[i].addr));
		get_segment(addrs[i].addr, &seg);
		printf("\t");
		print_segment(&seg);
	}
	return NULL;
}

const int MAX_THREAD = 2;

int main() {
	heap = malloc(0x10);
	lib = &printf;
    	if(!shm_get()){
		printf("Mmap failed");
	}
	pid_t pid;
	pid = fork();
	if (pid == 0) {
		sleep(1);
		printf("Child:\n");
		child();
	} else if (pid > 0) {
		printf("Parent:\n");
		child();
		wait(0);
	} else {
		printf("Error!n");
	}

	return 0;
}
