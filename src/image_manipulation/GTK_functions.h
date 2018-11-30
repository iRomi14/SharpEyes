#ifndef GTK_FUNCTIONS
#define GTK_FUNCTIONS

#include <gtk/gtk.h>
#include <SDL.h>

char FILE_NAME[256];
GtkBuilder *BUILDER;
GtkWidget *PRINT_IMAGE;
GObject *FRAME;
SDL_Surface *IMAGE;

void on_window_main_destroy(GObject *object, gpointer user_data);
void binarize_button(GObject *object, gpointer user_data);
void grayscale_button(GObject *object, gpointer user_data);
void rotate_button(GObject *object, gpointer user_data);
void smoothy_button(GObject *object, gpointer user_data);

void select_file(GObject *bouton);
void open_image_test();
void reload_image(int assertion);
//SDL_Surface* resize_image(SDL_Surface *img, int w, int h);

#endif