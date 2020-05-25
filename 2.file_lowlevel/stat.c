#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

// 실행시 파일 이름을 인자로 받는 프로그램
// argc: 넘겨받는 인자의 갯수. 파일의 절대경로때문에 갯수는 무조건 1 이상이다
// argv[][]: 인자를 저장. 첫번째 원소는 파일의 절대경로, 두번째 원소부터 넘겨받은 인자
int main(int argc, char** argv) {
	struct stat statbuf;
	
	if (argc < 2) {
		printf("usage: %s filename\n", argv[0]);
		return -1;
	}
	
	if (stat(argv[1], &statbuf)) {
		printf("stat() fail\n");
		return -1;
	}

	printf("filename: %s\n", argv[1]);
	printf("size: %ld\n", statbuf.st_size);
	
// file type
	if (S_ISDIR(statbuf.st_mode)) {
		printf("It is directory\n");
	}
	else if (S_ISREG(statbuf.st_mode)) {
		printf("It is regular file\n");
	}
	else {
		printf("?????\n");
	}

	return 0;
}
