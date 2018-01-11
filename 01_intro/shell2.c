#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 4096

static void sig_handler (int);

int main (void)
{
    if (signal(SIGINT, sig_handler) == SIG_ERR)
    {
        printf("signal error");
        exit(EXIT_FAILURE);
    }

    char buf[MAX_LINE_LENGTH];

    printf("%% ");

    while (fgets(buf, MAX_LINE_LENGTH, stdin) != NULL)
    {
        if (buf[strlen(buf) - 1] == '\n')
        {
            buf[strlen(buf) - 1] = 0;
        }

        pid_t pid;

        if ((pid = fork()) < 0)
        {
            printf("fork error");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // child
            execlp(buf, buf, (char *) 0);
            printf("couldn't execute: %s", buf);
            exit(127);
        }

        int status;

        // parent
        if ((pid = waitpid(pid, &status, 0)) < 0)
        {
            printf("waitpid error");
            exit(EXIT_FAILURE);
        }

        printf("%% ");
    }
}

void sig_handler (int sig_num)
{
    printf("interrupt (%d)\n%% ", sig_num);
}
