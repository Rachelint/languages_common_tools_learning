#include <sys/signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void sig_usr_hanlde(int sig) {
    printf("sig_usr_hanlde\n");
}

void sig_int_handle(int sig) {
    printf("sig_int_handle\n");
}

int main() {
    if (signal(SIGUSR1, sig_usr_hanlde) == SIG_ERR) {
        abort();        
    }

    if (signal(SIGINT, sig_int_handle) == SIG_ERR) {
        abort();        
    }
    /*
    * Block SIGQUIT and save current signal mask.
    */
    sigset_t bmask1, oldmask;
    sigemptyset(&bmask1);
    sigaddset(&bmask1, SIGINT);
    if (sigprocmask(SIG_BLOCK, &bmask1, &oldmask) < 0) {
        abort();
    }
    printf("sig_int is blocked now\n");
    
    pause();
    
    printf("sig_int is freed now\n");
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        abort();
    }

    getchar();
}

/* 
sig_int is blocked now
^C^C^C^C^C^C^C^C^Csig_usr_hanlde
sig_int is freed now
sig_int_handle
*/