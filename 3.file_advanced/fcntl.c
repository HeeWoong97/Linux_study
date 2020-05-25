#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

//프로세스가 생성되면 기본적으로 3개의 fd가 열린다 
//0번: STDIN_FILENO, 1번: STDOUT_FILENO, 2번: STDERR_FILENO
int main(int argc, char** argv) {
    int ret;
    int flag;
    int fd;
    char buf[128];

    fd = STDIN_FILENO;
    
    //blocking 모드로 실행
    //키보드로 입력하고 enter를 누를때까지 대기
    printf("trying to read...\n");
    memset(buf, 0, sizeof(buf));
    ret = read(fd, buf, sizeof(buf)); //stdin은 자동으로 닫힌다
    printf("read() [%s] - %d bytes\n", buf, ret);
    
    flag = fcntl(fd, F_GETFL);
    if (flag == -1) {
	printf("fcntl(F_GETFL) fail\n");
	return -1;
    }

    //non-blocking 모드로 설정
    ret = fcntl(fd, F_SETFL, flag | O_NONBLOCK);
    if (ret == -1) {
	printf("fcntl(F_SETFL) fail\n");
	return -1;		
    }
    
    //non-blocking 모드로 실행
    //키보드에서 입력이 없으면 바로 quit
    printf("trying to read...\n");
    memset(buf, 0, sizeof(buf));
    ret = read(fd, buf, sizeof(buf));
    printf("read() [%s] - %d bytes\n", buf, ret);
    if (ret == -1) {
	printf("errno=%d, %s\n", errno, strerror(errno)); //eoono 11: 더이상 읽을 값이 없을때
    }	

    return 0;
}
