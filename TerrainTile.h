//Created by:		Ryan Nolan-Hieb
//Date updated:		9/25/15
//Last update by:	N/A
//Reason for update:N/A

#ifndef __TERRAIN_TILE__
#define __TERRAIN_TILE__

#include "Sprite.h"

//Terrain Tile types. More to be added later!
enum TILE{ Blank = -1, Floor, Wall, Door, OpenDoor, ClosedDoor, Grass, Dirt, Water };


class TerrainTile : public Sprite
{
private:
	
	bool m_TriggerTile; //Does this tile trigger an event when the player walks over it?
	//m_TriggerType //A trigger Enum will be created at some point for triggerable tiles.

	TILE m_TileType; //Type of tile

	bool m_debugDraw;

public:

	TerrainTile(ALLEGRO_BITMAP *Image, int PosX, int PosY, TILE TileType, int FrameWidth, int FrameHeight,
		bool Collidable = false, bool TriggerTile = false, bool Animated = false, bool Continuious = false, bool Looped = false, int AnimFPS = 0)
		: Sprite(Image, PosX, PosY, FrameWidth, FrameHeight, Collidable, Animated, Continuious, Looped, AnimFPS),
		m_TriggerTile(m_TriggerTile), m_TileType(TileType)
	{
	}
	~TerrainTile()
	{
	}

	virtual void Draw();
	virtual int Event_Handler(ALLEGRO_EVENT&);
};



#endif