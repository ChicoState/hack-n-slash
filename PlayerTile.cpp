//Scott Backer

//PlayerTile Class CPP: Handles the sprite control for the player character

#include "PlayerTile.h"

#include <allegro5\allegro_primitives.h>

PlayerTile::PlayerTile(ALLEGRO_BITMAP *Image, int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable = false, bool Animated = false, bool Continuous = false, bool Looped = false, int AnimFPS =  6)
		: Sprite(Image, InputPlayerXPosition, InputPlayerYPosition, FrameWidth, FrameHeight, Collidable, Animated, Continuous, Looped, AnimFPS)
{
	
}

void PlayerTile::Draw(float InputPlayerXPosition, float InputPlayerYPosition)
{

	if (m_Image != NULL)
	{
		//Sprite::Set_CurRow(2, false);
		//al_draw_bitmap_region(m_Image, 0, 0, 48, 64, InputPlayerXPosition, InputPlayerYPosition, 0);
		al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, InputPlayerXPosition, InputPlayerYPosition, 0);
	}

	//draw the character
	/*
	al_draw_filled_rectangle(InputPlayerXPosition, InputPlayerYPosition - 9, InputPlayerXPosition + 10, InputPlayerYPosition - 7, al_map_rgb(255, 0, 0));
	al_draw_filled_rectangle(InputPlayerXPosition, InputPlayerYPosition + 9, InputPlayerXPosition + 10, InputPlayerYPosition + 7, al_map_rgb(255, 0, 0));
	al_draw_filled_triangle(InputPlayerXPosition - 12, InputPlayerYPosition - 17, InputPlayerXPosition + 12, InputPlayerYPosition, InputPlayerXPosition - 12, InputPlayerYPosition + 17, al_map_rgb(0, 255, 0));
	al_draw_filled_rectangle(InputPlayerXPosition - 12, InputPlayerYPosition - 2, InputPlayerXPosition + 15, InputPlayerYPosition + 2, al_map_rgb(0, 0, 255));
	*/
}