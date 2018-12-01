#include <gtk/gtk.h>
#include <SDL.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "to_binarize.h"
#include "GTK_functions.h"
#include "SDL_functions.h"
#include "../decoupage/decoupage.h"
//GObject *object, gpointer user_data
void on_window_main_destroy()
{
	g_object_unref(BUILDER);
	SDL_FreeSurface(IMAGE);
    gtk_main_quit();
}

void binarize_button()
{
    otsu(IMAGE);
    char *s = realpath("../temp/binarized.bmp", NULL);
    strncpy(FILE_NAME, s, 256);
    reload_image(0);
}

void draw_lines_button()
{
	SDL_Surface *img = draw_lines(IMAGE);
	SDL_SaveBMP(img, "../temp/rlsa.bmp");
	char *s = realpath("../temp/rlsa.bmp", NULL);
	strncpy(FILE_NAME, s, 256);
	reload_image(0);
	SDL_FreeSurface(img);
}

void grayscale_button()
{
	greyscale(IMAGE);
	char *s = realpath("../temp/grayscaled.bmp", NULL);
	strncpy(FILE_NAME, s, 256);
	//IMAGE = img;
	reload_image(0);
}

void rotate_button()
{
    printf("Rotation en loosdé\n");
}

void smoothy_button()
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
		printf("SELECTED FILE : %s\n", FILE_NAME);
		//printf("%s\n", FILE_NAME);
		g_free(filename);
	}
	gtk_widget_destroy(dialogue);
	open_image_test();
}

void open_image_test()
{
	//Partie SDL
    SDL_Surface *image = SDL_LoadBMP(FILE_NAME);
    printf("SDL loading img\n");

	if(image->w > 720 && image->h > 480)
	{
		SDL_Surface *new_image = Resize(image, 720, 480);
		printf("RESIZED img\n");
		SDL_SaveBMP(new_image,"../temp/new_image_resized.bmp");
		printf("SAVED new bmp\n");
		char *s = realpath("../temp/new_image_resized.bmp",FILE_NAME);
		strncpy(FILE_NAME, s, 256);
		printf("NEWfile name : %s\n", FILE_NAME);
		SDL_FreeSurface(new_image);
	}

	IMAGE = SDL_LoadBMP(FILE_NAME);
	SDL_FreeSurface(image);
	//Affichage fenentre dans GTK
	if (PRINT_IMAGE)
		reload_image(0);
	else
		reload_image(1);
}

void reload_image(int assertion)
{
	//GObject *frame = gtk_builder_get_object(BUILDER, "frame");
	if (assertion == 1)
	{
		FRAME = gtk_builder_get_object(BUILDER, "frame");
		PRINT_IMAGE = gtk_image_new_from_file(FILE_NAME);
		gtk_container_add(GTK_CONTAINER(FRAME), PRINT_IMAGE);
    	gtk_widget_show(PRINT_IMAGE);
	}
	if (assertion == 0)
	{
		printf("affichage : %s\n", FILE_NAME);
		//GtkWidget *print_img_temp = g_object_ref(PRINT_IMAGE);
		GtkWidget *print_img_temp = gtk_image_new_from_file(FILE_NAME);
		gtk_container_remove(GTK_CONTAINER(FRAME), PRINT_IMAGE);
		gtk_container_add(GTK_CONTAINER(FRAME), print_img_temp);
    	gtk_widget_show(print_img_temp);
		PRINT_IMAGE = print_img_temp;
		//FRAME = gtk_builder_get_object(BUILDER, "frame");
		//PRINT_IMAGE = gtk_image_new_from_file(FILE_NAME);
		//reload_image(1);
	}

	//GtkWidget *img;
	//PRINT_IMAGE = gtk_image_new_from_file(FILE_NAME);

	//gtk_container_add(GTK_CONTAINER(FRAME), PRINT_IMAGE);
    //gtk_widget_show(PRINT_IMAGE);
    //gtk_container_remove(GTK_CONTAINER(FRAME), PRINT_IMAGE);
}
