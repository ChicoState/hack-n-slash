//Created by:		Ryan Nolan-Hieb

#include "Utility.h"

//Randomly creates and returns a number between two amounts
int Random(int RandomAmount1, int RandomAmount2)
{
	int Number = rand() % (RandomAmount2 - RandomAmount1) + RandomAmount1;

	return Number;
}

int InitAllegro()
{
	if (!al_init())										//initialize Allegro
		return 1;

	al_init_primitives_addon();
	al_install_mouse();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();

	return 0;
}