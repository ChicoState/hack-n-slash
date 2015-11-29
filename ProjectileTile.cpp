//Scott Backer

//ProjectileTile Class CPP: Handles the sprite control for the Projectile weapon

#include "ProjectileTile.h"

#include <allegro5\allegro_primitives.h>

//!Constructor for the Projectileweapontile class
//In - 
//		int InputXPosition - the starting x position of the player
//		int InputXPosition - the starting y position of the player
//		int FrameWidth - the width of one sprite frame
//		int FrameHeight - the height of one sprite frame
//		bool Collidable - true if the tile is collidable else false
//		bool Animated - true if the tile is animated else false
//		bool Continuous - true if the tile is continues else false
//		bool Looped - true if the tile should loop else false
//		int AnimFPS - the fps for the animated tile
ProjectileTile::ProjectileTile(int InputXPosition = 0, int InputYPosition = 0, int FrameWidth = 44, int FrameHeight = 44,
		bool Collidable = true, bool Animated = false, bool Continuous = false, bool Looped = false, int AnimFPS =  6)
		: Sprite(al_load_bitmap("Player_Bow_Arrow_Sprite.png"), InputXPosition, InputYPosition, FrameWidth, FrameHeight, Collidable, Animated, Continuous, Looped, AnimFPS)
{
	//set the alpha of the sprite
	if(m_Image != NULL)
	{
		Sprite::Set_ImageAlpha(0, 128, 128);
	}
}

//!Overloaded event handler from Sprite class
int ProjectileTile::Event_Handler()
{
	Update();

	return 0;
}

//!Overloaded draw function from Sprite class
//In - 
//		int DrawXCoordinate - DrawXCoordinate - the x coordinate to draw the weapon at
//		int DrawYCoordinate - DrawXCoordinate - the y coordinate to draw the weapon at
//		int XDirection - the x direction relative to the player/AI using the weapon to draw at (use graph coordinates to calculate (Ex. 0, 1 is North or Up))
//		int YDirection - the y direction relative to the player/AI using the weapon to draw at (use graph coordinates to calculate (Ex. 0, 1 is North or Up))
void ProjectileTile::Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection)
{
	//draw sprite up
	if(XDirection == 0 && YDirection == -1)
	{
		Set_CurRow(1, false);
		al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, (DrawXCoordinate - m_FrameWidth / 2), (DrawYCoordinate - m_FrameHeight / 2), 0);
	}

	//draw sprite down
	else if(XDirection == 0 && YDirection == 1)
	{
		Set_CurRow(0, false);
		al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, (DrawXCoordinate - m_FrameWidth / 2), (DrawYCoordinate - m_FrameHeight / 2), 0);
	}

	//draw sprite left
	else if(XDirection == -1 && YDirection == 0)
	{
		Set_CurRow(3, false);
		al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, (DrawXCoordinate - m_FrameWidth / 2), (DrawYCoordinate - m_FrameHeight / 2), 0);
	}

	//draw sprite right
	else if(XDirection == 1 && YDirection == 0)
	{
		Set_CurRow(2, false);
		al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, (DrawXCoordinate - m_FrameWidth / 2), (DrawYCoordinate - m_FrameHeight / 2), 0);
	}

	//else draw a default direction
	else
	{
		Set_CurRow(1, false);
		al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, (DrawXCoordinate - m_FrameWidth / 2), (DrawYCoordinate - m_FrameHeight / 2), 0);
	}
}