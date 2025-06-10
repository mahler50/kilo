// input.h
#ifndef INPUT_H
#define INPUT_H

char *editor_prompt(char *prompt);
void editor_process_keypress();
void editor_move_cursor(int key);

#endif
