//Created by:		Ryan Nolan-Hieb

#include "DungeonGenerator.h"
#include "Utility.h"

#include <algorithm>
#include <iostream>

void DungeonGenerator::GenerateDungeonVars()
{
	m_NumRoomTries = Random(100, 200); //The number of times a room will be created and tried to be randomly placed inside the dungeon.
	m_ExtraConnectorChance = Random(3,6); //An extra chance for doors to be created
	m_RoomExtraSize = Random(3,6); //An additional size to be applied to rooms
	m_WindingPercent = Random(45,70); //A percentage for how windy the maze sections should be
}

void DungeonGenerator::GenerateDungeon(Display &MainDisplay)
{
	//These are here for when a new dungeon is generated things get set back to default and all previously stored data is cleared.
	m_CurrentRegion = -1;
	m_Dungeon.clear();
	m_Regions.clear();
	m_Rooms.clear();

	if (m_Map)
	{
		delete m_Map;
		m_Map = NULL;
	}

	//Generate some random variables that will help shape the overall dungeon
	GenerateDungeonVars();

	m_Dungeon.resize(m_DungeonWidth, std::vector<TILE>(m_DungeonHeight, Wall));				//fills the dungeon up with temp tiles
	m_Regions.resize(m_DungeonWidth, std::vector<int>(m_DungeonHeight, m_CurrentRegion));	//same thing with regions
	
	AddRooms(); //Adds rooms to the dungeon


	//grows mazes where there aren't any rooms
	for (int x = 1; x < m_DungeonWidth; x += 2)
	{
		for (int y = 1; y < m_DungeonHeight; y += 2)
		{
			if (Get_Tile(AVec2i(x, y)) != Wall)
			{
				continue;
			}
			
			GrowMaze(AVec2i(x,y));
		}
	}

	//connects the rooms and the mazes together
	connectRegions();

	//cleans up any long corridors that don't lead anywhere
	RemoveDeadEnds();

	//sets the boss and player spawn zones
	SetStartPosition();
	SetBossPortalSpawn();

	//creates the pretty graphical dungeon based on the previously calculated data
	InitMap(MainDisplay);
	DecorateMap(MainDisplay);
	CreateFog(MainDisplay);
}

//debug printing, prints the non graphical dungeon to CMD console
void DungeonGenerator::PrintCurrentMap()
{
	for (int y = 0; y < m_DungeonHeight; y++)
	{
		for (unsigned int x = 0; x < m_Dungeon.size(); x++)
		{
			if (m_Dungeon[x][y] == Wall)
			{
				std::cout << '0'; //walls are 0s
			}
			else if (m_Dungeon[x][y] == Door)
			{
				std::cout << '2'; //doors are 2s
			}
			else
			{
				std::cout << ' '; //everything else (open space) is just an empty character
			}
		}
		std::cout << std::endl;
	}
}

