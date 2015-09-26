//Created by:		Ryan Nolan-Hieb
//Date updated:		9/25/15
//Last update by:	N/A
//Reason for update:N/A

#ifndef __SPRITE__
#define __SPRITE__

#include <allegro5\allegro.h>
#include "Vec2.h"
#include <string>

class Sprite
{
private:

protected:
	ALLEGRO_BITMAP *m_Image;
	
	int m_MaxFramesX;   //The number of Tiles wide the sprite sheet is

	int m_Tile_Width;	//Width of the Sprite image to be used
	int m_Tile_Height;	//Height of the Sprite image to be used

	int m_CurColumn; //X location of where the image is located on a sprite sheet
	
	bool m_Animated; //If the image is animated or not
	bool m_LoopedAnim; //should we continously loop this anim?
	int m_AnimFPS; //how quickly does the anim update?

	bool m_Collidable; //Should this sprite by accounted for in collisions?

public:
	Sprite(int FrameWidth, int FrameHeight, 
		bool Collidable = false, bool Animated = false, bool Looped = false, int AnimFPS = 0)
		: m_Tile_Width(FrameWidth), m_Tile_Height(FrameHeight), m_Collidable(Collidable), m_Animated(Animated), 
		m_LoopedAnim(Looped), m_AnimFPS(AnimFPS)
	{
		//m_Image = al_load_bitmap("); //Since I'm not currently using Images for my Dungeon Generator this commented out. This should not be commented though!
		m_CurColumn = 0;
		m_MaxFramesX = 0;// al_get_bitmap_width(m_Image) / m_Tile_Width; //Same thing goes for here as well
	}

	~Sprite()
	{
		//al_destroy_bitmap(m_Image); //And here
	}

	//Pos = World Coordinates
	virtual void Draw(Vec2f Pos);

	virtual int Event_Handler(ALLEGRO_EVENT&);
};


#endif