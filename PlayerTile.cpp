//Scott Backer

//Scott Backer

//PlayerTile Class CPP: Handles the sprite control for the player character

#include "PlayerTile.h"

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
PlayerTile::PlayerTile(int InputPlayerXPosition, int InputPlayerYPosition, int FrameWidth, int FrameHeight,
		bool Collidable = true, bool Animated = true, bool Continuous = false, bool Looped = true, int AnimFPS =  6)
		: Sprite(al_load_bitmap("Player_Movement_Sprite_BIG.png"), InputPlayerXPosition, InputPlayerYPosition, FrameWidth, FrameHeight, Collidable, Animated, Continuous, Looped, AnimFPS),
		 m_SwordSprite(al_load_bitmap("Player_Sword_Sprite_BIG.png"), InputPlayerXPosition, InputPlayerYPosition, FrameWidth, FrameHeight, Collidable, Animated, Continuous, Looped, AnimFPS),
		 m_BowSprite(al_load_bitmap("Player_Bow_Sprite_BIG.png"), InputPlayerXPosition, InputPlayerYPosition, FrameWidth, FrameHeight, Collidable, Animated, Continuous, Looped, AnimFPS)
{
	//set the alpha of the sprite
	Sprite::Set_ImageAlpha(0, 128, 128);
	m_SwordSprite.Set_ImageAlpha(0, 128, 128);
	m_BowSprite.Set_ImageAlpha(0, 128, 128);

	//initialize member variables
	m_BowDrawActive = false;
}

//!Overloaded event handler from Sprite class
int PlayerTile::Event_Handler()
{
	//update the sprites
	Update();
	m_SwordSprite.Update();
	m_BowSprite.Update();

	return 0;
}

//!Overloaded draw function from Sprite class
//In - 
//		float InputPlayerXPosition - the player x posiiton to draw at
//		float InputPlayerXPosition - the player y posiiton to draw at
//		bool SwordActive - true if the player's sword attack is active else false
//		bool BowActive - true if the player's bow is active else ffalse
//		bool BowNewActivation - true if its a new bow shot activation else false
void PlayerTile::Draw(int InputPlayerXPosition, int InputPlayerYPosition, bool SwordActive, bool BowActive, bool BowNewActivation)
{
	//if the bow is active the first frame
	if(BowNewActivation)
	{
		m_BowDrawActive = true;
	}

	//if there is a player tile image draw it at the specified input location
	if (m_Image != NULL)
	{
		//draw sword
		if(SwordActive)
		{
			al_draw_bitmap_region(m_SwordSprite.Get_Image(), m_SwordSprite.Get_CurColumn() * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, InputPlayerXPosition, InputPlayerYPosition, 0);
		}
		
		//draw bow
		else if(BowActive)
		{
			al_draw_bitmap_region(m_BowSprite.Get_Image(), m_BowSprite.Get_CurColumn() * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, InputPlayerXPosition, InputPlayerYPosition, 0);

			//if the bow animation is finished set it to false
			if(m_BowSprite.Get_CurColumn() == m_BowSprite.Get_TilesWide())
			{
				m_BowDrawActive = false;
			}
		}

		//else draw no weapon
		else
		{
			al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, InputPlayerXPosition, InputPlayerYPosition, 0);
		}
	}
}