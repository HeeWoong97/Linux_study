#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void test_func(void) {
    printf("this is a test function!(pid %d)\n", getpid());
}

int main(int argc, char** argv) {
    pid_t pid;
    
    printf("origin process pid: %d\n", getpid());

    pid = fork();
    /* 이 이후부터 parent, child process는 각자의 코드를 실행하게 된다
       parent process의 pid가 먼저 반환되므로 parent process부터 실행 */
    if (pid == 0) {
	/* child process가 실행하는 부분 */
	printf("child process pid: %d\n", getpid());
    }
    else if (pid > 0) {
	/* parent process가 실행하는 부분 */
	printf("parent process pid: %d, child process pid: %d\n", getpid(), pid);
    }
    test_func();

    /* 
	프로그램을 이렇게 끝내면 안된다
	parent process가 child process를 정리해줘야됨
	그렇지 않으면 child process는 좀비 프로세스가 된다
     */

    return 0;
}
