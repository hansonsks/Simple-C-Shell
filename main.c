#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "global_vars.h"
#include "cmd_exec.h"
#include "alias.h"
#include "history.h"
#include "git_cmds.h"

#define INPUT_BUFFER_SIZE 5000
#define TOKEN_BUFFER_SIZE 200
#define MAX_USERNAME_LEN 256
#define TOKEN_DELIM " \t\r\n\a"
#define MAX_PATH_SIZE 1000

display_path = 0;

void shell_loop(void);
char *shell_read_input(void);
char **shell_get_args(char *input);
int shell_exec(char **args);

int main(int argc, char **argv) {
    shell_loop();
}

void shell_loop(void) {
    int status;
    char curr_dir[MAX_PATH];
    DWORD username_len = MAX_USERNAME_LEN;
    TCHAR username[username_len + 1];
    char *input;
    char history_input[INPUT_BUFFER_SIZE];
    char **args;

    do {
        if (display_path) {
            GetCurrentDirectory(MAX_PATH, curr_dir);
            printf("%s>", curr_dir);
        } else {
            GetUserName(username, &username_len);
            printf("[%s]>", username);
        }

        input = shell_read_input();
        strcpy(history_input, input);

        if (is_git_cmd(input)) {
            git_input_exec(input);
        } else if (has_git_alias(input)) {
            git_alias_exec(input);
        } else {
            args = shell_get_args(input);
            status = shell_exec(args);
            free(args);
        }

        free(input);

        if (status == 1 && history_input[0] != '\0')
            update_history(history_input);
    } while (status != -1);

    free_aliases();
    free_history();
}

char *shell_read_input(void) {
    int c;
    int buffer_index = 0;
    int buffer_size = INPUT_BUFFER_SIZE;
    char *buffer = (char *) malloc(sizeof(char) * buffer_size);

    if (!buffer) {
        fprintf(stderr, "HShell: Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    do {
        c = getchar();
        if (c == EOF || c == '\n')
            buffer[buffer_index++] = '\0';
        else
            buffer[buffer_index++] = c;

        if (buffer_index >= buffer_size) {
            buffer_size += INPUT_BUFFER_SIZE;
            buffer = (char *) realloc(buffer, buffer_size);

            if (!buffer) {
                fprintf(stderr, "HShell: Out of memory!\n");
                exit(EXIT_FAILURE);
            }
        }
    } while (c != EOF && c != '\n');

    return buffer;
}

char **shell_get_args(char *input) {
    int buffer_index = 0;
    int buffer_size = INPUT_BUFFER_SIZE;
    char *token;
    char *quoted_arg;
    char **tokens = (char **) malloc(sizeof(char *) * buffer_size);

    if (!tokens) {
        fprintf(stderr, "HShell: Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(input, TOKEN_DELIM);
    while (token != NULL) {

        // Handle arguments as a string
        if (token[0] == '"') {
            quoted_arg = (char *) malloc(sizeof(char) * MAX_PATH_SIZE);
            if (!quoted_arg) {
                fprintf(stderr, "HShell: Out of memory!\n");
                exit(EXIT_FAILURE);
            }

            // Copy the argument without the opening quote to quoted_arg
            strcpy(quoted_arg, token + 1);

            // While the ending quote is not found, continue to copy until no more arguments found
            while (token[strlen(token) - 1] != '"' && (token = strtok(NULL, TOKEN_DELIM)) != NULL) {
                strcat(quoted_arg, " ");
                strcat(quoted_arg, token);
            }

            // If the last token is valid and ends with ", terminate the quoted_arg string
            if (token) {
                if (token[strlen(token) - 1] == '"') {
                    quoted_arg[strlen(quoted_arg) - 1] = '\0';
                }
            }

            tokens[buffer_index++] = quoted_arg;

        } else {
            // Handle arguments as usual
            tokens[buffer_index++] = token;
        }

        if (buffer_index >= buffer_size) {
            buffer_size += TOKEN_BUFFER_SIZE;
            tokens = (char **) realloc(tokens, sizeof(char *) * buffer_size);

            if (!tokens) {
                fprintf(stderr, "HShell: Out of memory!\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[buffer_index] = NULL;

    return tokens;
}

int shell_exec(char **args) {
    int status = exec_cmd(args);
    if (status == -2)
        fprintf(stderr, "Unrecognised Command: %s\n", args[0]);
    return status;
}