void DungeonGenerator::connectRegions()
{
	std::map<std::vector<int>, std::unordered_set<int>> connectorRegions;
	
	for (int ix = 1; ix < m_DungeonWidth - 1; ix++)
	{
		for (int iy = 1; iy < m_DungeonHeight - 1; iy++)
		{
			AVec2i Pos(ix, iy);
			if (Get_Tile(Pos) != Wall)
				continue;
			
			std::unordered_set<int> Regions;
			
			for (unsigned int i = 0; i < m_Cardinal.size(); i++)
			{
				AVec2i Indexer = (Pos + m_Cardinal[i]);
				int Region = m_Regions[Indexer.x()][Indexer.y()];
				if (Region != -1) Regions.insert(Region);
			}
			
			if (Regions.size() < 2)
			{
				continue;
			}
			
			std::vector<int> Temp;
			Temp.push_back(Pos.x());
			Temp.push_back(Pos.y());
			connectorRegions[Temp] = Regions;
			
		}
	}
	
	std::vector<std::vector<int>> connectors;
	
	for (std::map<std::vector<int>, std::unordered_set<int>>::iterator it = connectorRegions.begin(); it != connectorRegions.end(); ++it)
	{
		connectors.push_back(it->first);
	}
	
	std::map<int, int> merged;
	std::unordered_set<int> openRegions;

	for (int i = 0; i <= m_CurrentRegion; i++)
	{
		merged[i] = i;
		openRegions.insert(i);
	}
	
	while (openRegions.size() > 1)
	{
		std::vector<int> Temp;
		Temp = connectors[Random(0, connectors.size())];
		AVec2i connector(Temp[0], Temp[1]);

		AddJunction(connector);

		std::vector<int> Regions;
		
		for (std::unordered_set<int>::iterator it = connectorRegions[Temp].begin(); it != connectorRegions[Temp].end(); ++it)
		{
			Regions.push_back(merged[*it]);
		}
		
		int dest = Regions[0];
		std::vector<int> Sources;

		for (std::vector<int>::iterator it = Regions.begin() + 1; it != Regions.end(); ++it)
		{
			Sources.push_back(*it);
		}

		for (int i = 0; i <= m_CurrentRegion; i++)
		{
			for (unsigned int x = 0; x < Sources.size(); x++)
			{
				if (Sources[x] == merged[i])
				{
					merged[i] = dest;
				}
			}
		}

		
		for (std::vector<int>::iterator it = Sources.begin(); it != Sources.end(); ++it)
		{
			openRegions.erase(*it);
		}
		
		for (auto it = connectors.begin(); it != connectors.end();)
		{
			if (EraserFunc(*it, connectorRegions, merged))
			{
				it = connectors.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
}

bool DungeonGenerator::EraserFunc(std::vector<int> &Pos, std::map<std::vector<int>, std::unordered_set<int>> &connectorRegions,
	std::map<int, int> &merged)
{
	AVec2i tmp(Pos[0], Pos[1]);

	std::unordered_set<int> regionss;

	for (std::unordered_set<int>::iterator it = connectorRegions[Pos].begin(); it != connectorRegions[Pos].end(); ++it)
	{
		regionss.insert(merged[*it]);
	}

	if (regionss.size() > 1)
	{
		return false;
	}

	if (Random(0, 101) < m_ExtraConnectorChance)
	{
		for (unsigned int i = 0; i < m_Cardinal.size(); i++)
		{
			AVec2i indexer(tmp + m_Cardinal[i]);
			if (m_Dungeon[indexer.x()][indexer.y()] == Door)
			{
				return true;
			}
		}

		AddJunction(tmp);
	}

	return true;
}

void DungeonGenerator::RemoveDeadEnds()
{
	bool Done = false;

	while (!Done)
	{
		Done = true;

		for (int ix = 1; ix < m_DungeonWidth - 1; ix++)
		{
			for (int iy = 1; iy < m_DungeonHeight - 1; iy++)
			{
				AVec2i Pos(ix, iy);

				if (Get_Tile(Pos) == Wall)
				{
					continue;
				}

				// If it only has one exit, it's a dead end.
				int exits = 0;

				for (unsigned int i = 0; i < m_Cardinal.size(); i++)
				{
					if (Get_Tile(Pos + m_Cardinal[i]) != Wall)
					{
						exits++;
					}
				}

				if (exits != 1)
				{
					continue;
				}

				Done = false;
				Set_Tile(Pos, Wall);
				m_Regions[Pos.x()][Pos.y()] = -1;
			}
		}
	}
}

void DungeonGenerator::AddJunction(AVec2i Pos)
{
	Set_Tile(Pos, Door);
}

void DungeonGenerator::GrowMaze(AVec2i Start)
{
	std::vector<AVec2i> Cells;
	AVec2i LastDir;

	m_CurrentRegion++;

	Carve(Start, Floor);
	
	Cells.push_back(Start);

	while (!Cells.empty())
	{
		AVec2i cell = Cells.back();

		std::vector<AVec2i> UnmadeCells;

		// See which adjacent cells are open.
		for (std::vector<AVec2i>::iterator it = m_Cardinal.begin(); it != m_Cardinal.end(); ++it)
		{
			if (CanCarve(cell, *it))
			{
				UnmadeCells.push_back(*it);
			}
		}

		if (!UnmadeCells.empty())
		{
			// Based on how "windy" passages are, try to prefer carving in the
			// same direction.
			AVec2i dir;
			bool contains = false;
			for (std::vector<AVec2i>::iterator it = UnmadeCells.begin(); it != UnmadeCells.end(); ++it)
			{
				if (*it == LastDir)
				{
					contains = true;
					break;
				}
			}

			if (contains && Random(0, 101) > m_WindingPercent)
			{
				dir = LastDir; //keep previous direction
			}
			else
			{
				dir = UnmadeCells[Random(0, UnmadeCells.size())]; //pick new direction out of possible ones
			}

			Carve(cell + dir, Floor);	  //carve out wall between the valid cells
			Carve(cell + dir * 2, Floor); //carve out valid cell

			Cells.push_back(cell + dir * 2);
			LastDir = dir;
		}
		else
		{
			// No adjacent uncarved cells.
			Cells.pop_back(); //Remove Last element
			LastDir = NULL; // This path has ended.
		}

	}
}

void DungeonGenerator::AddRooms()
{
	std::list<Rect> TempRooms;

	for (int i = 0; i < m_NumRoomTries; i++)
	{
		// Pick a random room size. The funny math here does two things:
		// - It makes sure rooms are odd-sized to line up with maze.
		// - It avoids creating rooms that are too rectangular: too tall and
		//   narrow or too wide and flat.
		int Size = Random(1, 3 + m_RoomExtraSize) * 2 + 1;
		int Rectangularity = Random(0, 1 + Size / 2) * 2;
		int width = Size;
		int height = Size;

		if (rand() % 2 == 0) //50% chance
		{
			width += Rectangularity;
		}
		else
		{
			height += Rectangularity;
		}

		int x = Random(0, (m_DungeonWidth - width) / 2) * 2 + 1;
		int y = Random(0, (m_DungeonHeight - height) / 2) * 2 + 1;

		Rect TempRoom(x, y, width, height);

		bool overlaps = false;
		for (std::list<Rect>::iterator it = TempRooms.begin(); it != TempRooms.end(); it++)
		{
			if (TempRoom.Intersects(*it))
			{
				overlaps = true;
				break;
			}
		}

		if (overlaps) continue; //don't add room and retry

		if (x + width > m_DungeonWidth || y + height > m_DungeonHeight)
		{
			std::cout << "room generation error" << std::endl;
		}

		TempRooms.push_back(TempRoom); //add non-overlapping room

		m_CurrentRegion++;

		for (int ix = x; ix < x + width; ix++)
		{
			for (int iy = y; iy < y + height; iy++)
			{
				Carve(AVec2i(ix, iy), Floor);
			}
		}
	}

	m_Rooms = TempRooms;
}

// Gets whether or not an opening can be carved from the given starting
// [Cell] at [pos] to the adjacent Cell facing [direction]. Returns `true`
// if the starting Cell is in bounds and the destination Cell is filled
// (or out of bounds).
bool DungeonGenerator::CanCarve(AVec2i Pos, AVec2i Direction)
{
	AVec2i iv2 = Pos + Direction * 3;

	Rect Bounds(0, 0, m_DungeonWidth, m_DungeonHeight);

	if (!Bounds.ContainsPoint(iv2.x(), iv2.y())) return 0;

	// Destination must not be open.
	return (Get_Tile(Pos + Direction * 2) == Wall);
}

void DungeonGenerator::Carve(AVec2i pos, TILE tile)
{
	DungeonGenerator::Set_Tile(pos, tile);
	
	m_Regions[pos.x()][pos.y()] = m_CurrentRegion;
}


void DungeonGenerator::InitMap(Display &MainDisplay)
{
	//create a 2d vector and fill it with temp tiles
	std::vector<std::vector<TerrainTile>> Layer;
	Layer.resize(m_DungeonWidth, std::vector<TerrainTile>(m_DungeonHeight, TerrainTile(NULL, 0, 0, Floor, cm_TileSize, cm_TileSize)));

	TILE CurTileType = Blank;

	//for each tile in the generated dungeon we need to have a graphical tile created
	//that is correct for it's tile type
	for (unsigned int x = 0; x < m_Dungeon.size(); x++)
	{
		for (unsigned int y = 0; y < m_Dungeon[0].size(); y++)
		{
			AVec2i Pos(x, y);
			CurTileType = Get_Tile(Pos);
			
			if (CurTileType == Wall)
			{
				Layer[x][y] = MakeWall(Pos); 
			}
			else if (CurTileType == Floor)
			{
				
				Layer[x][y] = MakeFloor(Pos);
			}
			else if (CurTileType == Door)
			{
				Layer[x][y] = MakeDoor(Pos);
			}
		}
	}

	//the previous helper functions will not generate the tiles around the borders of the map
	//so we need to do that here
	//this for loop fills in the border tiles on the top and bottom of the dungeon
	for (unsigned int x = 0; x < m_Dungeon.size(); x++)
	{
		CurTileType = Get_Tile(AVec2i(x, 1));

		if (CurTileType == Floor || CurTileType == Door)
		{
			Layer[x][0] = TerrainTile(m_DungeonTiles, x * cm_TileSize, 0 * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 0, 0); //a standard wall is placed 
		}

		CurTileType = Get_Tile(AVec2i(x, m_DungeonHeight - 2));

		if (CurTileType == Floor || CurTileType == Door)
		{
			Layer[x][m_DungeonHeight - 1] = TerrainTile(m_DungeonTiles, x * cm_TileSize, (m_DungeonHeight - 1) * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 3, 0); //top wall border is placed
		}
	}

	//this for loop fills in the border tiles on the right and left sides of the dungeon
	for (int y = 0; y < m_DungeonHeight; y++)
	{
		CurTileType = Get_Tile(AVec2i(1, y));

		if (CurTileType == Floor || CurTileType == Door)
		{
			Layer[0][y] = TerrainTile(m_DungeonTiles, 0 * cm_TileSize, y * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 4, 0); //right wall border is placed
		}

		CurTileType = Get_Tile(AVec2i(m_DungeonWidth - 2, y));

		if (CurTileType == Floor || CurTileType == Door)
		{
			Layer[m_DungeonWidth - 1][y] = TerrainTile(m_DungeonTiles, (m_DungeonWidth - 1) * cm_TileSize, y * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 5, 0); //left wall border is placed
		}
	}

	//A new terrain layer is created with those tiles
	TerrainLayer *Temp = new TerrainLayer(Layer);
	//A bitmap image of those tiles is generated so it can be drawn
	Temp->CreateBitmap(&MainDisplay); 
	//creates the m_Map. With the newly generated base layer 
	m_Map = new TerrainMap(m_EventQueue, Temp, m_MainPlayer);
}

void DungeonGenerator::DecorateMap(Display &MainDisplay)
{
	//create a 2d vector and fill it with temp tiles
	std::vector<std::vector<TerrainTile>> Layer;
	Layer.resize(m_DungeonWidth, std::vector<TerrainTile>(m_DungeonHeight, TerrainTile(NULL, 0, 0, Blank, cm_TileSize, cm_TileSize)));

	//since this layer will have objects in it, we only want those to spawn inside of rooms
	//so we need to get all of those. And for sake of easyness I put all of those rooms into a 
	//vector
	std::vector<Rect> Rooms;

	for (std::list<Rect>::iterator it = m_Rooms.begin(); it != m_Rooms.end(); it++)
	{
		Rooms.push_back(*it);
	}

	int NumObjectsToPlace = Random(25, 60);
	
	for (int i = 0; i < NumObjectsToPlace; i++)
	{
		int RoomNum = Random(0, m_Rooms.size() - 1); //pick a room
			
		AVec2i Pos(Random(Rooms[RoomNum].Get_X1(), Rooms[RoomNum].Get_X2()), Random(Rooms[RoomNum].Get_Y1(), Rooms[RoomNum].Get_Y2())); //pick a random spot in that room

		Layer[Pos.x()][Pos.y()] = TerrainTile(m_DungeonTiles, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 0, 2, true, TR_LOOT, m_EventQueue, Random(10,35)); //a standard destructible is placed
	}

	Layer[m_BossPortal.x()][m_BossPortal.y()] = TerrainTile(m_DungeonTiles, m_BossPortal.x() * cm_TileSize, m_BossPortal.y() *cm_TileSize, 
		Floor, cm_TileSize, cm_TileSize, false, 2, 2, true, TR_BOSS, m_EventQueue); //Boss Portal



	//A new terrain layer is created with those tiles
	TerrainLayer *Temp = new TerrainLayer(Layer);
	//A bitmap image of those tiles is generated so it can be drawn
	Temp->CreateBitmap(&MainDisplay);
	//Since the m_Map is created. We simply add this new layer to it
	m_Map->AddLayer(Temp, 1);
}

void DungeonGenerator::CreateFog(Display &MainDisplay)
{
	//create a 2d vector and fill it with temp tiles
	std::vector<std::vector<TerrainTile>> Layer;
	Layer.resize(m_DungeonWidth, std::vector<TerrainTile>(m_DungeonHeight, TerrainTile(NULL, 0, 0, Blank, cm_TileSize, cm_TileSize)));

	//as in the decorate layer, we only want fog to spawn inside of rooms
	//so we need to get all of those. And for sake of easyness I put all of those rooms into a 
	//vector
	std::vector<Rect> Rooms;

	for (std::list<Rect>::iterator it = m_Rooms.begin(); it != m_Rooms.end(); it++)
	{
		//we don't want fog in the room where the player spawns though
		if (it->ContainsPoint(m_StartPosition.x(), m_StartPosition.y()))
		{
			continue;
		}

		Rooms.push_back(*it);
	}

	for (unsigned int i = 0; i < Rooms.size(); i++)
	{
		for (int x = Rooms[i].Get_X1(); x < Rooms[i].Get_X2(); x++)
		{
			for (int y = Rooms[i].Get_Y1(); y < Rooms[i].Get_Y2(); y++)
			{
				Layer[x][y] = TerrainTile(NULL, x * cm_TileSize, y * cm_TileSize, Fog, cm_TileSize, cm_TileSize, false, 0, 0, 1, TR_FOG, m_EventQueue);
			}
		}
	}
	

	//A new terrain layer is created with those tiles
	TerrainLayer *Temp = new TerrainLayer(Layer, 0);
	//A bitmap image of those tiles is generated so it can be drawn
	Temp->CreateBitmap(&MainDisplay);
	//Since the m_Map is created. We simply add this new layer to it
	m_Map->AddLayer(Temp, 1);
}

TerrainTile DungeonGenerator::MakeWall(AVec2i Pos)
{
	bool Left = false;
	bool Right = false;
	bool Top = false;

	if (Pos.x() == 0 || Pos.y() == 0 || Pos.x() == m_DungeonWidth - 1 || Pos.y() == m_DungeonHeight - 1) //won't create a border tile
	{
		return TerrainTile(NULL, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true);
	}
	
	if (Get_Tile(Pos + m_Cardinal[0]) == Floor || Get_Tile(Pos + m_Cardinal[0]) == Door) //If the tile directly below is a Floor or Door 
	{
		return TerrainTile(m_DungeonTiles, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 0, 0); //a standard wall is placed
	}
	
	if (Get_Tile(Pos + m_Cardinal[1]) == Floor || Get_Tile(Pos + m_Cardinal[1]) == Door) //If the tile directly left is a Floor or Door
	{
		Left = 1;
	}
	if (Get_Tile(Pos + m_Cardinal[3]) == Floor || Get_Tile(Pos + m_Cardinal[3]) == Door) //If the tile directly right is a Floor or Door
	{
		Right = 1;
	}
	if (Get_Tile(Pos + m_Cardinal[2]) == Floor || Get_Tile(Pos + m_Cardinal[2]) == Door) //If the tile directly above is a Floor or Door
	{
		Top = 1;
	}

	if (!Left && !Right && !Top) //Blank Tile
	{
		return TerrainTile(NULL, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true);
	}

	if (Left && Right && Top)
	{
		return TerrainTile(m_DungeonTiles, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 3, 1); //right/left/top wall border is placed
	}

	if (Left && Right)
	{
		return TerrainTile(m_DungeonTiles, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 6, 0); //right/left wall border is placed
	}

	if (Top && Left)
	{
		return TerrainTile(m_DungeonTiles, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 4, 1); //Top/left wall border is placed
	}

	if (Top && Right)
	{
		return TerrainTile(m_DungeonTiles, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 2, 1); //Top/right wall border is placed
	}

	if (Left)
	{
		return TerrainTile(m_DungeonTiles, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 4, 0); //left wall border is placed
	}
	
	if (Right)
	{
		return TerrainTile(m_DungeonTiles, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 5, 0); //right wall border is placed
	}

	if (Top)
	{
		return TerrainTile(m_DungeonTiles, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 3, 0); //top wall border is placed
	}
	
	return TerrainTile(NULL, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Blank, cm_TileSize, cm_TileSize); //top wall border is placed 
}
TerrainTile DungeonGenerator::MakeDoor(AVec2i Pos)
{
	return TerrainTile(m_DungeonTiles, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Door, cm_TileSize, cm_TileSize, false, 7);
}
TerrainTile DungeonGenerator::MakeFloor(AVec2i Pos)
{
	return TerrainTile(m_DungeonTiles, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Floor, cm_TileSize, cm_TileSize, false, 7);
}

void DungeonGenerator::Draw(bool PrePlayerDraw)
{
	m_Map->Draw(PrePlayerDraw);
}

void DungeonGenerator::Event_Handler(ALLEGRO_EVENT &EV)
{
	m_Map->Event_Handler(EV);

	if (EV.type == TERRAINTILE_TRIGGER_EVENT)
	{
		//check if the player walked onto the boss spawn tile
		if ((TRIGGER)EV.user.data1 == TR_BOSS)
		{
			int X = EV.user.data2;
			int Y = EV.user.data3;

			WallOffBossRoom(); //wall off the boss room

			EV.user.type = CUSTOM_EVENT_ID(SPAWN_BOSS_EVENT); //tell the game to the boss is about to spawn
			EV.user.data1 = (intptr_t)X;
			EV.user.data2 = (intptr_t)Y;
			al_emit_user_event(&m_SpawnBossEvent, &EV, NULL);
		}
	}
	else if (EV.type == DUNGEON_COMPLETE_EVENT)
	{
		//dungeon completed, dungeon level up
		m_Level++;
	}
}

void DungeonGenerator::SetStartPosition()
{
	std::vector<Rect> TempRooms;
	
	Rect TempRoom(m_DungeonWidth, m_DungeonHeight, 1, 1);

	//Find the room furthest to the left of the screen
	for (std::list<Rect>::iterator it = m_Rooms.begin(); it != m_Rooms.end(); it++)
	{
		if (it->Get_X1() < TempRoom.Get_X1())
		{
			TempRoom = *it;
		}
	}

	//Find any other rooms that may be at that Xposition
	for (std::list<Rect>::iterator it = m_Rooms.begin(); it != m_Rooms.end(); it++)
	{
		if (it->Get_X1() == TempRoom.Get_X1())
		{
			TempRooms.push_back(*it);
		}
	}
	
	//Of all those rooms find the one that is the widest
	for (std::vector<Rect>::iterator it = TempRooms.begin(); it != TempRooms.end(); it++)
	{
		if (it->Get_X2() > TempRoom.Get_X2())
		{
			TempRoom = *it;
		}
	}

	//Find all rooms that are within the Xposition of the widest room
	for (std::list<Rect>::iterator it = m_Rooms.begin(); it != m_Rooms.end(); it++)
	{
		if (it->Get_X1() < TempRoom.Get_X2())
		{
			TempRooms.push_back(*it);
		}
	}
	
	//Now we should have, in general, all the rooms that are furthest to the left
	//later I think it would be better to spawn in a room with only one door, might 
		//add an additional step to single out rooms with the least amount doors

	//For now though we'll just choose a random room from those that we currently have
	//Then choose a random position in that room to spawn at.
	TempRoom = TempRooms[Random(0, TempRooms.size())];
	
	m_StartPosition = AVec2f(Random(TempRoom.Get_X1() + 1, TempRoom.Get_X2() - 1), Random(TempRoom.Get_Y1() + 1, TempRoom.Get_Y2() - 1));
}

void DungeonGenerator::SetBossPortalSpawn()
{
	std::vector<Rect> TempRooms;

	Rect TempRoom = m_Rooms.front();

	//Find the room furthest to the right of the screen
	for (std::list<Rect>::iterator it = m_Rooms.begin(); it != m_Rooms.end(); it++)
	{
		if (it->Get_X2() > TempRoom.Get_X2())
		{
			TempRoom = *it;
		}
	}

	//Find any other rooms that may be at that Xposition
	for (std::list<Rect>::iterator it = m_Rooms.begin(); it != m_Rooms.end(); it++)
	{
		if (it->Get_X2() == TempRoom.Get_X2())
		{
			TempRooms.push_back(*it);
		}
	}

	//Find all rooms that are within the Xposition of the widest room
	for (std::list<Rect>::iterator it = m_Rooms.begin(); it != m_Rooms.end(); it++)
	{
		if (it->Get_X2() > TempRoom.Get_X1())
		{
			TempRooms.push_back(*it);
		}
	}

	//Now we should have, in general, all the rooms that are furthest to the left
	

	//For now we'll just choose a random room from those that we currently have
	//Then choose a random position in that room to spawn at.
	TempRoom = TempRooms[Random(0, TempRooms.size())];

	m_BossPortal = AVec2f(Random(TempRoom.Get_X1() + 1, TempRoom.Get_X2() - 1), Random(TempRoom.Get_Y1() + 1, TempRoom.Get_Y2() - 1));
}

void DungeonGenerator::WallOffBossRoom()
{
	Rect BossRoom(0,0,0,0);

	for (std::list<Rect>::iterator it = m_Rooms.begin(); it != m_Rooms.end(); it++)
	{
		if (it->ContainsPoint(m_BossPortal.x(), m_BossPortal.y()))
		{
			BossRoom = *it;
			break;
		}
	}

	for (int x = BossRoom.Get_X1(); x <= BossRoom.Get_X2(); x++)
	{	
		if (BossRoom.Get_Y1() - 1 > 0)
		{
			if (m_Map->Get_Layer(0)->Get_Tile(AVec2i(x, BossRoom.Get_Y1() - 1)).Get_TileType() == Door)
			{
				m_Map->Get_Layer(0)->Set_Tile(AVec2i(x, BossRoom.Get_Y1() - 1), MakeWall(AVec2i(x, BossRoom.Get_Y1() - 1)));
			}
		}

		if (BossRoom.Get_Y2() < m_DungeonHeight)
		{
			if (m_Map->Get_Layer(0)->Get_Tile(AVec2i(x, BossRoom.Get_Y2())).Get_TileType() == Door)
			{
				m_Map->Get_Layer(0)->Set_Tile(AVec2i(x, BossRoom.Get_Y2()), MakeWall(AVec2i(x, BossRoom.Get_Y2() + 1)));
			}
		}
	}

	for (int y = BossRoom.Get_Y1(); y <= BossRoom.Get_Y2(); y++)
	{
		if (BossRoom.Get_X1() - 1 > 0)
		{
			if (m_Map->Get_Layer(0)->Get_Tile(AVec2i(BossRoom.Get_X1() - 1, y)).Get_TileType() == Door)
			{
				m_Map->Get_Layer(0)->Set_Tile(AVec2i(BossRoom.Get_X1() - 1, y), MakeWall(AVec2i(BossRoom.Get_X1() - 1, y)));
			}
		}

		if (BossRoom.Get_X2() < m_DungeonWidth)
		{
			if (m_Map->Get_Layer(0)->Get_Tile(AVec2i(BossRoom.Get_X2(), y)).Get_TileType() == Door)
			{
				m_Map->Get_Layer(0)->Set_Tile(AVec2i(BossRoom.Get_X2(), y), MakeWall(AVec2i(BossRoom.Get_X2() + 1, y)));
			}
		}
	}

	m_Map->UpdateInfoLayer(0);
	m_Map->Get_Layer(0)->CreateBitmap(NULL);
}
