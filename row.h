// row.h
#ifndef ROW_H
#define ROW_H

#include "kilo.h"

int editor_row_rx_to_cx(erow *row, int rx);
int editor_row_cx_to_rx(erow *row, int cx);
void editor_update_row(erow *row);
/* Insert a row at a specific line */
void editor_insert_row(int at, char *s, size_t len);
/* Insert a character at a specific place in a row */
void editor_row_insert_char(erow *row, int at, int c);
/* Insert a character at cursor's place  */
void editor_insert_char(int c);
/* Delete a character at a specific place in a row */
void editor_row_del_char(erow *row, int at);
/* Delete a character at cursor's place */
void editor_del_char();
void editor_free_row(erow *row);
/* Delete a specific row */
void editor_del_row(int at);
/* Append string to a row */
void editor_row_append_string(erow *row, char *s, size_t len);
/* Insert a new line at the cursor's place */
void editor_insert_new_line();

#endif
