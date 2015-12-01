//Created by:		Ryan Nolan-Hieb
//Date updated:		9/25/15
//Last update by:	N/A
//Reason for update:N/A

#include "DungeonGenerator.h"
#include "Utility.h"

#include <algorithm>
#include <iostream>


bool Streamline = true; //Not used currently


void DungeonGenerator::GenerateDungeonVars()
{
	m_NumRoomTries = Random(100, 200); //The number of times a room will be created and tried to be randomly placed inside the dungeon.
	m_ExtraConnectorChance = Random(3,6); //An extra chance for doors to be created
	m_RoomExtraSize = Random(3,6); //An additional size to be applied to rooms
	m_WindingPercent = Random(45,70); //A percentage for how windy the maze sections should be
}

void DungeonGenerator::GenerateDungeon(Display &MainDisplay)
{
	m_CurrentRegion = -1;

	GenerateDungeonVars();

	m_Dungeon.clear();
	m_Regions.clear();
	m_Rooms.clear();

	if (m_Map)
	{
		delete m_Map;
		m_Map = NULL;
	}

	m_Dungeon.resize(m_DungeonWidth, std::vector<TILE>(m_DungeonHeight, Wall));
	m_Regions.resize(m_DungeonWidth, std::vector<int>(m_DungeonHeight, m_CurrentRegion));
	
	AddRooms();

	for (int x = 1; x < m_DungeonWidth; x += 2)
	{
		for (int y = 1; y < m_DungeonHeight; y += 2)
		{
			if (Get_Tile(Vec2i(x, y)) != Wall)
			{
				continue;
			}
			
			GrowMaze(Vec2i(x,y));
		}
	}

	connectRegions();

	RemoveDeadEnds();

	//PrintCurrentMap();

	SetStartPosition();
	SetBossPortalSpawn();

	InitMap(MainDisplay);
	DecorateMap(MainDisplay);
	CreateFog(MainDisplay);
}

