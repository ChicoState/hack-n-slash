//Scott Backer

<<<<<<< HEAD
=======
//Scott Backer

>>>>>>> Scott
//PlayerTile Class CPP: Handles the sprite control for the player character

#include "PlayerTile.h"

#include <allegro5\allegro_primitives.h>

<<<<<<< HEAD
PlayerTile::PlayerTile(ALLEGRO_BITMAP *Image, int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable = true, bool TriggerTile = false, bool Animated = true, bool Looped = false, int AnimFPS = 30)
		: Sprite(Image, FrameWidth, FrameHeight, Collidable, Animated, Looped, AnimFPS)
{
	
}

void PlayerTile::Draw(float InputPlayerXPosition, float InputPlayerYPosition)
{

	if (m_Image != NULL)
	{
		Sprite::Set_CurRow(2, false);
		//al_draw_bitmap_region(m_Image, 100, 100, 100.100, 100, InputPlayerXPosition, InputPlayerYPosition, 0);
		al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, InputPlayerXPosition, InputPlayerYPosition, 0);
	}

	//draw the character
	/*
	al_draw_filled_rectangle(InputPlayerXPosition, InputPlayerYPosition - 9, InputPlayerXPosition + 10, InputPlayerYPosition - 7, al_map_rgb(255, 0, 0));
	al_draw_filled_rectangle(InputPlayerXPosition, InputPlayerYPosition + 9, InputPlayerXPosition + 10, InputPlayerYPosition + 7, al_map_rgb(255, 0, 0));
	al_draw_filled_triangle(InputPlayerXPosition - 12, InputPlayerYPosition - 17, InputPlayerXPosition + 12, InputPlayerYPosition, InputPlayerXPosition - 12, InputPlayerYPosition + 17, al_map_rgb(0, 255, 0));
	al_draw_filled_rectangle(InputPlayerXPosition - 12, InputPlayerYPosition - 2, InputPlayerXPosition + 15, InputPlayerYPosition + 2, al_map_rgb(0, 0, 255));
	*/
=======
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
PlayerTile::PlayerTile(int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable = true, bool Animated = true, bool Continuous = false, bool Looped = true, int AnimFPS =  6)
		: Sprite(al_load_bitmap("Player_Sprite.png"), InputPlayerXPosition, InputPlayerYPosition, FrameWidth, FrameHeight, Collidable, Animated, Continuous, Looped, AnimFPS)
{
	//set the alpha of the sprite
	Sprite::Set_ImageAlpha(135, 135, 135);
}

//!Overloaded event handler from Sprite class
int PlayerTile::Event_Handler()
{
	Update();

	return 0;
}

//!Overloaded draw function from Sprite class
//In - 
//		float InputPlayerXPosition - the player x posiiton to draw at
//		float InputPlayerXPosition - the player y posiiton to draw at
void PlayerTile::Draw(int InputPlayerXPosition, int InputPlayerYPosition)
{
	//if there is a player tile image draw it at the specified input location
	if (m_Image != NULL)
	{
		al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, InputPlayerXPosition, InputPlayerYPosition, 0);
	}
>>>>>>> Scott
}