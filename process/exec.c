#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void test_func(void) {
    printf("this is a test function!(pid %d)\n", getpid());
}

int main(int argc, char** argv) {
    pid_t pid;
    int exit_status;

    printf("origin process pid: %d\n", getpid());

    pid = fork();

    /* 이 이후부터 parent, child process는 각자의 코드를 실행하게 된다
       parent process의 pid가 먼저 반환되므로 parent process부터 실행 */
    if (pid == 0) {

	/* child process가 실행하는 부분 
	   -> ls -al를 실행 */
	printf("child process pid: %d\n", getpid());

	/* 명령어의 절대경로 확인법
	   -> shell상에서 which ls
	   실행파일의 첫번째 parameter는 실행파일의 이름
	   -> 파일에 전달되는 argv의 첫번째 요소는 파일의 이름이다
	   마지막 매개변수는 NULL로 끝낸다 */
	if (execl("/bin/ls", "ls", "-al", "/tmp", NULL) == -1) {
	    printf("execl() fail\n");
	    return -1;
	}
    }
    else if (pid > 0) {
	/* parent process가 실행하는 부분 */
	printf("parent process pid: %d, child process pid: %d\n", getpid(), pid);
    }
    test_func();

    pid = wait(&exit_status); //어떤 프로세스가 종료되었는지 반환
    if (WIFEXITED(exit_status)) { //정상 종료시
	printf("child %d returns %d\n", pid, WEXITSTATUS(exit_status));
    }
    else { //종료된것이 아니라 stop signal을 받은것과 같은 다른 이유라면
	printf("child %d is not exited\n", pid);
    }

    /* 이전에 실행한 명령의 return value
       -> shell상에서 echo $? */
    return 0;
}
