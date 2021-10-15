/* According to POSIX.1-2001, POSIX.1-2008 */
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

int read_timeout(int fd, void* buf, size_t nbytes, struct timeval timeout) {
    fd_set          input_set;
    int             ready_for_reading = 0;
    int             read_bytes = 0;    
    FD_ZERO(&input_set );
    FD_SET(STDIN_FILENO, &input_set);

    ready_for_reading = select(1, &input_set, NULL, NULL, &timeout);
    
    /* Here, first parameter is number of FDs in the set, 
     * second is our FD set for reading,
     * third is the FD set in which any write activity needs to updated,
     * which is not required in this case. 
     * Fourth is timeout
     */

    if (ready_for_reading == -1) {
        return -1;
    } 

    if (ready_for_reading) {
        read_bytes = read(fd, buf, nbytes);
    }

    return read_bytes;
}

int main() {
    timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    const uint32_t name_len_max = 32;
    char name[name_len_max + 1] = {'\0'};
    bool read_things = false;
    while (!read_things) {
        printf("try to read from stdin\n");
        int read_bytes = read_timeout(STDIN_FILENO, name, name_len_max, timeout);
        if (read_bytes > 0) {
            printf("the name is %s\n", name);

            read_things = true;
            printf("read success");

        } else if (read_bytes == 0) {
            // when it's disk io, select and epoll... will always be active
            printf("wait for input timeout\n");
        } else {
            perror("read failed or no input");
            read_things = true;
        }
    }
}
