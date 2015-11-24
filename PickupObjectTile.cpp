//Scott Backer

//PickupObjectTile Class CPP: Pickup Object Tile class that contains the sprite for the pickup object

#include "PickupObjectTile.h"

//!Constructor for the PickupObjectTile class
//In - 
//		ALLEGRO_BITMAP *SpriteImage - the sprite image of the player tile
//		int InputXPosition - the starting x position of the player
//		int InputXPosition - the starting y position of the player
//		int FrameWidth - the width of one sprite frame
//		int FrameHeight - the height of one sprite frame
//		bool Collidable - true if the tile is collidable else false
//		bool Animated - true if the tile is animated else false
//		bool Continuous - true if the tile is continues else false
//		bool Looped - true if the tile should loop else false
//		int AnimFPS - the fps for the animated tile
PickupObjectTile::PickupObjectTile(ALLEGRO_BITMAP *SpriteImage, int InputXPosition, int InputYPosition, int FrameWidth, int FrameHeight,
		bool Collidable, bool Animated, bool Continuous, bool Looped, int AnimFPS)
		: Sprite(SpriteImage, InputXPosition, InputYPosition, FrameWidth, FrameHeight, Collidable, Animated, Continuous, Looped, AnimFPS)
{
	//set the alpha of the sprite
	Sprite::Set_ImageAlpha(135, 135, 135);
}

//Draws the pickupobjecttile
//In - 
//		int DrawXCoordinate - DrawXCoordinate - the x coordinate to draw the weapon at
//		int DrawYCoordinate - DrawXCoordinate - the y coordinate to draw the weapon at
void PickupObjectTile::Draw(int DrawXCoordinate, int DrawYCoordinate)
{
	al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, (DrawXCoordinate - m_FrameWidth / 2), (DrawYCoordinate - m_FrameHeight / 2), 0);
}

//sets the alpha for the sprite image
//In - 
//		int R - the red alpha channel
//		int G - the green alpha channel
//		int B - the blue alpha channel
void PickupObjectTile::SetSpriteAlpha(int R, int G, int B)
{
	Sprite::Set_ImageAlpha(R, G, B);
}