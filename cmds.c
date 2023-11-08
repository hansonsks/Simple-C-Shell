#include "global_vars.h"
#include "cmds.h"
#include "history.h"
#include "alias.h"
#include <stdio.h>
#include <windows.h>

#define MAX_USERNAME_LEN 256
#define INPUT_BUFFER_SIZE 2000

int find_recursive(const char *dir, const char *pattern);

int cd_cmd(char **args) {
    if (args == NULL || args[0] == NULL) {
        printf("Usage: cd <path>\n");
        return 0;
    }

    if (SetCurrentDirectory(args[0])) {
       // printf("Current directory changed to: %s\n", resolved_path);
    } else {
        printf("Failed to change directory to: %s\n", args[0]);
    }

    return 1;
}

int help_cmd(char **args) {
    printf("This is Hanson's Shell - HShell\n");
    printf("Available built-in commands:\n");

    printf("\n######################### Basic Commands #########################\n");
    printf("cd <directory> \t\t-\t Change the current directory\n");
    printf("help \t\t\t-\t Display this help information\n");
    printf("ls \t\t\t-\t List files and directories in the current directory\n");
    printf("pwd \t\t\t-\t Prints the present working directory\n");
    printf("mkdir \t\t\t-\t Creates a directory / directories\n");
    printf("whoami \t\t\t-\t Prints the current user\n");
    printf("rm \t\t\t-\t Remove any file");
    printf("rmdir \t\t-'t Remove any directory\n");
    printf("cp <src> <dest> \t-\t Copies source file content to destination file to a designated location\n");
    printf("mv <src> <dest> \t-\t Moves a directory to a designated location\n");
    printf("echo \t\t\t-\t Displays text as stdout\n");
    printf("date \t\t\t-\t Displays the current time\n");
    printf("history \t\t-\t Displays the command history\n");
    printf("touch <path / None> \t-\t Creates a blank file\n");
    printf("explore <alias / path / None>\t-\t Opens Windows Explorer given a path (Best used with an alias)\n");
    printf("launch <alias / path> \t\t-\t Launch a program (.exe) given its alias or absolute path\n");
    printf("open <path> \t\t\t-\t Open any file, such as word docs and PDFs\n");
    printf("hide_path \t\t\t-\t Hides the current path and only display your username in the terminal\n");
    printf("show_path \t\t\t-\t Displays the path and hides your username\n");
    printf("find <path> <fname / *extension> \t-\t Finds files with a given name or extension (e.g., find folder1 alias*.txt)\n");
    printf("Note: If you have spaces in one argument, close them with double quotes like ""[cd \"C:\\Users\\Hanson Siu\"], don't use backslashes\n");
    printf("####################################################################\n");

    printf("\n######################### Alias Commands #########################\n");
    printf("An alias is essentially a shortcut for launching or opening a file/program with a long path\n");
    printf("Aliases are stored in the form \"<alias>=<alias_path>\" in a text file\n\n");
    printf("add_alias <alias_name> <path> \t-\t Give any file an alias to launch/open it as a shortcut\n");
    printf("remove_alias <alias_name> \t-\t Remove any alias\n");
    printf("save_alias <path> \t\t-\t Saves all aliases to a file of your choice\n");
    printf("load_alias <path> \t\t-\t Loads aliases from a text file\n");
    printf("list_alias \t\t\t-\t Lists all available aliases (Don't forget to load or save them!)\n");
    printf("peek_alias <fname> \t\t-\tLists all available aliases in a file\n");
    printf("####################################################################\n");

    printf("\n######################### Piping and Redirecting #########################\n");
    printf("TODO (I don't think I have the knowledge necessary to implement this)\n");
    printf("############################################################################\n");

    printf("\nMore to come! (maybe)\n\n");
    return 1;
}

