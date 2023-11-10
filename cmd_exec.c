#include "cmds.h"
#include "string.h"
#include "alias.h"
#include "git_cmds.h"
#include <stdio.h>
#include <stdlib.h>

char **resolve_alias(char **args);
int get_cmd_count();

char *builtin_strs[] = {
        "cd",
        "ls",
        "help",
        "quit",
        "launch",
        "open",
        "pwd",
        "mkdir",
        "echo",
        "date",
        "touch",
        "explore",
        "whoami",
        "rm",
        "rmdir",
        "cp",
        "mv",
        "history",
        "find",
        "add_alias",
        "del_alias",
        "mod_alias",
        "save_alias",
        "load_alias",
        "list_alias",
        "peek_alias",
        "clear_alias",
        "hide_path",
        "show_path"
};

int (*builtin_cmds[]) (char **) = {
        &cd_cmd,
        &ls_cmd,
        &help_cmd,
        &quit_cmd,
        &launch_cmd,
        &open_cmd,
        &pwd_cmd,
        &mkdir_cmd,
        &echo_cmd,
        &date_cmd,
        &touch_cmd,
        &explore_cmd,
        &whoami_cmd,
        &rm_cmd,
        &rmdir_cmd,
        &cp_cmd,
        &mv_cmd,
        &history_cmd,
        &find_cmd,
        &add_alias_cmd,
        &del_alias_cmd,
        &mod_alias_cmd,
        &save_alias_cmd,
        &load_alias_cmd,
        &list_alias_cmd,
        &peek_alias_cmd,
        &clear_alias_cmd,
        &hide_path,
        &show_path
};

int get_cmd_count() {
    return sizeof(builtin_strs) / sizeof(char *);
}

int exec_cmd(char **args) {
    if (args[0] == NULL) {
        return 1;
    }

    int exit_code = -2;
    char *cmd = args[0];
    char **resolved_args = resolve_alias(args);
    for (int i = 0; i < get_cmd_count(); i++) {
        if (strcmp(cmd, builtin_strs[i]) == 0) {
            exit_code = (*builtin_cmds[i]) (resolved_args);
        }
    }

//    char git_cmd[1000]; git_cmd[0] = '\0';
//    if (is_git_cmd(get_alias_path(cmd))) {
//        strcat(git_cmd, get_alias_path(cmd));
//        strcat(git_cmd, " ");
//        if (resolved_args != NULL) {
//            for (int i = 0; resolved_args[i] != NULL; i++) {
//                strcat(git_cmd, resolved_args[i]);
//                strcat(git_cmd, " ");
//            }
//        }
//        exit_code = git_exec(git_cmd);
//    } else {
//        for (int i = 0; i < get_cmd_count(); i++) {
//            if (strcmp(cmd, builtin_strs[i]) == 0) {
//                exit_code = (*builtin_cmds[i]) (resolved_args);
//            }
//        }
//    }

    if (resolved_args != NULL)
        free(resolved_args);

    return exit_code;
}

int get_arg_count(char **args) {
    int count = 0;
    while (args[count++] != NULL) ;
    return count;
}

char **resolve_alias(char **args) {
    if (args[1] == NULL) {
        return NULL;
    }

    char *launch_path = get_alias_path(args[1]);
    char **resolved_args = (char **) malloc(sizeof(char *) * get_arg_count(args));
    if (!resolved_args) {
        fprintf(stderr, "HShell: Out of memory!\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < get_arg_count(args); i++) {
        resolved_args[i] = NULL;
    }

    if (launch_path) {
        resolved_args[0] = launch_path;
        for (int i = 2; args[i] != NULL; i++) {
            resolved_args[i - 1] = args[i];
        }
    } else {
        for (int i = 1; args[i] != NULL; i++) {
            resolved_args[i - 1] = args[i];
        }
    }

    return resolved_args;
}
