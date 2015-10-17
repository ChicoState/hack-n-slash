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

	std::vector<TerrainTile> m_TileTypes; //Templates for all of the tiles in my dungeon (i.e walls, floors, etc..)

	TerrainMap *m_Map; //The "Graphical" 2D Array containing all the Tiles the dungeon is made of

	std::vector<std::vector<TILE>> m_Dungeon; //This is another 2D Array containing a simple version of all the Tile types the dungeon is made (i.e. Wall, Floor, Door, etc...)
	std::vector<std::vector<int>> m_Regions; //Utility for creating the dungeon

	std::list<Rect> m_Rooms; //A list of Rects that define where all the rooms in the dungeon are located. Only includes the inside of the room. Not the surrounding walls/Doors
	std::vector<Vec2i> m_Cardinal; //Utility for directions

	Vec2f m_StartPosition; //The players start position in the dungeon

	const int cm_DungeonWidth = 51; //The width of the dungeon. For now it is const for testing, this will be a random size later. Note: Size must be odd number
	const int cm_DungeonHeight = 31; //The height of the dungeon. For now it is const for testing, this will be a random size later. Note: Size must be odd number

public:
	DungeonGenerator(ALLEGRO_EVENT_QUEUE *EventQueue) : m_EventQueue(EventQueue)
	{
		m_Cardinal.push_back(Vec2i(0, 1));
		m_Cardinal.push_back(Vec2i(1, 0));
		m_Cardinal.push_back(Vec2i(0, -1));
		m_Cardinal.push_back(Vec2i(-1, 0));
	}
	~DungeonGenerator()
	{
		delete m_Map;
	}
	
	std::vector<std::vector<TILE>> GetDungeon() { return m_Dungeon; }
	std::vector<std::vector<int>> GetRegions() { return m_Regions;  }

	TILE Get_Tile(Vec2i Pos) { return m_Dungeon[Pos.x()][Pos.y()]; }
	void Set_Tile(Vec2i Pos, TILE tile) { m_Dungeon[Pos.x()][Pos.y()] = tile; }

	Vec2f GetStartPosition() { return m_StartPosition; }

	void GenerateDungeon(Display&);

	void Draw(); //This draw function will be changed quite a bit later once all of the Base classes needed have been created and once I get some actual graphics to use

private:
	
	
	void Carve(Vec2i pos, TILE tile);
	void AddRooms();
	void GrowMaze(Vec2i);
	bool CanCarve(Vec2i, Vec2i);
	void connectRegions();
	void AddJunction(Vec2i);
	void StreamLineCorridors();
	void RemoveDeadEnds();
	int BuildLineSegment(Vec2i, std::vector<Vec2i>&, std::vector<Vec2i>&, int, bool);

	void MultiplyDungeon(int);
	bool EraserFunc(std::vector<int>&, std::map<std::vector<int>, std::unordered_set<int>>&, std::map<int, int>&);

	void SetStartPosition();
	void InitMap(Display&);

	void PrintCurrentMap(); //Used for printing a command console version of the dungeon. Used for debugging.
};

#endif