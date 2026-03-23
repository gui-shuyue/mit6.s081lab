#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int p1[2], p2[2];
    int buf[5];
    pipe(p1);
    pipe(p2);

    int pid = fork();
    if (pid > 0) {
        // printf("parent: ping\n");
        close(p1[0]); // 父进程不从 p1 读，关掉
        close(p2[1]); // 父进程不往 p2 写，关掉

        write(p1[1], "ping", 4);
        if (read(p2[0], buf, 4) > 0) {
            printf("%d: received pong\n", getpid());
        }

        close(p1[1]); // 用完即关
        close(p2[0]);

        wait(0);
    } else {
        // printf("child: pong\n");
        close(p1[1]); // 子进程不往 p1 写，关掉
        close(p2[0]); // 子进程不从 p2 读，关掉

        if (read(p1[0], buf, 4) > 0) {
            printf("%d: received ping\n", getpid());
        }
        write(p2[1], "pong", 4);

        close(p1[0]); // 用完即关
        close(p2[1]);

    }
    exit(0);
}