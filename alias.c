#include "alias.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PATH_SIZE 1000
#define PATH_SEPARATOR "="

int alias_count = 0;
AliasItem alias_list[MAX_ALIAS_ITEMS];

int add_alias(const char *alias, const char *path) {
    if (find_alias(alias)) {
        return modify_alias(alias, path);
    }

    if (alias_count < MAX_ALIAS_ITEMS && alias != NULL && path != NULL) {
        alias_list[alias_count].alias = strdup(alias);
        alias_list[alias_count].path = strdup(path);
        alias_count++;
        return 1;
    } else {
        fprintf(stderr, "Alias list has reached its max size = %d\n", MAX_ALIAS_ITEMS);
        return 0;
    }
}

int remove_alias(const char *alias) {
    for (int i = 0; i < alias_count && alias_list[i].alias != NULL; i++) {
        if (strcmp(alias, alias_list[i].path) == 0) {
            free(alias_list[i].alias);
            free(alias_list[i].path);

            for (int j = i; j < alias_count - 1 && alias_list[j + 1].alias != NULL; j++) {
                alias_list[j].alias = strdup(alias_list[j + 1].alias);
                alias_list[j].path = strdup(alias_list[j + 1].path);
                free(alias_list[j + 1].alias); // Free the moved content
                free(alias_list[j + 1].path);  // Free the moved content
            }
            alias_count--;

            printf("The alias has been successfully deleted\n");
            return 1;
        }
    }
    printf("The alias was not found\n");
    return 0;
}

int modify_alias(const char *alias, const char *path) {
    for (int i = 0; i < alias_count && alias_list[i].alias != NULL; i++) {
        if (strcmp(alias, alias_list[i].path) == 0) {
            free(alias_list[i].path);
            alias_list[i].path = strdup(path);
            printf("The alias has been successfully modified\n");
            return 1;
        }
    }
    printf("Alias not found\n");
    return 0;
}

char *get_alias_path(const char *alias) {
    if (alias == NULL) {
        return NULL;
    }

    for (int i = 0; i < alias_count; i++) {
        if (strcmp(alias, alias_list[i].alias) == 0) {
            return alias_list[i].path;
        }
    }
    return NULL;
}

int find_alias(const char *alias) {
    if (alias == NULL) {
        return 0;
    }

    for (int i = 0; i < alias_count; i++) {
        if (strcmp(alias, alias_list[i].alias) == 0) {
            return 1;
        }
    }
    return 0;
}

int save_aliases(const char *fname) {
    FILE *file = fopen(fname, "w");
    if (file) {
        for (int i = 0; i < alias_count; i++) {
            fprintf(file, "%s%s%s\n", alias_list[i].alias, PATH_SEPARATOR, alias_list[i].path);
        }
        fclose(file);
        return 1;
    } else {
        fprintf(stderr, "File not found to save aliases\n");
    }
    return 0;
}

int load_aliases(const char *fname) {
    char line[MAX_PATH_SIZE];
    FILE *file = fopen(fname, "r");
    if (file) {
        while (fgets(line, sizeof(line), file)) {
            char *alias = strtok(line, PATH_SEPARATOR);
            char *path = strtok(NULL, "\n");
            if (alias && path) {
                add_alias(alias, path);
            }
        }
        fclose(file);
        return 1;
    } else {
        fprintf(stderr, "File not found to load aliases\n");
    }
    return 0;
}

void free_aliases(void) {
    for (int i = 0; i < alias_count; i++) {
        free(alias_list[i].alias);
        free(alias_list[i].path);
    }
}

int clear_alias(void) {
    free_aliases();
    alias_count = 0;
    printf("Your alias list has been cleared\n");
    return 1;
}

int list_aliases(void) {
    if (alias_count == 0) {
        printf("You have no aliases yet!\n");
        return 1;
    }

    printf("\n===== Your Aliases =====\n\n");
    for (int i = 0; i < alias_count && alias_list[i].alias != NULL; i++) {
        printf("  %d. %s -> %s\n", i + 1, alias_list[i].alias, alias_list[i].path);
    }
    printf("\n========================\n\n");
    return 1;
}

int is_alias(char *alias) {
    for (int i = 0; alias_list[i].alias != NULL; i++) {
        if (strcmp(alias, alias_list[i].alias) == 0) {
            return 1;
        }
    }
    return 0;
}

int peek_alias(const char *fname) {
    FILE *file = fopen(fname, "r");
    if (file == NULL) {
        fprintf(stderr, "\"%s\" not found for peeking aliases\n", fname);
        return 0;
    }

    printf("\n===== %s's Aliases =====\n\n", fname);
    int i = 1;
    char line[MAX_PATH_SIZE]; // Adjust buffer size as needed
    while (fgets(line, sizeof(line), file) != NULL) {
        char *alias = strtok(line, PATH_SEPARATOR);
        char *path = strtok(NULL, "\n");
        if (alias && path) {
            printf("%d: %s, Path: %s\n", i++, alias, path);
        }
    }
    printf("\n========================\n\n");

    fclose(file);
    return 1;
}
