#include <stdlib.h>
#include <string.h>
#include "alias.h"

//#define MAX_GIT_CMD_LEN 1000

//int git_cmd(char **args) {
//    char full_cmd[MAX_GIT_CMD_LEN] = "git ";
//
//    if (args != NULL) {
//        for (int i = 0; args[i] != NULL; i++) {
//            printf("PRINTING \"%s\"\n", args[i]);
//            strcat(full_cmd, args[i]);
//            strcat(full_cmd, " ");
//        }
//    }
//
//    return system(full_cmd);
//}

int git_input_exec(char *input) {
    // Let Git handle git commands
    return system(input);
}

int git_alias_exec(char *input) {
    char git_args[1000] = "";
    char first_part[100] = "";
    char rest[1000] = "";

    const char *whitespace = strchr(input, ' ');
    if (whitespace != NULL) {
        size_t first_part_len = whitespace - input;
        strncpy(first_part, input, first_part_len);
        strcpy(rest, whitespace + 1);

        strcat(git_args, get_alias_path(first_part));
        strcat(git_args, " ");
        strcat(git_args, rest);
    } else {
        strcat(git_args, get_alias_path(input));
    }

    return system(git_args);
}

int is_git_cmd(const char *input) {
    if (input == NULL) {
        return 0;
    }

    char *git_str = "git";
    int i = 0;
    while (i < 3 && (git_str[i] == input[i])) i++;
    return i == 3;
}

int has_git_alias(char *input) {
    char temp[100] = "";
    strcpy(temp, input);
    return find_alias(strtok(temp, " "));
}
