#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <sys/types.h>

#define MAXCPU 4
static void print_cpuset(cpu_set_t* mask) {
    printf("available CPUs: ");
    for (int i = 0; i < MAXCPU; i++) {
	if (CPU_ISSET(i, mask)) { //mask의 i번째 cpu를 사용할 수 있다면
	    printf("CPU%d ", i);
	}
    }
    printf("\n");
}

int main(int argc, char** argv) {
    int cur_cpu;
    cpu_set_t mask;

    /* 어떤 cpu를 사용할 수 있는가? 
       -> mask에 저장 */
    if (sched_getaffinity(getpid(), sizeof(cpu_set_t), &mask)) {
	perror("sched_getaffinity()\n");
	return -1;
    }
    print_cpuset(&mask);

    cur_cpu = sched_getcpu();
    printf("process is running on CPU%d\n", cur_cpu); //현재 어느 cpu에서 동작하는가
    
    CPU_ZERO(&mask); //cpu set 초기화
    for (int i = 0; i < MAXCPU; i++) {
	CPU_SET(i, &mask); //i번째 CPU로 설정
	if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &mask)) { //설정한 CPU에서 프로세스 실행
	    perror("sched_setaffinity()\n");
	    return -1;
	}
	
	sleep(2);

	cur_cpu = sched_getcpu();
	printf("Now, run on CPU%d\n", cur_cpu);

	CPU_ZERO(&mask); //cpu set 초기화
    }

    return 0;
}
