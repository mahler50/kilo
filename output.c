#include "output.h"

#include "kilo.h"
#include "row.h"

void editor_scroll() {
    E.rx = 0;
    if (E.cy < E.num_rows) {
        E.rx = editor_row_cx_to_rx(&E.row[E.cy], E.cx);
    }

    // scrolling up
    if (E.cy < E.row_off) {
        E.row_off = E.cy;
    }
    // scrolling down
    if (E.cy >= E.row_off + E.screen_rows) {
        E.row_off = E.cy - E.screen_rows + 1;
    }
    // scrolling left
    if (E.rx <  E.col_off) {
        E.col_off = E.rx;
    }
    // scrolling right
    if (E.rx >= E.col_off + E.screen_cols) {
        E.col_off = E.rx - E.screen_cols + 1;
    }
}

void editor_draw_rows(struct abuf *ab) {
    int y;
    for (y = 0; y < E.screen_rows; y++) {
        int file_row = y + E.row_off;
        if (file_row >= E.num_rows) {
            if (E.num_rows == 0 && y == E.screen_rows / 4) {
                char welcome[80];
                int welcome_len =
                    snprintf(welcome, sizeof(welcome),
                             "Kilo editor -- version %s", KILO_VERSION);
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
        } else {
            int len = E.row[file_row].rsize- E.col_off;
            if (len < 0) len = 0;
            if (len > E.screen_cols) len = E.screen_cols;
            ab_append(ab, &E.row[file_row].render[E.col_off], len);
        }

        // clear each next line during drawing.
        ab_append(ab, "\x1b[K", 3);
        ab_append(ab, "\r\n", 2);
    }
}

void editor_draw_status_bar(struct abuf *ab) {
    ab_append(ab, "\x1b[7m", 4);
    // status bar shows filename and total lines.
    char status[80], rstatus[80];
    int len = snprintf(status, sizeof(status), "%.20s - %d lines %s", E.filename ? E.filename : "[No Name]", E.num_rows, E.dirty ? "(modified)" : "");
    int rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d", E.cy + 1, E.num_rows);
    if (len > E.screen_cols) len = E.screen_cols;
    ab_append(ab, status, len);
    while (len < E.screen_cols) {
        if (E.screen_cols - len == rlen) {
            ab_append(ab, rstatus, rlen);
            break;
        } else {
            ab_append(ab, " ", 1);
            len++;
        }
    }
    ab_append(ab, "\x1b[m", 3);
    ab_append(ab, "\r\n", 2);
}

void editor_draw_message_bar(struct abuf *ab) {
    ab_append(ab, "\x1b[K", 3);
    int msg_len = strlen(E.statusmsg);
    if (msg_len > E.screen_cols) msg_len = E.screen_cols;
    if (msg_len && time(NULL) - E.statusmsg_time < 5) {
        ab_append(ab, E.statusmsg, msg_len);
    }
}

void editor_refresh_screen() {
    editor_scroll();

    struct abuf ab = ABUF_INIT;
    // `\x1b` is the ascii escape character.
    // hide the cursor.
    ab_append(&ab, "\x1b[?25l", 6);
    // `H` is used to position the cursor. No arg means we put at [0, 0] place.
    ab_append(&ab, "\x1b[H", 3);

    editor_draw_rows(&ab);
    editor_draw_status_bar(&ab);
    editor_draw_message_bar(&ab);

    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.row_off) + 1, (E.rx - E.col_off) + 1);
    ab_append(&ab, buf, strlen(buf));
    // show the cursor.
    ab_append(&ab, "\x1b[?25h", 6);

    write(STDOUT_FILENO, ab.b, ab.len);
    ab_free(&ab);
}

void editor_set_status_message(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(E.statusmsg, sizeof(E.statusmsg), fmt, ap);
    va_end(ap);
    E.statusmsg_time = time(NULL);
}
