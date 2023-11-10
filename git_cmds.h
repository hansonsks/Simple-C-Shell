#ifndef GIT_CMDS_H
#define GIT_CMDS_H

int git_input_exec(char *input);
int git_alias_exec(char *input);
int is_git_cmd(char *input);
int has_git_alias(char *input);

#endif //SNAKEGAME_GIT_CMDS_H
