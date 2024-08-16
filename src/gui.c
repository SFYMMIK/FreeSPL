#include "include/gui.h"
#include <gtk/gtk.h>

GtkWidget* window;

void create_window(const char* title, int width, int height) {
    gtk_init(NULL, NULL);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), title);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void add_label(const char* text) {
    GtkWidget* label = gtk_label_new(text);
    gtk_container_add(GTK_CONTAINER(window), label);
}

void add_button(const char* label, void (*onClick)(void)) {
    GtkWidget* button = gtk_button_new_with_label(label);
    g_signal_connect(button, "clicked", G_CALLBACK(onClick), NULL);
    gtk_container_add(GTK_CONTAINER(window), button);
}

void start_main_loop() {
    gtk_widget_show_all(window);
    gtk_main();
}
