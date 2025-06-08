#include "abuf.h"
#include "kilo.h"
#include "output.h"

void editor_draw_rows(struct abuf *ab) {
    int y;
    for (y = 0; y < E.screen_rows; y++) {
        if (y == E.screen_rows / 3) {
            char welcome[80];
            int welcome_len = snprintf(welcome, sizeof(welcome), "Kilo editor -- version %s", KILO_VERSION);
            // if welcome is longer than cols, we just truncate it.
            if (welcome_len > E.screen_cols) welcome_len = E.screen_cols;
            int padding = (E.screen_cols - welcome_len) / 2;
            if (padding) {
                ab_append(ab, "~", 1);
                padding--;
            }
            // make welcome at the centeral of a row.
            while (padding--) ab_append(ab, " ", 1);
            ab_append(ab, welcome, welcome_len);
        } else {
            ab_append(ab, "~", 1);
        }

        // clear each line during drawing.
        ab_append(ab, "\x1b[K", 3);
        if (y < E.screen_rows - 1) {
            ab_append(ab, "\r\n", 2);
        }
    }
}

void editor_refresh_screen() {
    struct abuf ab = ABUF_INIT;
    // `\x1b` is the ascii escape character.
    // hide the cursor.
    ab_append(&ab, "\x1b[?25l", 6);
    // `H` is used to position the cursor. No arg means we put at [0, 0] place.
    ab_append(&ab, "\x1b[H", 3);

    editor_draw_rows(&ab);

    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
    ab_append(&ab, buf, strlen(buf));
    // show the cursor.
    ab_append(&ab, "\x1b[?25h", 6);

    write(STDOUT_FILENO, ab.b, ab.len);
    ab_free(&ab);
}