void DungeonGenerator::PrintCurrentMap()
{
	for (int y = 0; y < m_DungeonHeight; y++)
	{
		for (int x = 0; x < m_Dungeon.size(); x++)
		{
			if (m_Dungeon[x][y] == Wall)
			{
				std::cout << '0';
			}
			else if (m_Dungeon[x][y] == Door)
			{
				std::cout << '2';
			}
			else
			{
				std::cout << ' ';
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
			Vec2i Pos(ix, iy);
			if (Get_Tile(Pos) != Wall)
				continue;
			
			std::unordered_set<int> Regions;
			
			for (int i = 0; i < m_Cardinal.size(); i++)
			{
				Vec2i Indexer = (Pos + m_Cardinal[i]);
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
		Vec2i connector(Temp[0], Temp[1]);

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
			for (int x = 0; x < Sources.size(); x++)
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
	Vec2i tmp(Pos[0], Pos[1]);

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
		for (int i = 0; i < m_Cardinal.size(); i++)
		{
			Vec2i indexer(tmp + m_Cardinal[i]);
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
				Vec2i Pos(ix, iy);

				if (Get_Tile(Pos) == Wall)
				{
					continue;
				}

				int exits = 0;

				for (int i = 0; i < m_Cardinal.size(); i++)
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

void DungeonGenerator::AddJunction(Vec2i Pos)
{
	Set_Tile(Pos, Door);
}

void DungeonGenerator::GrowMaze(Vec2i Start)
{
	std::vector<Vec2i> Cells;
	Vec2i LastDir;

	m_CurrentRegion++;

	Carve(Start, Floor);
	
	Cells.push_back(Start);

	while (!Cells.empty())
	{
		Vec2i cell = Cells.back();

		std::vector<Vec2i> UnmadeCells;

		for (std::vector<Vec2i>::iterator it = m_Cardinal.begin(); it != m_Cardinal.end(); ++it)
		{
			if (CanCarve(cell, *it))
			{
				UnmadeCells.push_back(*it);
			}
		}

		if (!UnmadeCells.empty())
		{
			Vec2i dir;
			bool contains = false;
			for (std::vector<Vec2i>::iterator it = UnmadeCells.begin(); it != UnmadeCells.end(); ++it)
			{
				if (*it == LastDir)
				{
					contains = true;
					break;
				}
			}

			if (contains && Random(0, 101) > m_WindingPercent)
			{
				dir = LastDir;
			}
			else
			{
				dir = UnmadeCells[Random(0, UnmadeCells.size())];
			}

			Carve(cell + dir, Floor);
			Carve(cell + dir * 2, Floor);

			Cells.push_back(cell + dir * 2);
			LastDir = dir;
		}
		else
		{
			Cells.pop_back();
			LastDir = NULL;
		}

	}
}

void DungeonGenerator::AddRooms()
{
	std::list<Rect> TempRooms;

	for (int i = 0; i < m_NumRoomTries; i++)
	{
		int Size = Random(1, 3 + m_RoomExtraSize) * 2 + 1;
		int Rectangularity = Random(0, 1 + Size / 2) * 2;
		int width = Size;
		int height = Size;

		if (rand() % 2 == 0)
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

		if (overlaps) continue;

		if (x + width > m_DungeonWidth || y + height > m_DungeonHeight)
		{
			std::cout << "room generation error" << std::endl;
		}

		TempRooms.push_back(TempRoom);

		m_CurrentRegion++;

		for (int ix = x; ix < x + width; ix++)
		{
			for (int iy = y; iy < y + height; iy++)
			{
				Carve(Vec2i(ix, iy), Floor);
			}
		}
	}

	m_Rooms = TempRooms;
}

bool DungeonGenerator::CanCarve(Vec2i Pos, Vec2i Direction)
{
	Vec2i iv2 = Pos + Direction * 3;

	Rect Bounds(0, 0, m_DungeonWidth, m_DungeonHeight);

	if (!Bounds.ContainsPoint(iv2.x(), iv2.y())) return 0;

	return (Get_Tile(Pos + Direction * 2) == Wall);
}

void DungeonGenerator::Carve(Vec2i pos, TILE tile)
{
	DungeonGenerator::Set_Tile(pos, tile);
	
	m_Regions[pos.x()][pos.y()] = m_CurrentRegion;
}


void DungeonGenerator::InitMap(Display &MainDisplay)
{
	
	std::vector<std::vector<TerrainTile>> Layer;
	Layer.resize(m_DungeonWidth, std::vector<TerrainTile>(m_DungeonHeight, TerrainTile(NULL, 0, 0, Floor, cm_TileSize, cm_TileSize)));

	TILE CurTileType = Blank;

	for (int x = 0; x < m_Dungeon.size(); x++)
	{
		for (int y = 0; y < m_Dungeon[0].size(); y++)
		{
			Vec2i Pos(x, y);
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

	for (int x = 0; x < m_Dungeon.size(); x++)
	{
		CurTileType = Get_Tile(Vec2i(x, 1));

		if (CurTileType == Floor || CurTileType == Door)
		{
			Layer[x][0] = TerrainTile(m_DungeonTiles, x * cm_TileSize, 0 * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 0, 0); //a standard wall is placed 
		}

		CurTileType = Get_Tile(Vec2i(x, m_DungeonHeight - 2));

		if (CurTileType == Floor || CurTileType == Door)
		{
			Layer[x][m_DungeonHeight - 1] = TerrainTile(m_DungeonTiles, x * cm_TileSize, (m_DungeonHeight - 1) * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 3, 0); //top wall border is placed
		}
	}

	for (int y = 0; y < m_DungeonHeight; y++)
	{
		CurTileType = Get_Tile(Vec2i(1, y));

		if (CurTileType == Floor || CurTileType == Door)
		{
			Layer[0][y] = TerrainTile(m_DungeonTiles, 0 * cm_TileSize, y * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 4, 0); //right wall border is placed
		}

		CurTileType = Get_Tile(Vec2i(m_DungeonWidth - 2, y));

		if (CurTileType == Floor || CurTileType == Door)
		{
			Layer[m_DungeonWidth - 1][y] = TerrainTile(m_DungeonTiles, (m_DungeonWidth - 1) * cm_TileSize, y * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 5, 0); //left wall border is placed
		}
	}

	TerrainLayer *Temp = new TerrainLayer(Layer);
	Temp->CreateBitmap(&MainDisplay);
	m_Map = new TerrainMap(m_EventQueue, Temp, m_MainPlayer);
}

void DungeonGenerator::DecorateMap(Display &MainDisplay)
{
	std::vector<std::vector<TerrainTile>> Layer;
	Layer.resize(m_DungeonWidth, std::vector<TerrainTile>(m_DungeonHeight, TerrainTile(NULL, 0, 0, Blank, cm_TileSize, cm_TileSize)));

	TILE CurTileType = Blank;

	std::vector<Rect> Rooms;

	for (std::list<Rect>::iterator it = m_Rooms.begin(); it != m_Rooms.end(); it++)
	{
		Rooms.push_back(*it);
	}

	int NumObjectsToPlace = Random(25, 60);
	
	for (int i = 0; i < NumObjectsToPlace; i++)
	{
		int RoomNum = Random(0, m_Rooms.size() - 1);
			
		Vec2i Pos(Random(Rooms[RoomNum].Get_X1(), Rooms[RoomNum].Get_X2()), Random(Rooms[RoomNum].Get_Y1(), Rooms[RoomNum].Get_Y2()));

		Layer[Pos.x()][Pos.y()] = TerrainTile(m_DungeonTiles, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Wall, cm_TileSize, cm_TileSize, true, 0, 2, true, TR_LOOT, m_EventQueue, Random(10,35)); //a standard wall is placed
	}

	Layer[m_BossPortal.x()][m_BossPortal.y()] = TerrainTile(m_DungeonTiles, m_BossPortal.x() * cm_TileSize, m_BossPortal.y() *cm_TileSize, 
		Floor, cm_TileSize, cm_TileSize, false, 2, 2, true, TR_BOSS, m_EventQueue); //Boss Portal


	TerrainLayer *Temp = new TerrainLayer(Layer);
	Temp->CreateBitmap(&MainDisplay);
	m_Map->AddLayer(Temp, 1);
}

void DungeonGenerator::CreateFog(Display &MainDisplay)
{
	std::vector<std::vector<TerrainTile>> Layer;
	Layer.resize(m_DungeonWidth, std::vector<TerrainTile>(m_DungeonHeight, TerrainTile(NULL, 0, 0, Blank, cm_TileSize, cm_TileSize)));

	std::vector<Rect> Rooms;

	for (std::list<Rect>::iterator it = m_Rooms.begin(); it != m_Rooms.end(); it++)
	{
		if (it->ContainsPoint(m_StartPosition.x(), m_StartPosition.y()))
		{
			continue;
		}

		Rooms.push_back(*it);
	}

	for (int i = 0; i < Rooms.size(); i++)
	{
		for (int x = Rooms[i].Get_X1(); x < Rooms[i].Get_X2(); x++)
		{
			for (int y = Rooms[i].Get_Y1(); y < Rooms[i].Get_Y2(); y++)
			{
				Layer[x][y] = TerrainTile(NULL, x * cm_TileSize, y * cm_TileSize, Fog, cm_TileSize, cm_TileSize, false, 0, 0, 1, TR_FOG, m_EventQueue);
			}
		}
	}


	TerrainLayer *Temp = new TerrainLayer(Layer, 0);
	Temp->CreateBitmap(&MainDisplay);
	m_Map->AddLayer(Temp, 1);
}

TerrainTile DungeonGenerator::MakeWall(Vec2i Pos)
{
	bool Left = false;
	bool Right = false;
	bool Top = false;

	if (Pos.x() == 0 || Pos.y() == 0 || Pos.x() == m_DungeonWidth - 1 || Pos.y() == m_DungeonHeight - 1)
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
	
}
TerrainTile DungeonGenerator::MakeDoor(Vec2i Pos)
{
	return TerrainTile(m_DungeonTiles, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Door, cm_TileSize, cm_TileSize, false, 7);
}
TerrainTile DungeonGenerator::MakeFloor(Vec2i Pos)
{
	return TerrainTile(m_DungeonTiles, Pos.x() * cm_TileSize, Pos.y() * cm_TileSize, Floor, cm_TileSize, cm_TileSize, false, 7);
}

void DungeonGenerator::Draw(bool PrePlayerDraw)
{
	m_Map->Draw(PrePlayerDraw);
	
	//al_draw_filled_circle(m_BossPortal.x() * cm_TileSize, m_BossPortal.y() * cm_TileSize, 5, al_map_rgb(255, 255, 255));
	//al_draw_filled_circle(m_BossPortal.x() * cm_TileSize, m_BossPortal.y() * cm_TileSize, 5, al_map_rgb(255, 0, 0));
}

void DungeonGenerator::Event_Handler(ALLEGRO_EVENT &EV)
{
	m_Map->Event_Handler(EV);

	if (EV.type == TERRAINTILE_TRIGGER_EVENT)
	{
		if ((TRIGGER)EV.user.data1 == TR_BOSS)
		{
			int X = EV.user.data2;
			int Y = EV.user.data3;

			WallOffBossRoom();

			EV.user.type = CUSTOM_EVENT_ID(SPAWN_BOSS_EVENT);
			EV.user.data1 = (intptr_t)X;
			EV.user.data2 = (intptr_t)Y;
			al_emit_user_event(&m_SpawnBossEvent, &EV, NULL);
		}
	}
	else if (EV.type == DUNGEON_COMPLETE_EVENT)
	{
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
	
	m_StartPosition = Vec2f(Random(TempRoom.Get_X1() + 1, TempRoom.Get_X2() - 1), Random(TempRoom.Get_Y1() + 1, TempRoom.Get_Y2() - 1));
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

	m_BossPortal = Vec2f(Random(TempRoom.Get_X1() + 1, TempRoom.Get_X2() - 1), Random(TempRoom.Get_Y1() + 1, TempRoom.Get_Y2() - 1));
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
			if (m_Map->Get_Layer(0)->Get_Tile(Vec2i(x, BossRoom.Get_Y1() - 1)).Get_TileType() == Door)
			{
				m_Map->Get_Layer(0)->Set_Tile(Vec2i(x, BossRoom.Get_Y1() - 1), MakeWall(Vec2i(x, BossRoom.Get_Y1() - 1)));
			}
		}

		if (BossRoom.Get_Y2() < m_DungeonHeight)
		{
			if (m_Map->Get_Layer(0)->Get_Tile(Vec2i(x, BossRoom.Get_Y2())).Get_TileType() == Door)
			{
				m_Map->Get_Layer(0)->Set_Tile(Vec2i(x, BossRoom.Get_Y2()), MakeWall(Vec2i(x, BossRoom.Get_Y2() + 1)));
			}
		}
	}

	for (int y = BossRoom.Get_Y1(); y <= BossRoom.Get_Y2(); y++)
	{
		if (BossRoom.Get_X1() - 1 > 0)
		{
			if (m_Map->Get_Layer(0)->Get_Tile(Vec2i(BossRoom.Get_X1() - 1, y)).Get_TileType() == Door)
			{
				m_Map->Get_Layer(0)->Set_Tile(Vec2i(BossRoom.Get_X1() - 1, y), MakeWall(Vec2i(BossRoom.Get_X1() - 1, y)));
			}
		}

		if (BossRoom.Get_X2() < m_DungeonWidth)
		{
			if (m_Map->Get_Layer(0)->Get_Tile(Vec2i(BossRoom.Get_X2(), y)).Get_TileType() == Door)
			{
				m_Map->Get_Layer(0)->Set_Tile(Vec2i(BossRoom.Get_X2(), y), MakeWall(Vec2i(BossRoom.Get_X2() + 1, y)));
			}
		}
	}

	m_Map->UpdateInfoLayer(0);
	m_Map->Get_Layer(0)->CreateBitmap(NULL);
}
