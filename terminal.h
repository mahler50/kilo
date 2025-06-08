// terminal.h
#ifndef TERMINAL_H
#define TERMINAL_H

void die(const char *s);
void enable_raw_mode();
void disable_raw_mode();
int editor_read_key();
int get_window_size(int *rows, int *cols);

#endif
