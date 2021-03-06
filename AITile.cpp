//
// File: AITile.cpp
// Author: James Beller (Original class written by Scott Backer)
// Group: Hack-'n-Slash
// Date: 12/4/2015
//
// Note: This source file and the header file, AITile.h, are copied from Scott's
// PlayerTile files with a couple minor changes.
//
#include "AITile.h"

#include <allegro5\allegro_primitives.h>

//!Constructor for the playertile class
//In - 
//		int InputPlayerXPosition - the starting x position of the player
//		int InputPlayerXPosition - the starting y position of the player
//		int FrameWidth - the width of one sprite frame
//		int FrameHeight - the height of one sprite frame
//		bool Collidable - true if the tile is collidable else false
//		bool Animated - true if the tile is animated else false
//		bool Continuous - true if the tile is continues else false
//		bool Looped - true if the tile should loop else false
//		int AnimFPS - the fps for the animated tile
AITile::AITile(ALLEGRO_BITMAP *image, int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable = true, bool Animated = true, bool Continuous = false, bool Looped = true, int AnimFPS =  6)
		: Sprite(image, InputPlayerXPosition, InputPlayerYPosition, FrameWidth, FrameHeight, Collidable, Animated, Continuous, Looped, AnimFPS)
{
	//set the alpha of the sprite
	Sprite::Set_ImageAlpha(135, 135, 135);
}

//!Overloaded event handler from Sprite class
int AITile::Event_Handler()
{
	Update();

	return 0;
}

//!Overloaded draw function from Sprite class
//In - 
//		float InputPlayerXPosition - the player x posiiton to draw at
//		float InputPlayerXPosition - the player y posiiton to draw at
void AITile::Draw(int InputPlayerXPosition, int InputPlayerYPosition)
{
	//if there is a player tile image draw it at the specified input location
	if (m_Image != NULL)
	{
		al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, InputPlayerXPosition, InputPlayerYPosition, 0);
	}
}