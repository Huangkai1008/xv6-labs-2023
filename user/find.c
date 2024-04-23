#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

/**
 * @brief Find all the files in a directory tree with a specific name.
 */
void find(char *path, char *expr) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(STDERR_FILENO, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(STDERR_FILENO, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)
            continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0) {
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        switch (st.type) {
            case T_DIR:
                if (strcmp(de.name, ".") != 0 && strcmp(de.name, ".."))
                    find(buf, expr);
            case T_FILE:
                if (strcmp(de.name, expr) == 0) {
                    printf("%s\n", buf);
                }
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        find(".", argv[2]);
    } else if (argc == 3) {
        find(argv[1], argv[2]);
    } else {
        fprintf(STDERR_FILENO, "Usage: find <dir> <file> ...\n");
        exit(1);
    }
    exit(0);
}