//void expand_glob(char ***tokens, int *token_count) {
//    // Iterate through the tokens to find and expand glob patterns
//    for (int i = 0; i < *token_count; i++) {
//        char *token = (*tokens)[i];
//
//        if (strchr(token, '*') != NULL || strchr(token, '?') != NULL) {
//            WIN32_FIND_DATA find_data;
//            HANDLE hFind = FindFirstFile(token, &find_data);
//
//            if (hFind != INVALID_HANDLE_VALUE) {
//                do {
//                    // Expand the glob pattern with the matching file/directory names
//                    // and add them to the tokens array
//                    char *expanded_token = (char *)malloc(strlen(find_data.cFileName) + 1);
//                    strcpy(expanded_token, find_data.cFileName);
//                    (*tokens)[(*token_count)++] = expanded_token;
//                } while (FindNextFile(hFind, &find_data) != 0);
//
//                FindClose(hFind);
//
//                // Remove the original glob pattern token
//                free(token);
//                (*tokens)[i] = NULL;
//            }
//        }
//    }
//}

int ls_cmd(char **args) {
    WIN32_FIND_DATA find_data;
    HANDLE handle = FindFirstFile("*.*", &find_data);

    if (handle == INVALID_HANDLE_VALUE) {
        printf("Error in listing files and directores\n");
        return 0;
    }

    printf("\n########## Files in current directory ##########\n");
    do {
        printf("%s\n", find_data.cFileName);
    } while(FindNextFile(handle, &find_data) != 0);
    printf("--------------------------------------------------\n\n");

    FindClose(handle);
    return 1;
}

int quit_cmd(char **args) {
    printf("Thank you for using HShell!\n");
    return -1;
}

int launch_cmd(char **args) {
    if (args == NULL || args[0] == NULL) {
        printf("Usage: launch <file_path>\n");
        return 0;
    }

    // Concatenate the arguments to form a single operation
    char *cmd = args[0];
    for (int i = 1; args[i] != NULL; i++) {
        strcat(cmd, " ");
        strcat(cmd, args[i]);
    }

    // Create a Process Info structure
    PROCESS_INFORMATION process_info;
    ZeroMemory(&process_info, sizeof(process_info));

    // Create a Startup Info structure
    STARTUPINFO startup_info;
    ZeroMemory(&startup_info, sizeof(startup_info));

    // Create a Process
    if (!CreateProcess(
            NULL,
            cmd,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &startup_info,
            &process_info
    )) {
        fprintf(stderr, "\nFailed to execute command: launch %s\n", args[0]);
        return 1;
    }

    // Wait for the Child Process to finish
    if (WaitForSingleObject(process_info.hProcess, INFINITE) == WAIT_FAILED) {
        fprintf(stderr, "\nError waiting for command to complete: launch %s\n", args[0]);
        CloseHandle(process_info.hProcess);
        CloseHandle(process_info.hThread);
        return 1;
    }

    // Get the Exit Code of the child process
//    DWORD exit_code;
//    if (!GetExitCodeProcess(process_info.hProcess, &exit_code)) {
//        fprintf(stderr, "Error getting exit code for: %s\n", args[0]);
//        exit_code = 0;
//    }

    // Close process and thread handles
    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);

    return 1;
}

int open_cmd(char **args) {
    if (args == NULL || args[0] == NULL) {
        printf("Usage: open <file_path>\n");
        return 0;
    }

    HINSTANCE result = ShellExecute(NULL, "open", args[0], NULL, NULL, SW_SHOWNORMAL);

    if ((int) result <= 32) {
        fprintf(stderr, "\nFailed to open file: %s\n", args[0]);
    }

    return 1;
}

int explore_cmd(char **args) {
    char curr_dir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, curr_dir);
    HINSTANCE result;

    if (args == NULL || args[0] == NULL) {
        result = ShellExecute(NULL, "explore", curr_dir, NULL, NULL, SW_SHOWNORMAL);
    } else {
        result = ShellExecute(NULL, "explore", args[0], NULL, NULL, SW_SHOWNORMAL);
    }

    if ((int) result <= 32) {
        fprintf(stderr, "Failed to locate folder\n");
    }

    return 1;
}

