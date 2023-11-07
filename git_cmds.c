#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int git_exec(char *input) {
    return system(input);
}

int is_git_cmd(char *input) {
    if (strlen(input) < 3) {
        return 0;
    }

    char *git_str = "git";
    int i = 0;
    while (i < 3 && (git_str[i] == input[i])) i++;
    return i == 3;
}