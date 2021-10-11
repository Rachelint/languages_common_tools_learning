#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

// new pipe pairs
int fd_pipe(int fd[2]) {
    // return: -1 failed, 0 success
    return(socketpair(AF_UNIX, SOCK_STREAM, 0, fd));
}

static const uint32_t parent_side = 0;
static const uint32_t child_side = 1;

int main() {
    int pipes[2];
    if (-1 == fd_pipe(pipes)) {
        perror("fd_pipe failed");
        abort();
    }

    int ret = fork();
    if (ret > 0) {
        printf("parent: begin\n");

        // close child_side
        int parent_pipe_fd = pipes[parent_side];
        close(pipes[child_side]);

        char buf[1];
        int ret = read(parent_pipe_fd, buf, 1);

        if (0 == ret) {
            printf("parent: child process finished\n");
        }
    } else if (ret == 0) {
        printf("child: begin\n");

        int child_pipe_fd = pipes[child_side];
        close(pipes[parent_side]);

        sleep(1);
        printf("child: finished\n");
    } else {
        perror("fork() failed");
        abort();
    }   
}