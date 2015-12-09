//Created by:		Ryan Nolan-Hieb

#ifndef __SPRITE__
#define __SPRITE__

#include <allegro5\allegro.h>
#include "Vec2.h"
#include <string>



class Sprite
{
private:
	

protected:
	ALLEGRO_BITMAP *m_Image; //the sprite sheet image

	int m_PosX; //world location in X where the tile is located
	int m_PosY; //world location in Y where the tile is located

	int m_MaxFramesX;   //The number of Tiles wide the sprite sheet is
	int m_MaxFramesY;   //The number of Tiles tall the sprite sheet is

	int m_FrameWidth;	//Width of one frame
	int m_FrameHeight;	//Height of one frame

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
		: m_Image(Image), m_PosX(PosX), m_PosY(PosY), m_FrameWidth(FrameWidth), m_FrameHeight(FrameHeight),
		m_Collidable(Collidable), m_Animated(Animated), m_Continuous(Continuous), m_LoopedAnim(Looped), m_AnimFPS(AnimFPS)
	{
		m_CurColumn = 0;
		m_CurRow = 0;
		m_FrameCount = 0;
		if (Image)
		{
			m_MaxFramesX = (al_get_bitmap_width(m_Image) / m_FrameWidth) - 1;
			m_MaxFramesY = (al_get_bitmap_height(m_Image) / m_FrameHeight) - 1;
		}

		if ((!m_Animated) || (m_MaxFramesY == 0))
			m_Continuous = false;
	}

	~Sprite()
	{
	}

	//Basic setters and getters

	bool Get_IsAnimated(){ return m_Animated; }
	int Get_TileWidth(){ return m_FrameWidth; }
	int Get_TileHeight(){ return m_FrameHeight; }
	bool Get_Collidable() { return m_Collidable; }
	int Get_CurRow(){ return m_CurRow; }
	int Get_CurColumn(){ return m_CurColumn; }
	int Get_TilesWide(){ return m_MaxFramesX; }
	int Get_TilesHigh(){ return m_MaxFramesY; }

	ALLEGRO_BITMAP *Get_Image(){ return m_Image; }

	void Set_CurRow(int Row, bool ResetCurColumn = true);
	void Set_ImageAlpha(int, int, int);

	//Core functions

	virtual void Draw(); //Draws the sprite

	virtual void Update(); //Updates the animation of a sprite if it's animtated

	virtual int Event_Handler(ALLEGRO_EVENT&); //handles events
};


#endif