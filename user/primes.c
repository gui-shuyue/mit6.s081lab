#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void filter(int left_pipe) {
    int left_first;

    // 读第一个数
    if (read(left_pipe, &left_first, sizeof(int)) <= 0) {
        close(left_pipe);
        exit(0);
    }

    printf("prime %d\n", left_first);

    int right_pipe[2];
    pipe(right_pipe);

    if (fork() == 0) {
        close(right_pipe[1]); // 子进程不往右管道写，关掉
        filter(right_pipe[0]);
    } else {
        close(right_pipe[0]); // 父进程不从右管道读，关掉

        int num;
        // 读剩下的数
        while (read(left_pipe, &num, sizeof(int)) > 0) {
            if (num % left_first != 0) {
                write(right_pipe[1], &num, sizeof(int));
            }
        }

        close(left_pipe);
        close(right_pipe[1]);
        wait(0);
        exit(0);
    }

}

int main() {
    int p[2];
    pipe(p);

    if (fork() == 0) {
        close(p[1]); // 子进程不往管道写，关掉
        filter(p[0]);
    } else {
        close(p[0]); // 父进程不从管道读，关掉

        for (int i = 2; i <= 35; i++) {
            write(p[1], &i, sizeof(int));
        }

        close(p[1]);
        wait(0);
        exit(0);
    }
    return 0;
}