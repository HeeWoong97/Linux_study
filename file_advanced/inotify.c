#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>

//디렉터리 2개를 watch하면서 파일이 삭제되거 생성되는 이벤트 catch
int main(int argc, char** argv) {
    int fd = -1;
    int wd = -1;
    char buf[1024];
    int ret;

    //inotify에서 catch한 이벤트를 읽어들일 fd
    fd = inotify_init();
    if (fd == -1) {
	printf("inotify_init() fail\n");
	return -1;
    }
    
    //폴더 지정, wathch생성
    wd = inotify_add_watch(fd, ".", IN_CREATE | IN_DELETE);
    if (wd == -1) {
	printf("add_watch() fail\n");
	goto err;
    }
    wd = inotify_add_watch(fd, "../file_basic", IN_CREATE | IN_DELETE);
    if (wd == -1) {
	printf("add_watch() fail\n");
	goto err;
    }

    ret = read(fd, buf, sizeof(buf));

    return 0;

err:
    if (fd >= 0){
	close(fd);
    }
    if (wd >= 0){
	close(wd);
    }

    return -1;
}