int pwd_cmd(char **args) {
    DWORD buffer_size = MAX_PATH;
    char current_directory[MAX_PATH];

    // Get the current working directory
    if (GetCurrentDirectory(buffer_size, current_directory) == 0) {
        fprintf(stderr, "\nFailed to get the current working directory.\n");
    } else {
        printf("%s\n", current_directory);
    }

    return 1;
}

int mkdir_cmd(char **args) {
    if (args == NULL) {
        printf("Usage: mkdir <fname>\n");
        return 0;
    }

    for (int i = 0; args[i] != NULL; i++) {
        if (!CreateDirectory(args[i], NULL)) {
            fprintf(stderr, "\nFailed to create directory: %s\n", args[i]);
        }
    }
    return 1;
}

int echo_cmd(char **args) {
    if (args == NULL) {
        printf("Usage: echo <args>\n");
        return 0;
    }

    for (int i = 0; args[i] != NULL; i++) {
        printf("%s%s", i > 0 ? " " : "", args[i]);
    }
    printf("\n");

    return 1;
}

int date_cmd(char **args) {
    SYSTEMTIME curr_time;
    GetLocalTime(&curr_time);

    printf("%d-%d-%d %d-%d-%d\n", curr_time.wYear, curr_time.wMonth, curr_time.wDay,
           curr_time.wHour, curr_time.wMinute, curr_time.wSecond);

    return 1;
}

int touch_cmd(char **args) {
    if (args == NULL) {
        printf("Usage: touch <fname>\n");
        return 0;
    }

    char curr_dir[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, curr_dir);
    HANDLE hFile;

    if (args[0] == NULL) {
        printf("Usage: touch <file_name>\n");
        return 0;
    } else {
        for (int i = 0; args[i] != NULL; i++) {
            hFile = CreateFile(args[i], GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile == INVALID_HANDLE_VALUE) {
                fprintf(stderr, "Failed to create file: %s\n", args[i]);
            }
        }
    }

    CloseHandle(hFile);

    return 1;
}

int whoami_cmd(char **args) {
    DWORD size = MAX_USERNAME_LEN;
    TCHAR username[MAX_USERNAME_LEN + 1];

    if (GetUserName(username, &size)) {
        printf("Username: %s\n", username);
    } else {
        printf("Failed to get username.\n");
        return 0;
    }

    return 1;
}

int rm_cmd(char **args) {
    if (args == NULL || args[0] == NULL) {
        printf("Usage: rm <file_path>\n");
        return 1;
    }

    for (int i = 0; args[i] != NULL; i++) {
        if (DeleteFile(args[i])) {
            printf("File deleted: %s\n", args[i]);
        } else {
            fprintf(stderr, "\nFailed to delete file: %s\n", args[i]);
        }
    }

    return 1;
}

int rmdir_cmd(char **args) {
    if (args == NULL || args[0] == NULL) {
        printf("Usage: rmdir <dir_path>\n");
        return 0;
    }

    for (int i = 0; args[i] != NULL; i++) {
        if (RemoveDirectory(args[i])) {
            printf("Directory deleted: %s\n", args[i]);
        } else {
            fprintf(stderr, "\nFailed to remove directory: %s\n", args[i]);
        }
    }

    return 1;
}

int cp_cmd(char **args) {
    if (args == NULL || args[0] == NULL || args[1] == NULL) {
        printf("Usage: cp <source_file> <destination_file>\n");
        return 0;
    }

    if (CopyFile(args[0], args[1], FALSE)) {
        printf("File copied successfully: %s -> %s\n", args[0], args[1]);
    } else {
        fprintf(stderr, "Failed to copy file: %s -> %s\n", args[0], args[1]);
    }

    return 1;
}

