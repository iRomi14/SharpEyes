#include <gtk/gtk.h>
#include <SDL.h>
#include <stdio.h>
#include <string.h>

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

void select_file(GObject *bouton)
{
	gint res;
	GtkWidget *topparent = gtk_widget_get_toplevel(GTK_WIDGET(bouton));
	GtkWidget *dialogue = gtk_file_chooser_dialog_new("Select Image",
														GTK_WINDOW (topparent),
														GTK_FILE_CHOOSER_ACTION_OPEN,
														"Ouvrir", GTK_RESPONSE_ACCEPT,
														"Annuler", GTK_RESPONSE_CANCEL,
														NULL);

	res = gtk_dialog_run(GTK_DIALOG(dialogue));
	if (res == GTK_RESPONSE_ACCEPT)
	{
		char *filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialogue));
		strncpy(FILE_NAME, filename, 256);
		/*GtkWidget *image = gtk_image_new_from_file(filename);
		open_image(filename, GTK_IMAGE(image));*/
		//printf("%s\n%s\n", filename, FILE_NAME);
		g_free(filename);
	}
	gtk_widget_destroy(dialogue);
	open_image_test(&BUILDER);
}

void open_image(GObject *object, gpointer user_data)
{
	GObject *frame = gtk_builder_get_object(&BUILDER, "frame");

	GtkWidget *image = gtk_image_new_from_file(FILE_NAME);

	gtk_container_add(GTK_CONTAINER(frame), image);
    gtk_widget_show(image);

	/*SDL_Surface *img = IMG_Load(filename);

	if(img->w > 1080 && img->h > 720)
		{

			//printf("Redimensionnement \n");
			SDL_Surface *new = Resize(img);

			SDL_SaveBMP(new,"image_resize");
    		SDL_FreeSurface(new);
			gtk_image_set_from_file (GTK_IMAGE (image), "image_resize");
		}

	else
		gtk_image_set_from_file (GTK_IMAGE (image), filename);

    //sdl_main();
    //printf("lancement du main mdr\n");*/
}

void open_image_test(GtkBuilder *builder)
{
	printf("debug\n");
	GObject *frame = gtk_builder_get_object(NULL, "frame");
printf("debug2\n");
	GtkWidget *image = gtk_image_new_from_file(FILE_NAME);
printf("debug3\n");
	gtk_container_add(GTK_CONTAINER(frame), image);
  printf("debug4\n");
    gtk_widget_show(image);
}
