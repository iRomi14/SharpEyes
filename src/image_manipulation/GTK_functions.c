#include <gtk/gtk.h>
#include <SDL.h>

#include "open_image.h"
#include "GTK_functions.h"

void on_window_main_destroy(GObject *object, gpointer user_data)
{
    gtk_main_quit();
}

/*void binarize_button(GObject *object, gpointer user_data)
{
    otsu(object);
}*/

void grayscale_button(GObject *object, gpointer user_data)
{
    printf("Niveau de gris tmtc\n");
}

void rotate_button(GObject *object, gpointer user_data)
{
    printf("Rotation en loosdé\n");
}

void smoothy_button(GObject *object, gpointer user_data)
{
    printf("Le beau lissage Starfoulaye\n");
}

void open_image(GObject *object, gpointer user_data)
{
    sdl_main();
    //printf("lancement du main mdr\n");
}
