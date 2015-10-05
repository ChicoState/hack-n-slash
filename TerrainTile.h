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
	int m_PosX;
	int m_PosY;

	bool m_TriggerTile;
	//m_TriggerType //A trigger Enum will be created at some point for triggerable tiles.

	TILE m_TileType;

public:

	TerrainTile(int PosX, int PosY, TILE TileType, int FrameWidth, int FrameHeight,
		bool Collidable = false, bool TriggerTile = false, bool Animated = false, bool Looped = false, int AnimFPS = 0)
		: Sprite(FrameWidth, FrameHeight, Collidable, Animated, Looped, AnimFPS), m_PosX(PosX), m_PosY(PosY),
		m_TriggerTile(m_TriggerTile), m_TileType(TileType)
	{
	}
	TILE getTileType(){ return m_TileType; }
	virtual void Draw();
};

#endif