int mv_cmd(char **args) {
    if (args == NULL || args[0] == NULL || args[1] == NULL) {
        printf("Usage: mv <source> <destination>\n");
        return 0;
    }

    const char *source = args[0];
    const char *destination = args[1];
    char newDestination[MAX_PATH];

    DWORD destinationAttributes = GetFileAttributes(destination);
    if (destinationAttributes != INVALID_FILE_ATTRIBUTES && (destinationAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        // Extract the file name from the source
        const char *fileName = strrchr(source, '\\');
        if (fileName) {
            fileName++; // Move past the backslash
        } else {
            fileName = source; // Use the entire source path as the filename
        }

        sprintf(newDestination, "%s\\%s", destination, fileName);

        if (MoveFile(source, newDestination)) {
            printf("Moved %s to %s\n", source, newDestination);
        } else {
            fprintf(stderr, "Failed to move %s to %s\n", source, newDestination);
        }
    } else {
        if (MoveFile(source, destination)) {
            printf("Renamed %s to %s\n", source, destination);
        } else {
            fprintf(stderr, "Failed to rename %s to %s\n", source, destination);
        }
    }

    return 1;
}

int history_cmd(char **args) {
    history_loop();
    return 3;
}

int find_cmd(char **args) {
    if (args == NULL || args[0] == NULL || args[1] == NULL) {
        printf("Usage: find <dir> <pattern>\n");
        return 0;
    }

    const char *dir = args[0];
    const char *pattern = args[1];
    return find_recursive(dir, pattern);
}

int find_recursive(const char *dir, const char *pattern) {
    char search_path[MAX_PATH];
    char sub_dir[MAX_PATH];
    WIN32_FIND_DATA find_data;

    snprintf(search_path, sizeof(search_path), "%s\\%s", dir, pattern);

    HANDLE hFind = FindFirstFile(search_path, &find_data);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("No files found.\n");
        return 1;
    }

    do {
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (strcmp(find_data.cFileName, ".") != 0 && strcmp(find_data.cFileName, "..") != 0) {
                snprintf(sub_dir, sizeof(sub_dir), "%s\\%s", dir, find_data.cFileName);
                find_recursive(sub_dir, pattern);
            }
        } else {
            printf("%s\\%s\n", dir, find_data.cFileName);
        }
    } while (FindNextFile(hFind, &find_data) != 0);

    FindClose(hFind);

    return 1;
}

int add_alias_cmd(char **args) {
    if (args == NULL || args[0] == NULL || args[1] == NULL) {
        printf("Usage: add_alias <name> <path / anything>\n");
        return 0;
    }
    return add_alias(args[0], args[1]);
}

int del_alias_cmd(char **args) {
    if (args == NULL || args[0] == NULL) {
        printf("Usage: del_alias <alias>\n");
        return 0;
    }
    return remove_alias(args[0]);
}

int mod_alias_cmd(char **args) {
    if (args == NULL || args[0] == NULL || args[1] == NULL) {
        printf("Usage: modify_alias <alias> <new path / something>\n");
        return 0;
    }
    return modify_alias(args[0], args[1]);
}

int save_alias_cmd(char **args) {
    if (args == NULL || args[0] == NULL) {
        printf("Usage: save_alias <fname>\n");
        return 0;
    }
    return save_aliases(args[0]);
}

int load_alias_cmd(char **args) {
    if (args == NULL || args[0] == NULL) {
        printf("Usage: load_alias <fname>\n");
        return 0;
    }
    return load_aliases(args[0]);
}

int list_alias_cmd(char **args) {
    return list_aliases();
}

int peek_alias_cmd(char **args) {
    if (args == NULL || args[0] == NULL) {
        printf("Usage: peek_alias <fname>\n");
        return 0;
    }
    return peek_alias(args[0]);
}

int clear_alias_cmd(char **args) {
    return clear_alias();
}

int hide_path(char **args) {
    display_path = 0;
    printf("Your path is now hidden\n");
    return 1;
}

int show_path(char **args) {
    display_path = 1;
    printf("Your path is now displayed\n");
    return 1;
}
