#include <gtk/gtk.h>
#include <stdio.h>
#include <SDL.h>

#include "GTK_functions.h"
#include "to_binarize.h"
#include "open_image.h"
#include "SDL_functions.h"

/* 
    Compiler GTK + SDL: 
        gcc -o gladewin main.c open_image.c GTK_functions.c SDL_functions.c to_binarize.c ../pixel/pixel_operations.c ../decoupage/decoupage.h
        -Wall $(sdl2-config --cflags --libs) `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
    Compiler SDL : 
        gcc test.c -o exec $(sdl2-config --cflags --libs)
*/


int main(int argc, char *argv[])
{
	GtkBuilder  *builder;
	GtkWidget   *window;

	gtk_init(&argc, &argv);
 
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "window_main.glade", NULL);
 
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    //Association fonction quitter a la fenetre
    GObject *quit_button = gtk_builder_get_object(builder, "quit");
    g_signal_connect (quit_button, "select", G_CALLBACK (on_window_main_destroy), NULL);

    //Ouvrir image
    GObject *open_button = gtk_builder_get_object(builder, "open");
    g_signal_connect (open_button, "activate", G_CALLBACK (open_image), NULL);

    //binarisation
    /*GObject *bin_button = gtk_builder_get_object(builder, "binariser");
    g_signal_connect (bin_button, "activate", G_CALLBACK (binarize_button), NULL);*/

    //Grayscale
    GObject *gray_button = gtk_builder_get_object(builder, "gris");
    g_signal_connect (gray_button, "activate", G_CALLBACK (grayscale_button), NULL);

    //Rotation
    GObject *rotat_button = gtk_builder_get_object(builder, "rotation");
    g_signal_connect (rotat_button, "activate", G_CALLBACK (rotate_button), NULL);

    //Lissage
    GObject *smooth_button = gtk_builder_get_object(builder, "lissage");
    g_signal_connect (smooth_button, "activate", G_CALLBACK (smoothy_button), NULL);
 
    g_object_unref(builder);
 
    gtk_widget_show(window);                
    gtk_main();
 
    return 0;
}
