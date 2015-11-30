//Created by:		Ryan Nolan-Hieb
//Date updated:		9/25/15
//Last update by:	N/A
//Reason for update:N/A

#ifndef __TERRAIN_TILE__
#define __TERRAIN_TILE__

#include "Sprite.h"

//Terrain Tile types. More to be added later!
enum TILE{ Blank = -1, Floor, Wall, Door, OpenDoor, ClosedDoor, Grass, Dirt, Water };
enum TRIGGER{TR_NONE = -1, TR_SPIKE, TR_BOSS, TR_LOOT, TR_RETURN};

//A single Tile that makes up the terrain.
class TerrainTile : public Sprite
{
private:	
	bool m_TriggerTile; //Does this tile trigger an event when the player walks over it?
	TRIGGER m_TriggerType; //A trigger Enum will be created at some point for triggerable tiles.

	TILE m_TileType; //Type of tile

	//If the user would like some Tile from a sprite sheet, and only that tile, he may specifiy the X and Y positions of that tile 
	//and only that tile will be drawn
	int m_StartFrameX; 
	int m_StartFrameY;

	int m_HP; //The health of the tile. A tile with <= 0 HP is considered retired. A tile with > 1000 HP is considered undestuctible
	bool m_Retired;

	ALLEGRO_EVENT_SOURCE m_TerrainTriggerEvent; //If m_TriggerTile, then this is the custom event for broadcasting the trigger when it fires
	ALLEGRO_EVENT_QUEUE *m_EventQueue;

	bool m_debugDraw;

public:

	TerrainTile(ALLEGRO_BITMAP *Image, int PosX, int PosY, TILE TileType, int FrameWidth, int FrameHeight, bool Collidable = false,
		int StartFrameX = 0, int StartFrameY = 0, bool TriggerTile = false, TRIGGER TriggerType = TR_NONE, ALLEGRO_EVENT_QUEUE *EventQueue = NULL,
		int HP = 1001, bool Animated = false, bool Continuious = false, bool Looped = false, int AnimFPS = 0)
		: Sprite(Image, PosX, PosY, FrameWidth, FrameHeight, Collidable, Animated, Continuious, Looped, AnimFPS),
		m_TriggerTile(TriggerTile), m_TriggerType(TriggerType), m_TileType(TileType), m_HP(HP), m_EventQueue(EventQueue)
	{
		m_StartFrameX = StartFrameX;
		m_StartFrameY = StartFrameY;
		m_Retired = false;

		if (m_EventQueue && m_TriggerType != TR_NONE)
		{
			al_init_user_event_source(&m_TerrainTriggerEvent);
			al_register_event_source(m_EventQueue, &m_TerrainTriggerEvent);
		}
		else
		{
			TriggerTile = false;
		}

	}
	~TerrainTile()
	{
		if (m_TriggerTile)
		{
			//al_unregister_event_source(m_EventQueue, &m_TerrainTriggerEvent);
		}
	}

	bool Get_TriggerTile(){ return m_TriggerTile; }
	bool CheckCollision(Vec2f);
	bool Get_Retired(){ return m_Retired; }

	void Set_TileHP(int HP){ m_HP = HP; }
	int Get_TileHP(){ return m_HP; }

	TILE Get_TileType(){ return m_TileType; }
	TRIGGER Get_TriggerType() { return m_TriggerType; }

	void RetireTile();
	virtual void Draw();
	virtual int Event_Handler(ALLEGRO_EVENT&);
};



#endif