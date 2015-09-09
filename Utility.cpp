//Created by:		Ryan Nolan-Hieb
//Date updated:		9/3/15
//Last update by:	N/A
//Reason for update:N/A

#include "Utility.h"

void Utility::Set_GameOver(bool GameOver)
{
	m_GameOver = GameOver;
}

bool Utility::Get_GameOver() const
{
	return m_GameOver;
}

void Utility::InitAllegro()
{
	if (!al_init())										//initialize Allegro
		m_GameOver = 1;

	al_init_primitives_addon();
	al_install_mouse();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
}