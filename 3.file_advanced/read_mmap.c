#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

struct person {
    char name[16];
    int age;
};

static int write_info(struct person *p) {
    int fd;
    ssize_t ret;

    fd = open("person_info", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if (fd == -1) {
	printf("open() fail\n");
	return -1;
    }

    ret = write(fd, p, sizeof(struct person));
    if (ret == -1) {
	printf("write() fail\n");
	close(fd);
	return -1;
    }
    else if (ret != sizeof(struct person)) {
	printf("write() fail(partial write)\n");
	close(fd);
	return -1;
    }

    close(fd);
    return 0;
}

static int dump_info(void) {
    int fd;
    struct stat sb;
    struct person* p;

    fd = open("person_info", O_RDONLY);
     if (fd == -1) {
	printf("open() fail\n");
	return -1;
    }
     
    //fstat: 파일의 크기
    if (fstat(fd, &sb) == 1) {
	printf("stat() fail\n");
	close(fd);
	return -1;
    }

    /*
	void *nmap(void *addr, size_t length, int prot, int flags,
		int fd, off_t offset);	
    */

    //NILL: 커널에게 메모리 주소를 알아서 할당하라고 함
    p = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) {
	printf("mmanp() fail\n");
	return -1;
    }
    
    //파일 offset이 아니라 포인터를 이용해서 파일을 조작한다
    for (int i = 0; i < sb.st_size / sizeof(struct person); i++) {
	printf("name: %s, age: %d\n", p[i].name, p[i].age);
    }	

    return 0;
}   

int main(void) {
    struct person persons[] = {
	{ "kim", 40},
	{ "mike", 23}
    };
   
    write_info(&persons[0]);
    write_info(&persons[1]);

    dump_info();

    return 0;
}
