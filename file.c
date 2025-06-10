#include "file.h"

#include "input.h"
#include "kilo.h"
#include "output.h"
#include "row.h"
#include "terminal.h"

char *editor_rows_to_string(int *buf_len) {
    int total_len = 0;
    int j;
    for (j = 0; j < E.num_rows; j++) {
        total_len += E.row[j].size + 1;
    }

    *buf_len = total_len;
    char *buf = malloc(total_len);
    char *p = buf;
    for (j = 0; j < E.num_rows; j++) {
        memcpy(p, E.row[j].chars, E.row[j].size);
        p += E.row[j].size;
        *p = '\n';
        p++;
    }

    return buf;
}

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

        // insert a line to editor text buffer.
        editor_insert_row(E.num_rows, line, line_len);
    }

    free(line);
    fclose(fp);
    E.dirty = 0;
}

void editor_save() {
    if (E.filename == NULL) {
        E.filename = editor_prompt("Save as : %s (ESC to cancel)");
        if (E.filename == NULL) {
            editor_set_status_message("Save aborted");
            return;
        }
    }

    int len;
    char *buf = editor_rows_to_string(&len);

    int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
    if (fd != -1) {
        if (ftruncate(fd, len) != -1) {
            if (write(fd, buf, len) == len) {
                close(fd);
                free(buf);
                E.dirty = 0;
                editor_set_status_message("%d bytes written to disk", len);
                return;
            }
        }
        close(fd);
    }

    free(buf);
    editor_set_status_message("Failed to save! I/O error: %s", strerror(errno));
}
