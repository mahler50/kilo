// output.h
#ifndef OUTPUT_H
#define OUTPUT_H

#include "abuf.h"

void editor_scroll();
void editor_refresh_screen();
void editor_set_status_message(const char *fmt, ...);
void editor_draw_status_bar(struct abuf *ab);
void editor_draw_message_bar(struct abuf *ab);
void editor_draw_rows(struct abuf *ab);

#endif
