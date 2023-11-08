#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "history.h"

void clear_history(void);
void print_cmd(const char *cmd);
char *get_prev_cmd(void);
char *get_next_cmd(void);
void handle_keypress(INPUT_RECORD key_event, char *curr_cmd);

#define INPUT_BUFFER_SIZE 5000
#define MAX_HISTORY_SIZE 100

int history_size = 0;
char *history[MAX_HISTORY_SIZE];
int curr_index = -1;

void history_loop(void) {
    printf("\n@@@@@@@@@@@@@@@@@@@@ Entering History mode @@@@@@@@@@@@@@@@@@@@\n");
    printf("Navigate the history with up/down arrows\n");
    printf("Q = Quit, C = Clear History, Enter = Copy current command to clipboard\n");
    print_history();

    INPUT_RECORD key_event;
    KEY_EVENT_RECORD key_record;
    DWORD num_events;
    curr_index = history_size;
    char curr_cmd[INPUT_BUFFER_SIZE];

    while (1) {
        if (ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &key_event, 1, &num_events)) {
            if (num_events > 0) {
                handle_keypress(key_event, curr_cmd);
                if (key_event.EventType == KEY_EVENT) {
                    key_record = key_event.Event.KeyEvent;
                    if (key_record.wVirtualKeyCode == 0x51 ||
                        key_record.wVirtualKeyCode == 0x43 || key_record.wVirtualKeyCode == VK_RETURN) {
                        break;
                    }
                }
            }
        }
    }

    system("cls");
}

void update_history(char *input) {
    if (history_size < MAX_HISTORY_SIZE) {
        history[history_size++] = strdup(input);
    } else {
        free(history[0]);
        for (int i = 1; i < MAX_HISTORY_SIZE; i++) {
            history[i - 1] = history[i];
        }
        history[MAX_HISTORY_SIZE - 1] = strdup(input);
    }
}

void print_history(void) {
    printf("===== Your Command History =====\n\n");
    for (int i = 0; i < history_size; i++) {
        printf("    %d: %s\n", i + 1, history[i]);
    }
    printf("\n================================\n");
}

void free_history(void) {
    for (int i = 0; i < history_size; i++) {
        free(history[i]);
    }
}

void clear_history(void) {
    for (int i = 0; i < history_size; i++) {
        free(history[i]);
    }
    history_size = 0;
    curr_index = -1;
}

void print_cmd(const char *cmd) {
    COORD coord;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    coord.X = 0;
    coord.Y = csbi.dwCursorPosition.Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    int num_clear_spaces = csbi.dwSize.X - strlen(cmd);
    for (int i = 0; i < num_clear_spaces; i++) {
        printf(" ");
    }

    coord.X = 0;
    coord.Y = csbi.dwCursorPosition.Y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    printf("%s", cmd);
}

char *get_prev_cmd(void) {
    if (curr_index > 0) {
        curr_index--;
        return history[curr_index];
    }
    return NULL;
}

char *get_next_cmd(void) {
    if (curr_index < history_size - 1) {
        curr_index++;
        return history[curr_index];
    } else if (curr_index == history_size - 1) {
        curr_index = -1;
    }
    return NULL;
}

void handle_keypress(INPUT_RECORD key_event, char *curr_cmd) {
    char *prev_cmd;
    char *next_cmd;

    if (key_event.EventType == KEY_EVENT) {
        KEY_EVENT_RECORD key = key_event.Event.KeyEvent;

        if (key.bKeyDown) {
            if (key.wVirtualKeyCode == VK_UP) {
                prev_cmd = get_prev_cmd();
                if (prev_cmd) {
                    print_cmd(prev_cmd);
                    strncpy(curr_cmd, prev_cmd, strlen(prev_cmd) + 1);
                }
            } else if (key.wVirtualKeyCode == VK_DOWN) {
                next_cmd = get_next_cmd();
                if (next_cmd) {
                    print_cmd(next_cmd);
                    strncpy(curr_cmd, next_cmd, strlen(next_cmd) + 1);
                }
            } else if (key.wVirtualKeyCode == 0x43) {
                clear_history();
            } else if (key.wVirtualKeyCode == VK_RETURN) {
                if (OpenClipboard(NULL)) {
                    EmptyClipboard();
                    size_t len = strlen(curr_cmd) + 1;
                    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, len * sizeof(wchar_t));

                    if (hg) {
                        wchar_t *dest = (wchar_t *)GlobalLock(hg);
                        mbstowcs(dest, curr_cmd, len);
                        GlobalUnlock(hg);
                        SetClipboardData(CF_UNICODETEXT, hg);
                    }

                    CloseClipboard();
                }
            }
        }
    }
}

