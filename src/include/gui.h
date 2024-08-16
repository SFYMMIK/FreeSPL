#ifndef GUI_H
#define GUI_H

void create_window(const char* title, int width, int height);
void add_label(const char* text);
void add_button(const char* label, void (*onClick)(void));
void start_main_loop();

#endif
