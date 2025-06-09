#include "file.h"

#include "row.h"
#include "kilo.h"
#include "terminal.h"

void editor_open(char *filename) {
    free(E.filename);
    E.filename = strdup(filename);

    FILE *fp = fopen(filename, "r");
    if (!fp) die("fopen");

    char *line = NULL;
    size_t line_cap = 0;
    ssize_t line_len;
    while ((line_len = getline(&line, &line_cap, fp)) != -1) {
        // we need to delete "\r\n"
        while (line_len > 0 &&
               (line[line_len - 1] == '\n' || line[line_len - 1] == '\r'))
            line_len--;

        // copy the line to editor text buffer.
        editor_append_row(line, line_len);
    }

    free(line);
    fclose(fp);
}
