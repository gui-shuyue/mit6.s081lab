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
        write(p1[1], "ping", 4);
        if (read(p2[0], buf, 4) > 0) {
            printf("%d: received pong\n", getpid());
        }
        wait(0);
    } else {
        // printf("child: pong\n");
        if (read(p1[0], buf, 4) > 0) {
            printf("%d: received ping\n", getpid());
        }
        write(p2[1], "pong", 4);
    }
    exit(0);
}