#ifndef GTK_FUNCTIONS
#define GTK_FUNCTIONS

#include <gtk/gtk.h>

void on_window_main_destroy(GObject *object, gpointer user_data);
// void binarize_button(GObject *object, gpointer user_data);
void grayscale_button(GObject *object, gpointer user_data);
void rotate_button(GObject *object, gpointer user_data);
void smoothy_button(GObject *object, gpointer user_data);
void open_image(GObject *object, gpointer user_data);

#endif