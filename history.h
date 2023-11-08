#ifndef HISTORY_H
#define HISTORY_H

extern int history_size;
extern int curr_index;

void history_loop(void);
void update_history(char *input);
void print_history(void);
void free_history(void);

#endif //HISTORY_H
