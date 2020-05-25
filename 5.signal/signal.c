#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

/* kill 명령어를 통해 프로세스에 시그널 보내는 방법
shell 상에서
1) ps -ef | grep 'process name' 명령어를 통해 프로세스의 pid를 얻는다
2) kill 'signal' 'process pid'를 통해 프로세스에 signal 전달

ex) kill -TERM 2660: 2660번 프로세스에 SIGTERM을 보낸다  */

/* SIGTERM을 받으면 실행하는 함수
SIGTERM은 프로세스를 종료시키는 신호인데, handler함수에서 종료하는 부분이 없으므로 프로세스를 종료시키지 않는다. */
void sigterm_handler(int signum) {
    printf("got sigterm...\n");
} 

int main(int argc, char** argv) {
    sigset_t set; //시그널들을 저장할 set

    sigemptyset(&set); //set 초기화
    sigaddset(&set, SIGINT); //set에 SIGINT 추가
    sigprocmask(SIG_SETMASK, &set, NULL); //set에 있는 신호가 들어와도 무시한다
    
    signal(SIGTERM, sigterm_handler); //SIGTERM: Terminate signal
    printf("start of while(1)\n");
    while(1) {
	sleep(1);
    }
    return 0;
}
