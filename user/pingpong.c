#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MSG_LEN 1


/**
 * @brief Ping-pong a byte between two processes.
 * @details Uses xv6 system calls to ''ping-pong'' a byte between two processes over a pair of pipes,
 *          one for each direction.
 */
int main() {
    // Two pipes for two direction communication.
    int p2c[2];
    int c2p[2];

    // Create pipes, if errors, exit with code 1.
    if (pipe(p2c) == -1 || pipe(c2p) == -1) {
        exit(1);
    }

    char byte;

    switch (fork()) {
        case -1:
            exit(1);
        case 0:
            // The child process should receive the byte from the parent,
            // print "<pid>: received ping", where <pid> is its process ID,
            // write the byte on the pipe to the parent, and exit.
            if (close(p2c[1]) == -1 || close(c2p[0]) == -1) { // Close unused ends
                exit(-1);
            };

            read(p2c[0], &byte, MSG_LEN);
            fprintf(STDOUT_FILENO, "%d: received ping\n", getpid());
            close(p2c[0]);

            // Write the byte on the pipe to the parent.
            write(c2p[1], &byte, MSG_LEN);
            close(c2p[1]);
            exit(0);

        default:
            // The parent process should send a byte to the child,
            // and read the byte from the child,
            // print "<pid>: received pong" and exit.
            if (close(p2c[0]) == -1 || close(c2p[1]) == -1) { // Close unused ends
                exit(-1);
            }

            byte = 'b';
            write(p2c[1], &byte, MSG_LEN);
            close(p2c[1]);

            // Wait for child to exit and read response.
            wait(0);

            read(c2p[0], &byte, MSG_LEN);
            fprintf(STDOUT_FILENO, "%d: received pong\n", getpid());
            close(c2p[0]);
            exit(0);
    }
}