//Created by:		Ryan Nolan-Hieb

//Gathered from http://journal.stuffwithstuff.com/2014/12/21/rooms-and-mazes/
//converted to C++ by Ryan Nolan-Hieb

/// The random dungeon generator.
///
/// Starting with a stage of solid walls, it works like so:
///
/// 1. Place a number of randomly sized and positioned rooms. If a room
///	overlaps an existing room, it is discarded. Any remaining rooms are
///	carved out.
/// 2. Any remaining solid areas are filled in with mazes. The maze generator
///	will grow and fill in even odd-shaped areas, but will not touch any
///	rooms.
/// 3. The result of the previous two steps is a series of unconnected rooms
///	and mazes. We walk the stage and find every tile that can be a
///	"connector". This is a solid tile that is adjacent to two unconnected
///	regions.
/// 4. We randomly choose connectors and open them or place a door there until
///	all of the unconnected regions have been joined. There is also a slight
///	chance to carve a connector between two already-joined regions, so that
///	the dungeon isn't single connected.
/// 5. The mazes will have a lot of dead ends. Finally, we remove those by
///	repeatedly filling in any open tile that's closed on three sides. When
///	this is done, every corridor in a maze actually leads somewhere.
///
/// The end result of this is a multiply-connected dungeon with rooms and lots
/// of winding corridors.


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
	int m_X;		//Starting X position of the rectangle
	int m_Y;		//Starting Y position of the rectangle
	int m_Width;	//Width of the rectangle
	int m_Height;	//Height of the rectangle

public:

	Rect(int X, int Y, int Width, int Height) : m_X(X), m_Y(Y), m_Width(Width), m_Height(Height)
	{}

	//returns true if the passed in position is contained within the boundaries of the rectangle
	bool ContainsPoint(int x, int y) {
		return !(x < m_X || y < m_Y || x > Get_X2() || y > Get_Y2());
	}

	int Get_X2() { return m_X + m_Width; }  //Get the X2 position of the rectangle
	int Get_Y2() { return m_Y + m_Height; }	//Get the Y2 position of the rectangle
	int Get_X1() { return m_X; }			//Get the X1 position of the rectangle
	int Get_Y1() { return m_Y; }			//Get the Y1 position of the rectangle

	//operator overloads for == and !=
	bool operator== (const Rect &RHS) { return (m_X == RHS.m_X || m_Y == RHS.m_Y || m_Width == RHS.m_Width || m_Height == RHS.m_Height); }
	bool operator!= (const Rect &RHS) { return !(*this == RHS); }
	
	//returns true if any point of another rectangle is contained within this one
	bool Intersects(Rect B){ return !(m_X > B.Get_X2() || m_Y > B.Get_Y2() || this->Get_X2() < B.m_X || this->Get_Y2() < B.m_Y); }
};


class DungeonGenerator
{
private:
	ALLEGRO_EVENT_QUEUE *m_EventQueue; //Allegro Event Queue
	ALLEGRO_EVENT_SOURCE m_SpawnBossEvent; //Allegro Event to tell the game to spawn the boss of the dungeon

	TerrainMap *m_Map; //The "Graphical" 2D Array containing all the Tiles the dungeon is made of

	std::vector<std::vector<TILE>> m_Dungeon; //This is another 2D Array containing a simple version of all the Tile types the dungeon is made (i.e. Wall, Floor, Door, etc...)
	std::vector<std::vector<int>> m_Regions; //Utility for creating the dungeon

	std::list<Rect> m_Rooms; //A list of Rects that define where all the rooms in the dungeon are located. Only includes the inside of the room. Not the surrounding walls/Doors
	std::vector<AVec2i> m_Cardinal; //Utility for directions

	AVec2f m_StartPosition; //The players start position in the dungeon
	AVec2f m_BossPortal; //spawn point for the boss portal

	int m_Level; //The level the player is on/number of dungeons completed
	
	
	int m_CurrentRegion; //Used internally to keep track of which parts still need to combined or removed from the main dungeon
	int m_NumRoomTries; //The number of times a room will be created and tried to be randomly placed inside the dungeon.
	int m_ExtraConnectorChance; //An extra chance for doors to be created
	int m_RoomExtraSize; //An additional size to be applied to rooms
	int m_WindingPercent; //A percentage for how windy the maze sections should be

