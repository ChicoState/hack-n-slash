//Scott Backer

//GUILayer.CPP - Draws the GUI Layer of the game to the screen

#ifndef GUILAYER_H
#define GUILAYER_H

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <string>

#include "Camera.h"

class GUILayer
{
public:
	GUILayer(int InputScreenWidth, int InputScreenHeight); //!Constructor for the GUI Layer
	void DrawPlayerInformation(Camera& InputCamera, int InputPlayerLevel, int InputPlayerHealth); //!Draws the player information to the gui layer
	void DrawScoreInformation(Camera& InputCamera, int InputScore); //Draws the score information of the player
	void DrawGameTimeInformation(Camera& InputCamera, const ALLEGRO_TIMER* InputTimer); //Draws the game time information of the player
	void DrawFinalScoreScreen(int InputFinalScore); //Draws the final score information of the player on the end screen

private:
	ALLEGRO_BITMAP* PlayerInfoImage; //background image for player info section
	ALLEGRO_BITMAP* TimerInfoImage; //background image for timer info section
	ALLEGRO_FONT *font28; //font for player health and level and score
	ALLEGRO_FONT *font96; //font for final player score

	int m_ScreenWidth; //the screen width of the game display
	int m_ScreenHeight; //the screen height of the game display
};

#endif