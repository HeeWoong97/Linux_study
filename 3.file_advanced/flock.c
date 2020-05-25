#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <errno.h>

static void print_usage(const char* progname) {
    printf("usage: %s (ex|sh)\n", progname);
    exit(-1);
}

int main(int argc, char** argv) {
    int fd;
    int ops;

    if (argc < 2) {
	print_usage(argv[0]);
    }
    
    //어떤 종류의 LOCK을 잡을지 입력변수로 판단
    if (!strcmp(argv[1], "sh")) {
	ops = LOCK_SH;
    }
    else if (!strcmp(argv[1], "ex")) { //Exclusive Lock: 한 쓰레드가 lock을 잡으면 다른 쓰레드들은 큐에서 대기
	ops = LOCK_EX;
    }	
    else if (!strcmp(argv[1], "nb")) { //None block Lock: lock획득에 실패하면 무조건 error
	ops = LOCK_NB;
    }
    else {
	print_usage(argv[0]);
    }

    fd = open("lockfile", O_RDWR | O_CREAT, 0644); //lock으로 사용할 파일 생성
    if (fd < 0) {
	printf("open() fail\n");
	return -1;
    }

    printf("trying to grab the lock\n");
    if (flock(fd, ops) != 0) {
	printf("flock(ops %d), errno=%d(%s)\n", ops, errno, strerror(errno));
	goto out;
    }
    printf("grab the lock!!\n");
    getc(stdin); //사용자의 입력 받으면 unlock

    if (flock(fd, LOCK_UN)) {
	printf("flock(unlock)\n");
	goto out;
    }

    close(fd);

    return 0;

out:
    close(fd);
    return -1;
}
