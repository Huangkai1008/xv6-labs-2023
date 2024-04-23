#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX_LEN 128

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(STDERR_FILENO, "Usages: xargs [options] [command [initial-arguments]]\n");
        exit(1);
    }

    char *arg[MAXARG];
    int j = 0;
    for (int i = 1; i < argc; i++)
        arg[j++] = argv[i];
    for (int i = argc - 1; i < MAXARG; i++)
        arg[i] = (char *)malloc(MAX_LEN);

    char buf;
    int _read;
    while (1)
    {
        int m = 0;
        while ((_read = read(0, &buf, 1)) != 0)
        {
            if (buf == ' ')
            {
                arg[j][m] = 0;
                m = 0;
                j++;
            }
            else if (buf == '\n')
            {
                arg[j][m] = 0;
                m = 0;
                j++;
                break;
            }
            else
            {
                arg[j][m] = buf;
                m++;
            }
        }
        arg[j] = 0;
        j = argc - 1;
        switch (fork()) {
            case -1:
                exit(-1);
            case 0:
                exec(arg[0], arg);
            default:
                wait(0);
        }

        if (_read <= 0)
            break;
    }
    exit(0);
}