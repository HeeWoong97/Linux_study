#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>

#if 0
struct inotify_event {
               int      wd;       /* Watch descriptor */
               uint32_t mask;     /* Mask describing event */
               uint32_t cookie;   /* Unique cookie associating related
                                     events (for rename(2)) */
               uint32_t len;      /* Size of name field */
               char     name[];   /* Optional null-terminated name */
           };
#endif

//디렉터리 2개를 watch하면서 파일이 삭제되거 생성되는 이벤트 catch
int main(int argc, char** argv) {
    int fd = -1;
    int wd1 = -1;
    int wd2 = -1;
    char buf[1024];
    int ret;
    struct inotify_event* event;

    //inotify에서 catch한 이벤트를 읽어들일 fd
    fd = inotify_init();
    if (fd == -1) {
	printf("inotify_init() fail\n");
	return -1;
    }
    
    //폴더 지정, wathch생성
    wd1 = inotify_add_watch(fd, ".", IN_CREATE | IN_DELETE);
    if (wd1 == -1) {
	printf("add_watch() fail\n");
	goto err;
    }
    wd2 = inotify_add_watch(fd, "../file_basic", IN_CREATE | IN_DELETE);
    if (wd2 == -1) {
	printf("add_watch() fail\n");
	goto err;
    }

    while (1) {
	//buf안에 있는 내용: 파일 이벤트에 대한 inotify_event구조체들
	//ret: fd로 부터 읽어온 데이터의 size
	ret = read(fd, buf, sizeof(buf));
	if (ret == -1) {
	    printf("read() fail\n");
	    break;
	}

	//buf의 첫번째 원소를 struct inotify_event형 포인터가 가리키게 만듦(강제 형변환)
	//얘를 event포인터가 가리킴
	event = (struct inotify_event*)&buf[0];
	//모든 이벤트에
	while (ret > 0) {
	    if (event->mask & IN_CREATE) {
		printf("file %s is created\n", event->name);
	    }   
	    if (event->mask & IN_DELETE) {
		printf("file %s is deleted\n", event->name);
	    }
	    //event가 가리키는 위치를 다음으로 옮김
	    //inotify_event의 실제 크기: sizeof(struct inotify_event) + event->len
	    event = (struct inotify_event*)((char*)event + sizeof(struct inotify_event) + event->len);
	    //ret(모든 event의 size)에서 이벤트 하나씩 뺌)
	    ret -= (sizeof(struct inotify_event) + event->len);
	}
    }
    close(fd);
    close(wd1);
    close(wd2);

    return 0;

err:
    if (fd >= 0) {
	close(fd);
    }
    if (wd1 >= 0) {
	close(wd1);
    }
    if (wd2 >= 0) {
	close(wd2);
    }

    return -1;
}
