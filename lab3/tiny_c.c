#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX 64
#define SEP " \t\n"

static char *args[MAX];
static char *out_file;

char *get_line()
{
    static char buf[1024];

    return fgets(buf, sizeof(buf), stdin);
}

char **parse_line(char *line)
{
    int n = 0;
    char *eq, *val;
    char *tok = strtok(line, SEP);

    args[0] = NULL;
    out_file = NULL;

    if (tok != NULL && (eq = strchr(tok, '=')) != NULL && eq != tok) {
        *eq = '\0';
        setenv(tok, eq + 1, 1);
        return args;
    }

    while (tok != NULL) {
        if (strcmp(tok, ">") == 0) {
            out_file = strtok(NULL, SEP);
            break;
        }
        if (tok[0] == '$') {
            val = getenv(tok + 1);
            args[n++] = val ? val : "";
        } else {
            args[n++] = tok;
        }
        tok = strtok(NULL, SEP);
    }
    args[n] = NULL;

    return args;
}

void simple_cmd(char **command)
{
    pid_t pid = fork();

    if (pid == 0) {
        if (out_file != NULL) {
            int fd = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror(out_file);
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        execvp(command[0], command);
        perror(command[0]);
        exit(1);
    }
    waitpid(pid, NULL, 0);
}

int main()
{
    while (1) {
        printf("> ");
        fflush(stdout);
        char *line = get_line();
        if (line == NULL)
            break;
        char **command = parse_line(line);
        if (command[0] == NULL)
            continue;
        if (strcmp(command[0], "exit") == 0)
            break;
        simple_cmd(command);
    }
    return 0;
}
