#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char * const argv[]) {
    int pipe_fd[2];

    if(pipe(pipe_fd) < 0) {
        perror("pipe");
        return 1;
    }

    if(fork() == 0) {
        close(pipe_fd[1]);

        char buf[10];
        int bytes = 0;
        while(read(pipe_fd[0], buf, 10) > 0) {
            write(1, buf, bytes);
        }

        _exit(0);
    }

    close(pipe_fd[0]);

    wait(NULL);

    return 0;
}
