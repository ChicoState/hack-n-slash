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
	ALLEGRO_BITMAP *m_Image; //The image should be created/generated elsewhere then. This class will handle the deletion of said bitmap

	int m_PosX; //world location in X where the tile is located
	int m_PosY; //world location in Y where the tile is located

	int m_MaxFramesX;   //The number of Tiles wide the sprite sheet is
	int m_MaxFramesY;   //The number of Tiles tall the sprite sheet is

	int m_Tile_Width;	//Width of the Sprite image to be used
	int m_Tile_Height;	//Height of the Sprite image to be used

	int m_CurColumn; //X location of where the image is located on a sprite sheet
	int m_CurRow; //The current row in the Sprite sheet

	bool m_Animated; //If the image is animated or not
	bool m_LoopedAnim; //should we continously loop this anim?
	bool m_Continuous; //should we continue to animate onto the next row of the sprite sheet? 
	int m_AnimFPS; //how quickly does the anim update?
	int m_FrameCount; //used for animated sprites.

	bool m_Collidable; //Should this sprite by accounted for in collisions?

public:
	Sprite(ALLEGRO_BITMAP *Image, int PosX, int PosY, int FrameWidth, int FrameHeight,
		bool Collidable = false, bool Animated = false, bool Continuous = false, bool Looped = false, int AnimFPS = 0)
		: m_Image(Image), m_PosX(PosX), m_PosY(PosY), m_Tile_Width(FrameWidth), m_Tile_Height(FrameHeight), 
		m_Collidable(Collidable), m_Animated(Animated), m_Continuous(Continuous), m_LoopedAnim(Looped), m_AnimFPS(AnimFPS)
	{
		m_CurColumn = 0;
		m_CurRow = 0;
		m_FrameCount = 0;
		m_MaxFramesX = (al_get_bitmap_width(m_Image) / m_Tile_Width) - 1;
		m_MaxFramesY = (al_get_bitmap_height(m_Image) / m_Tile_Height) - 1;

		if ((!m_Animated) || (m_MaxFramesY == 0))
			m_Continuous = false;
	}

	~Sprite()
	{
		al_destroy_bitmap(m_Image);
	}

	bool Get_IsAnimated(){ return m_Animated; }
	int Get_TileWidth(){ return m_Tile_Width; }
	int Get_TileHeight(){ return m_Tile_Height; }

	void Set_CurRow(int, bool);

	virtual void Draw();

	virtual void Update();

	virtual int Event_Handler(ALLEGRO_EVENT&);
};


#endif