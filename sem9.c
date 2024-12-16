#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Signal handler function
void sig_handler(int signum) {
    printf("Received signal: %d\n", signum);
}

int main() {
    struct sigaction sa = { .sa_handler = sig_handler, .sa_flags = 0 };
    sigset_t mask;

    // Register signal handler for SIGINT
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return EXIT_FAILURE;
    }
    printf("Signal handler registered for SIGINT (Ctrl+C)\n");

    // Check and modify the signal mask
    sigemptyset(&mask);
    if (sigprocmask(SIG_BLOCK, NULL, &mask) == -1) {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }

    if (sigismember(&mask, SIGINT)) {
        printf("SIGINT is already present in the signal mask.\n");
    } else {
        sigaddset(&mask, SIGINT);
        if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
            perror("sigprocmask");
            return EXIT_FAILURE;
        }
        printf("Added SIGINT to the signal mask.\n");
    }

    // Wait for signals indefinitely
    for (;;) pause();

    return 0;
}
