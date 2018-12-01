#ifndef GTK_FUNCTIONS
#define GTK_FUNCTIONS

#include <gtk/gtk.h>
#include <SDL.h>

char FILE_NAME[256];
GtkBuilder *BUILDER;
GtkWidget *PRINT_IMAGE;
GObject *FRAME;
SDL_Surface *IMAGE;
//GObject *object, gpointer user_data
void on_window_main_destroy();
void binarize_button();
void draw_lines_button();
void grayscale_button();
void rotate_button();
void smoothy_button();

void select_file(GObject *bouton);
void open_image_test();
void reload_image(int assertion);
//SDL_Surface* resize_image(SDL_Surface *img, int w, int h);

#endif