/*** includes ***/

#include "kilo.h"
#include "terminal.h"
#include "input.h"
#include "output.h"

/*** init ***/

// Global editor config instance.
struct EditorConfig E;

void init_editor() {
    E.cx = 0;
    E.cy = 0;

    if (get_window_size(&E.screen_rows, &E.screen_cols) == -1)
        die("get_window_size");
}

int main() {
    enable_raw_mode();
    init_editor();

    while (1) {
        editor_refresh_screen();
        editor_process_keypress();
    }
    return 0;
}
