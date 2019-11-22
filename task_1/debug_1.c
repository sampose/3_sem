#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define A_LOT_OF 64
#define DELIMITER " (),\n"
#define SIZE_OF_PCHAR sizeof(char*)

char **parse_the_input(char *line) {
        int bufsize = A_LOT_OF;
        char **tokens = malloc(SIZE_OF_PCHAR * bufsize);
        char *cur_token;

        int position = 0;
        cur_token = strtok(line, DELIMITER);
        while(cur_token != NULL) {
                tokens[position] = cur_token;
                position++;
                if (position >= bufsize) {
                        bufsize += A_LOT_OF;
                        tokens = realloc(tokens, SIZE_OF_PCHAR * bufsize);
                }
                cur_token = strtok(NULL, DELIMITER);
        }
        tokens[position] = NULL;

        return tokens;
}

int main() {
	
        pid_t P;
        while(1) {
		printf("%s %c", "Write a command:", '\n');
                P = fork();
                if (P < 0) {
                        printf("%s %c", "Fork error", '\n');
                        return 0;
                }
                if (P == 0) {
                        char *line = NULL;
                        ssize_t bufsize = 0;
                        getline(&line, &bufsize, stdin);
                        char **prepared_command = parse_the_input(line);
                        if (strcmp(prepared_command[0], "exit") == 0) exit(254);
                        execv(prepared_command[0], prepared_command);
                        free(line);

                        printf("%s %c", "I'm the child process and I have failed.", '\n');
                        exit(255);

                }

                else {
                        int status;
                        waitpid(P, &status, 0);
                        if (WEXITSTATUS(status) == 254) return 0;
                        printf("%s %d %c", "I'm the parent process and my child returned: ", WEXITSTATUS(status), '\n');
                }
        }
        return 0;
}





