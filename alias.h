#ifndef ALIAS_H
#define ALIAS_H

#define MAX_ALIAS_ITEMS 100

typedef struct {
    char *alias;
    char *path;
} AliasItem;

extern int alias_count;
extern AliasItem alias_list[MAX_ALIAS_ITEMS];

int add_alias(const char *alias, const char *path);
int remove_alias(const char *alias);
int modify_alias(const char *alias, const char *path);
char *get_alias_path(const char *alias);
int find_alias(const char *alias);
int save_aliases(const char *fname);
int load_aliases(const char *fname);
void free_aliases(void);
int list_aliases(void);
int peek_alias(const char *fname);
int clear_alias(void);
int is_alias(char *alias);

#endif //ALIAS_H
