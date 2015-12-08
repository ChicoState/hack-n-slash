//Created by:	Ryan Nolan-Hieb

#ifndef __TERRAIN_TILE__
#define __TERRAIN_TILE__

#include "Sprite.h"

//Terrain Tile types.
enum TILE{ Blank = -1, Floor, Wall, Door, OpenDoor, ClosedDoor, Grass, Dirt, Water, Fog, Shrub, Rock, Tree, DungeonEntrance };

//Terrain Tile trigger types. These are different events a tile can fire off
enum TRIGGER{TR_NONE = -1, TR_SPIKE, TR_BOSS, TR_LOOT, TR_RETURN, TR_FOG};

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
	bool m_Retired; //if the tile is retired or not

	ALLEGRO_EVENT_SOURCE m_TerrainTriggerEvent; //If m_TriggerTile, then this is the custom event for broadcasting the trigger when it fires
	ALLEGRO_EVENT_QUEUE *m_EventQueue; //pointer to the event queue, needed for custom events

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

		if (m_EventQueue && m_TriggerType != TR_NONE) //ensuring everything is set correctly to have a valid trigger
		{
			al_init_user_event_source(&m_TerrainTriggerEvent);
			al_register_event_source(m_EventQueue, &m_TerrainTriggerEvent);
		}
		else //if anything isn't set right then it's not a trigger tile
		{
			TriggerTile = false;
		}

	}
	~TerrainTile()
	{
	}

	//Basic setters and getters
	bool Get_TriggerTile(){ return m_TriggerTile; }
	bool Get_Retired(){ return m_Retired; }

	void Set_TileHP(int HP){ m_HP = HP; }
	int Get_TileHP(){ return m_HP; }

	TILE Get_TileType(){ return m_TileType; }
	TRIGGER Get_TriggerType() { return m_TriggerType; }


	//Core functions
	bool CheckCollision(AVec2f); //checks to see if the given point is contained within this tile
	void RetireTile(); //retires this tile. Basically removing it's ability to collide, trigger events, and be drawn
	
	virtual void Draw(); //Draws the tile
	virtual int Event_Handler(ALLEGRO_EVENT&); //Handles events for the tile

private:
	void EmitEvent(ALLEGRO_EVENT&); //helper function for emitting custom events
};



#endif