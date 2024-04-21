#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/**
 * @brief Suspend execution for an interval of time
 * @details The sleep command suspends execution for a minimum of seconds.
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(STDERR_FILENO, "Usage: sleep seconds...\n");
        exit(1);
    }

    char *p = argv[1];
    while (*p) {
        if (!is_digit(*p)) {
            fprintf(STDERR_FILENO, "%s is a invalid number\n", argv[1]);
            exit(1);
        }
        p++;
    }

    p = argv[1];
    int seconds = atoi(p); // NOLINT
    sleep(seconds);
    exit(0);
}
