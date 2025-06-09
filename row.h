// row.h
#ifndef ROW_H
#define ROW_H

#include "kilo.h"

int editor_row_cx_to_rx(erow *row, int cx);
void editor_update_row(erow *row);
void editor_append_row(char *s, size_t len);

#endif
