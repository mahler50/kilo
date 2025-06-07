/*** includes ***/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/*** data model ***/

// original termios state.
struct termios org_termios;

void die(const char *s) {
    perror(s);
    exit(1);
}

/*** terminal ***/

// recover original termios state.
void disable_raw_mode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &org_termios) == -1)
        die("tcsetattr");
}

void enable_raw_mode() {
    // get STDIN termios.
    if (tcgetattr(STDIN_FILENO, &org_termios) == -1) die("tcgetattr");
    atexit(disable_raw_mode);

    struct termios raw = org_termios;
    // disable ICRNL(carriage return cast) & IXON(software flow control) flag.
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    // disable OPOST(post process of output) flag.
    raw.c_oflag &= ~(OPOST);
    // set character size to 8 bits per byte.
    raw.c_cflag |= (CS8);
    // disable ECHO & ICANON & IEXTEN & ISIG flag.
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    // minimum number of bytes of input before `read` return.
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    // set termios back, TCSAFLUSH means waits for all pending output and discards any input that hasn't been read.
    if (tcsetattr(STDERR_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/*** init ***/

int main() {
    enable_raw_mode();

    // in raw mode, we press `q` to exit.
    while (1) {
        char c = '\0';
        // FIXME: this will crash when reading something through pipeline.
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
        if (iscntrl(c)) {
            printf("%d\r\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }
        if (c == 'q') break;
    }
    return 0;
}
