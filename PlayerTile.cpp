//Scott Backer

//Scott Backer

//PlayerTile Class CPP: Handles the sprite control for the player character

#include "PlayerTile.h"

#include <allegro5\allegro_primitives.h>

//!Constructor for the playertile class
//In - 
<<<<<<< HEAD
=======
//		ALLEGRO_BITMAP *SpriteImage - the sprite image of the player tile
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31
//		int InputPlayerXPosition - the starting x position of the player
//		int InputPlayerXPosition - the starting y position of the player
//		int FrameWidth - the width of one sprite frame
//		int FrameHeight - the height of one sprite frame
//		bool Collidable - true if the tile is collidable else false
//		bool Animated - true if the tile is animated else false
//		bool Continuous - true if the tile is continues else false
//		bool Looped - true if the tile should loop else false
//		int AnimFPS - the fps for the animated tile
<<<<<<< HEAD
PlayerTile::PlayerTile(int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable = true, bool Animated = true, bool Continuous = false, bool Looped = true, int AnimFPS =  6)
		: Sprite(al_load_bitmap("Player_Movement_Sprite_BIG.png"), InputPlayerXPosition, InputPlayerYPosition, FrameWidth, FrameHeight, Collidable, Animated, Continuous, Looped, AnimFPS),
		 m_SwordSprite(al_load_bitmap("Player_Sword_Sprite_BIG.png"), InputPlayerXPosition, InputPlayerYPosition, FrameWidth, FrameHeight, Collidable, Animated, Continuous, Looped, AnimFPS),
		 m_BowSprite(al_load_bitmap("Player_Bow_Sprite_BIG.png"), InputPlayerXPosition, InputPlayerYPosition, FrameWidth, FrameHeight, Collidable, Animated, Continuous, Looped, AnimFPS)
=======
PlayerTile::PlayerTile(ALLEGRO_BITMAP *SpriteImage, int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable = true, bool Animated = true, bool Continuous = false, bool Looped = true, int AnimFPS =  6)
		: Sprite(SpriteImage, InputPlayerXPosition, InputPlayerYPosition, FrameWidth, FrameHeight, Collidable, Animated, Continuous, Looped, AnimFPS)
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31
{
	//set the alpha of the sprite
	Sprite::Set_ImageAlpha(0, 128, 128);
	m_SwordSprite.Set_ImageAlpha(0, 128, 128);
	m_BowSprite.Set_ImageAlpha(0, 128, 128);
}

//!Overloaded event handler from Sprite class
int PlayerTile::Event_Handler()
{
	Update();
<<<<<<< HEAD
	m_SwordSprite.Update();
	m_BowSprite.Update();
=======
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31

	return 0;
}

//!Overloaded draw function from Sprite class
//In - 
//		float InputPlayerXPosition - the player x posiiton to draw at
//		float InputPlayerXPosition - the player y posiiton to draw at
<<<<<<< HEAD
void PlayerTile::Draw(int InputPlayerXPosition, int InputPlayerYPosition, bool SwordActive, bool BowActive)
=======
void PlayerTile::Draw(int InputPlayerXPosition, int InputPlayerYPosition)
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31
{
	//if there is a player tile image draw it at the specified input location
	if (m_Image != NULL)
	{
<<<<<<< HEAD
		if(SwordActive)
		{
			al_draw_bitmap_region(m_SwordSprite.Get_Image(), m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, InputPlayerXPosition, InputPlayerYPosition, 0);
		}
		
		else if(BowActive)
		{
			al_draw_bitmap_region(m_BowSprite.Get_Image(), m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, InputPlayerXPosition, InputPlayerYPosition, 0);
		}

		else
		{
			al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, InputPlayerXPosition, InputPlayerYPosition, 0);
		}
=======
		al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, InputPlayerXPosition, InputPlayerYPosition, 0);
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31
	}
}