	const int cm_TileSize = 128; //The size, in pixels, of each tile in the dungeon
	const int m_DungeonWidth = 51; //The width of the dungeon. Note: Size must be odd number
	const int m_DungeonHeight = 31; //The height of the dungeon. Note: Size must be odd number

	Player *m_MainPlayer; //Pointer to the main player in the game

	ALLEGRO_BITMAP *m_DungeonTiles; //The images containing all of the tiles need to graphically fill the dungeon

public:
	DungeonGenerator(ALLEGRO_EVENT_QUEUE *EventQueue, Player *MainPlayer) : m_EventQueue(EventQueue), m_MainPlayer(MainPlayer), 
		m_DungeonTiles(al_load_bitmap("DungeonTiles.png"))
	{
		m_Cardinal.push_back(AVec2i(0, 1));  //North
		m_Cardinal.push_back(AVec2i(1, 0));	//East
		m_Cardinal.push_back(AVec2i(0, -1));	//South
		m_Cardinal.push_back(AVec2i(-1, 0));	//West	

		m_Level = 1;
		m_CurrentRegion = -1;

		al_init_user_event_source(&m_SpawnBossEvent);
		al_register_event_source(m_EventQueue, &m_SpawnBossEvent);

	}
	~DungeonGenerator()
	{
		delete m_Map;
	}

	void GenerateDungeon(Display&); //The call the user makes to start generating a new dungeon

	void Event_Handler(ALLEGRO_EVENT &EV); //Handles the events for the dungeon
	void Draw(bool PrePlayerDraw); //Draws the dungeon
	
	//basic Setters and Getters
	std::vector<std::vector<TILE>> GetDungeon() { return m_Dungeon; }
	std::vector<std::vector<int>> GetRegions() { return m_Regions;  }

	TILE Get_Tile(AVec2i Pos) { return m_Dungeon[Pos.x()][Pos.y()]; }
	void Set_Tile(AVec2i Pos, TILE tile) { m_Dungeon[Pos.x()][Pos.y()] = tile; }

	TerrainMap* Get_Map(){ return m_Map; }

	std::list<Rect> Get_Rooms() { return m_Rooms; }

	AVec2f GetStartPosition() { return AVec2f(m_StartPosition.x() * cm_TileSize, m_StartPosition.y() * cm_TileSize); }
	
	int Get_DungeonWidth(){ return m_DungeonWidth; }
	int Get_DungeonHeight(){ return m_DungeonHeight; }

	int Get_DungeonLevel() { return m_Level; }
	void Set_DungeonLevel(int Level) { m_Level = Level; }

private:
	//Basic helper functions for creating the basic dungeon layout (no graphics)

	void GenerateDungeonVars(); //Randomizes variables that will determine the overall shape of the dungeon
	void Carve(AVec2i pos, TILE tile); //Carves a tile into a new tile type
	void AddRooms(); //Places rooms throughout dungeon
	void GrowMaze(AVec2i);  //Spanning tree algorithm to create a maze between all of the rooms that get created
	bool CanCarve(AVec2i, AVec2i); //given a position and a direction, returns if you can carve in that direction
	void connectRegions(); //connects the different sections of the maze together
	void AddJunction(AVec2i); //creates a door at the given position
	void RemoveDeadEnds(); //gets rid of long coridors that lead to nothing
	bool EraserFunc(std::vector<int>&, std::map<std::vector<int>, std::unordered_set<int>>&, std::map<int, int>&); //helper function for merging different parts of the dungeon


	//Basic helper function to help graphically create the dungeon

	void SetStartPosition();		//Selects a room on the left side of the map where the player will spawn
	void SetBossPortalSpawn();		//Selects a room on the right side of the map to place the Boss Spawn Portal
	void InitMap(Display&);			//Places the first layer of the m_Map containing the basic textures around the level
	void DecorateMap(Display&);		//Places the second layer of the m_Map containing the basic textures the destructible objects throughout the dungeons
	void CreateFog(Display&);		//Places the third layer of the m_Map containing the basic textures for creating fog in all of the rooms
	void WallOffBossRoom();			//function used to wall off the room that contains the boss to prevent players from running away

	TerrainTile MakeWall(AVec2i);	//Helper function to decide which wall sprite to use
	TerrainTile MakeDoor(AVec2i);	//Helper function to decide which door sprite to use
	TerrainTile MakeFloor(AVec2i);	//Helper function to decide which floor sprite to use
	
	void PrintCurrentMap(); //Used for printing a command console version of the dungeon. Used for debugging.
};

#endif