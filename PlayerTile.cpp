//Scott Backer

//PlayerTile Class H: Handles the sprite control for the player character

#include "PlayerTile.h"

#include <allegro5\allegro_primitives.h>

PlayerTile::PlayerTile(int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable = true, bool TriggerTile = false, bool Animated = true, bool Looped = false, int AnimFPS = 0)
		: Sprite(FrameWidth, FrameHeight, Collidable, Animated, Looped, AnimFPS), m_XPosition(InputPlayerXPosition), m_YPosition(InputPlayerYPosition)
{
	m_Image = al_load_bitmap("Player_Sprite.png");
	//m_Tile_Width = 48;
	//m_Tile_Height = 64;
	m_CurColumn = 0;
	m_MaxFramesX = 3;
	//al_get_bitmap_width(m_Image) / m_Tile_Width;
}

void PlayerTile::Draw(float InputPlayerXPosition, float InputPlayerYPosition)
{
	m_XPosition = InputPlayerXPosition;
	m_YPosition = InputPlayerYPosition;

	//draw the character
	al_draw_filled_rectangle(InputPlayerXPosition, InputPlayerYPosition - 9, InputPlayerXPosition + 10, InputPlayerYPosition - 7, al_map_rgb(255, 0, 0));
	al_draw_filled_rectangle(InputPlayerXPosition, InputPlayerYPosition + 9, InputPlayerXPosition + 10, InputPlayerYPosition + 7, al_map_rgb(255, 0, 0));
	al_draw_filled_triangle(InputPlayerXPosition - 12, InputPlayerYPosition - 17, InputPlayerXPosition + 12, InputPlayerYPosition, InputPlayerXPosition - 12, InputPlayerYPosition + 17, al_map_rgb(0, 255, 0));
	al_draw_filled_rectangle(InputPlayerXPosition - 12, InputPlayerYPosition - 2, InputPlayerXPosition + 15, InputPlayerYPosition + 2, al_map_rgb(0, 0, 255));
}

void PlayerTile::SetXPosition(int InputPlayerXPosition)
{

}

void PlayerTile::SetYPosition(int InputPlayerYPosition)
{

}