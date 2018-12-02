#include <gtk/gtk.h>
#include <SDL.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "smooth.h"
#include "to_binarize.h"
#include "GTK_functions.h"
#include "SDL_functions.h"

void on_window_main_destroy()
{
	if (PRINT_IMAGE)
	{
		g_object_unref(BUILDER);
		SDL_FreeSurface(IMAGE);
    	gtk_main_quit();
	}
}

void binarize_button()
{
	if (PRINT_IMAGE)
	{
		otsu(IMAGE);
		SDL_SaveBMP(IMAGE, "src/temp/binarized.bmp");
    	if(realpath("src/temp/binarized.bmp", FILE_NAME) == NULL)
    		return;
    	reload_image(0);
	}
}

void draw_lines_button()
{
	if (PRINT_IMAGE)
	{
		SDL_Surface *img = draw_lines(IMAGE);
		SDL_SaveBMP(img, "src/temp/rlsa.bmp");
		if(realpath("src/temp/rlsa.bmp", FILE_NAME) == NULL)
			return;
		reload_image(0);
		SDL_FreeSurface(img);
	}
}

void grayscale_button()
{
	if (PRINT_IMAGE)
	{
		greyscale(IMAGE);
		SDL_SaveBMP(IMAGE, "src/temp/grayscaled.bmp");
		if(realpath("src/temp/grayscaled.bmp", FILE_NAME) == NULL)
			return;
		reload_image(0);
	}
}

void rotate_button()
{
    printf("Rotation en loosdé\n");
}

void smoothy_button()
{
	if (PRINT_IMAGE)
	{
		remove_dots(IMAGE);
    	SDL_SaveBMP(IMAGE, "src/temp/smoothed.bmp");
    	if(realpath("src/temp/smoothed.bmp", FILE_NAME) == NULL)
    		return;
    	reload_image(0);
	}
}

void start_OCR()
{
	if (PRINT_IMAGE)
	{
		binarize_button();
		inverse(IMAGE);
		SDL_Surface *img =draw_lines(IMAGE);
		isolateLine(img);
		GObject *label = gtk_builder_get_object(BUILDER, "frame_label");
		GtkWidget *text = GTK_WIDGET(label);

		/*for (size_t i = 0; i < 10000; i++)
		{
	        printf("%c",Final_Text[i]);
		}*/
		//printf("\n");
		gtk_label_set_text(GTK_LABEL(text), Final_Text);
  		printf("OCR done\n");
		SDL_FreeSurface(img);
	}
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
		//printf("SELECTED FILE : %s\n", FILE_NAME);
		g_free(filename);
	}
	gtk_widget_destroy(dialogue);
	open_image_test();
}

void open_image_test()
{
	//Partie SDL
    SDL_Surface *image = SDL_LoadBMP(FILE_NAME);

	if(image->w > 1080 && image->h > 720)
	{
		SDL_Surface *new_image = Resize(image, 1080, 720);
		SDL_SaveBMP(new_image,"src/temp/new_image_resized.bmp");
		if(realpath("src/temp/new_image_resized.bmp", FILE_NAME) == NULL)
			return;
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
		GtkWidget *print_img_temp = gtk_image_new_from_file(FILE_NAME);
		gtk_container_remove(GTK_CONTAINER(FRAME), PRINT_IMAGE);
		gtk_container_add(GTK_CONTAINER(FRAME), print_img_temp);
    	gtk_widget_show(print_img_temp);
		PRINT_IMAGE = print_img_temp;
	}
}
