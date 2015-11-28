//Created by:		Ryan Nolan-Hieb
//Date updated:		9/25/15
//Last update by:	N/A
//Reason for update:N/A

#ifndef __DUNGEON_GENERATOR__
#define __DUNGEON_GENERATOR__

#include "Vec2.h"
#include <vector>
#include <list>
#include <random>
#include <unordered_map>
#include <map>
#include <unordered_set>

#include "allegro5\allegro.h"
#include "allegro5\allegro_primitives.h"

#include "TerrainLayer.h"
#include "TerrainTile.h"
#include "TerrainMap.h"
#include "Player.h"


//Definition of a simple rectangle class. 
//Used for creating Rooms in the Dungeon Generator
class Rect
{
private:
	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;

public:

	Rect(int X, int Y, int Width, int Height) : m_X(X), m_Y(Y), m_Width(Width), m_Height(Height)
	{}

	bool ContainsPoint(int x, int y) {
		return !(x < m_X || y < m_Y || x > m_Width || y > m_Height);
	}

	int Get_X2() { return m_X + m_Width; }
	int Get_Y2() { return m_Y + m_Height; }
	int Get_X1() { return m_X; }
	int Get_Y1() { return m_Y; }

	bool operator== (const Rect &RHS) { return (m_X == RHS.m_X || m_Y == RHS.m_Y || m_Width == RHS.m_Width || m_Height == RHS.m_Height); }
	bool operator!= (const Rect &RHS) { return !(*this == RHS); }
	
	bool Intersects(Rect B){ return !(m_X > B.Get_X2() || m_Y > B.Get_Y2() || this->Get_X2() < B.m_X || this->Get_Y2() < B.m_Y); }
};

class DungeonGenerator
{
private:
	ALLEGRO_EVENT_QUEUE *m_EventQueue; //Allegro Event Queue

	TerrainMap *m_Map; //The "Graphical" 2D Array containing all the Tiles the dungeon is made of

	std::vector<std::vector<TILE>> m_Dungeon; //This is another 2D Array containing a simple version of all the Tile types the dungeon is made (i.e. Wall, Floor, Door, etc...)
	std::vector<std::vector<int>> m_Regions; //Utility for creating the dungeon

	std::list<Rect> m_Rooms; //A list of Rects that define where all the rooms in the dungeon are located. Only includes the inside of the room. Not the surrounding walls/Doors
	std::vector<Vec2i> m_Cardinal; //Utility for directions

	Vec2f m_StartPosition; //The players start position in the dungeon
	Vec2f m_BossPortal; //spawn point for the boss portal

	int m_Level; //The level the player is on
	
	
	int m_CurrentRegion; 
	int m_NumRoomTries; //The number of times a room will be created and tried to be randomly placed inside the dungeon.
	int m_ExtraConnectorChance; //An extra chance for doors to be created
	int m_RoomExtraSize; //An additional size to be applied to rooms
	int m_WindingPercent; //A percentage for how windy the maze sections should be

	const int cm_TileSize = 128; //The size, in pixels, of each tile in the dungeon
	const int cm_DungeonWidth = 51; //The width of the dungeon. For now it is const for testing, this will be a random size later. Note: Size must be odd number
	const int cm_DungeonHeight = 31; //The height of the dungeon. For now it is const for testing, this will be a random size later. Note: Size must be odd number

	Player *m_MainPlayer;

	ALLEGRO_BITMAP *m_DungeonTiles;

public:
	DungeonGenerator(ALLEGRO_EVENT_QUEUE *EventQueue, Player *MainPlayer) : m_EventQueue(EventQueue), m_MainPlayer(MainPlayer), 
		m_DungeonTiles(al_load_bitmap("DungeonTiles.png"))
	{
		m_Cardinal.push_back(Vec2i(0, 1));
		m_Cardinal.push_back(Vec2i(1, 0));
		m_Cardinal.push_back(Vec2i(0, -1));
		m_Cardinal.push_back(Vec2i(-1, 0));

		m_Level = 1;
		m_CurrentRegion = -1;
	}
	~DungeonGenerator()
	{
		delete m_Map;
	}
	
	std::vector<std::vector<TILE>> GetDungeon() { return m_Dungeon; }
	std::vector<std::vector<int>> GetRegions() { return m_Regions;  }

	TILE Get_Tile(Vec2i Pos) { return m_Dungeon[Pos.x()][Pos.y()]; }
	void Set_Tile(Vec2i Pos, TILE tile) { m_Dungeon[Pos.x()][Pos.y()] = tile; }

	TerrainMap* Get_Map(){ return m_Map; }

	std::list<Rect> Get_Rooms() { return m_Rooms; }

	Vec2f GetStartPosition() { return Vec2f(m_StartPosition.x() * cm_TileSize, m_StartPosition.y() * cm_TileSize); }

	int Get_DungeonLevel() { return m_Level; }
	void Set_DungeonLevel(int Level) { m_Level = Level; }

	void GenerateDungeon(Display&);

	void Event_Handler(ALLEGRO_EVENT &EV);
	void Draw();

private:
	
	
	void Carve(Vec2i pos, TILE tile);
	void AddRooms();
	void GrowMaze(Vec2i);
	bool CanCarve(Vec2i, Vec2i);
	void connectRegions();
	void AddJunction(Vec2i);
	void RemoveDeadEnds();
	void GenerateDungeonVars();

	bool EraserFunc(std::vector<int>&, std::map<std::vector<int>, std::unordered_set<int>>&, std::map<int, int>&);

	void SetStartPosition();
	void SetBossPortalSpawn();
	void InitMap(Display&);
	void DecorateMap(Display&);

	TerrainTile MakeWall(Vec2i);
	TerrainTile MakeDoor(Vec2i);
	TerrainTile MakeFloor(Vec2i);
	
	void PrintCurrentMap(); //Used for printing a command console version of the dungeon. Used for debugging.
};

//used for generating a random number between two values. Should probably be moved to a different file but again, I'm lazy :P.
//int Random(int, int);

#endif