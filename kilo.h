#ifndef KILO_H
#define KILO_H

#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define KILO_VERSION "0.0.1"
#define KILO_TAB_STOP 8
#define KILO_QUIT_TIMES 3

// map a key to ctrl-key
#define CTRL_KEY(k) ((k) & 0x1f)

enum EditorKey {
    BACKSPACE = 127,
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    HOME_KEY,
    END_KEY,
    PAGE_UP,
    PAGE_DOWN
};

// data structures

// represent a row of text.
typedef struct erow {
    int size;
    int rsize;
    char *chars;
    char *render;
} erow;

struct EditorConfig {
    // cursor's position
    int cx, cy;
    int rx;
    // row offset
    int row_off;
    // col offset
    int col_off;
    // terminal size
    int screen_rows;
    int screen_cols;
    // row number of text
    int num_rows;
    // text buffer
    erow *row;
    int dirty;
    char *filename;
    char statusmsg[80];
    time_t statusmsg_time;
    // origin termios state
    struct termios org_termios;
};

extern struct EditorConfig E;

#endif
