/*** includes ***/

#include "kilo.h"
#include "file.h"
#include "terminal.h"
#include "input.h"
#include "output.h"

/*** init ***/

// Global editor config instance.
struct EditorConfig E;

void init_editor() {
    E.cx = 0;
    E.cy = 0;
    E.rx = 0;
    E.row_off = 0;
    E.col_off = 0;
    E.num_rows = 0;
    E.row = NULL;
    E.dirty = 0;
    E.filename = NULL;
    E.statusmsg[0] = '\0';
    E.statusmsg_time = 0;

    if (get_window_size(&E.screen_rows, &E.screen_cols) == -1)
        die("get_window_size");
    // last line should be status bar.
    E.screen_rows -= 2;
}

int main(int argc, char **argv) {
    enable_raw_mode();
    init_editor();
    if (argc >= 2) {
        editor_open(argv[1]);
    }

    editor_set_status_message("HELP: CTRL-S = save | CTRL-Q = quit | CTRL-F = find");

    while (1) {
        editor_refresh_screen();
        editor_process_keypress();
    }
    return 0;
}
