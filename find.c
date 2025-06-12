#include "find.h"

#include "input.h"
#include "kilo.h"
#include "row.h"

void editor_find_callback(char *query, int key) {
    static int last_match = -1; // last matched row index, -1 means no match found
    static int direction = 1; // search direction, 1 for down, -1 for up

    if (key == '\r' || key == '\x1b') {
        last_match = -1;
        direction = 1;
        return;
    } else if (key == ARROW_RIGHT || key == ARROW_DOWN) {
        direction = 1;
    } else if (key == ARROW_LEFT || key == ARROW_UP) {
        direction = -1;
    } else {
        last_match = -1;
        direction = 1;
    }

    if (last_match == -1) direction = 1;
    // start from the last match.
    int current = last_match;

    int i;
    for (i = 0; i < E.num_rows; i++) {
        current += direction;
        // reset current position.
        if (current == -1) current = E.num_rows - 1;
        else if (current == E.num_rows) current = 0;

        erow *row = &E.row[current];
        char *match = strstr(row->render, query);
        if (match) {
            last_match = current;
            E.cy = current;
            E.cx = editor_row_rx_to_cx(row, match - row->render);
            E.row_off = E.num_rows;
            break;
        }
    }
}

void editor_find() {
    int saved_cx = E.cx;
    int saced_cy = E.cy;
    int saved_col_off = E.col_off;
    int saved_row_off = E.row_off;

    char *query =
        editor_prompt("Search: %s (Use ESC/Arrows/Enter)", editor_find_callback);

    if (query) {
        free(query);
    } else {
        E.cx = saved_cx;
        E.cy = saced_cy;
        E.col_off = saved_col_off;
        E.row_off = saved_row_off;
    }
}
