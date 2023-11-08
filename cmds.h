#ifndef CMDS_H
#define CMDS_H

int cd_cmd(char **args);
int help_cmd(char **args);
int ls_cmd(char **args);
int quit_cmd(char **args);
int launch_cmd(char **args);
int open_cmd(char **args);
int pwd_cmd(char **args);
int mkdir_cmd(char **args);
int echo_cmd(char **args);
int date_cmd(char **args);
int touch_cmd(char **args);
int explore_cmd(char **args);
int whoami_cmd(char **args);
int rm_cmd(char **args);
int rmdir_cmd(char **args);
int cp_cmd(char **args);
int mv_cmd(char **args);
int history_cmd(char **args);
int find_cmd(char **args);
int add_alias_cmd(char **args);
int del_alias_cmd(char **args);
int mod_alias_cmd(char **args);
int save_alias_cmd(char **args);
int load_alias_cmd(char **args);
int list_alias_cmd(char **args);
int peek_alias_cmd(char **args);
int clear_alias_cmd(char **args);
int hide_path(char **args);
int show_path(char **args);

#endif //CMDS_H
