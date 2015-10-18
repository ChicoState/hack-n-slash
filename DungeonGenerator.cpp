//Created by:		Ryan Nolan-Hieb
//Date updated:		9/25/15
//Last update by:	N/A
//Reason for update:N/A

#include "DungeonGenerator.h"

#include <algorithm>
#include <iostream>

int NumRoomTries = 200; //The number of times a room will be created and tried to be randomly placed inside the dungeon.
int ExtraConnectorChance = 5; //An extra chance for doors to be created
int RoomExtraSize = 2; //An additional size to be applied to rooms
int WindingPercent = 60; //A percentage for how windy the maze sections should be
bool Streamline = true; //Not used currently

int currentRegion = -1; //Don't change this. It's used internally. Should be a member variable but I'm just too lazy to move it there at the moment :P

//This is the size of the graphical tiles that being drawn. This is set low currently in order to see the entire map on the screen.
//Normally this value should be set 64. 
int TileSize = 128; 

//used for generating a random number between two values. Should probably be moved to a different file but again, I'm lazy :P.
int Random(int RandomAmount1, int RandomAmount2)
{

	int Number = rand() % (RandomAmount2 - RandomAmount1) + RandomAmount1;
	//int Number = rand() % RandomAmount2 + RandomAmount1;

	return Number;
}

void DungeonGenerator::GenerateDungeon(Display &MainDisplay)
{
	currentRegion = -1;

	m_Dungeon.resize(cm_DungeonWidth, std::vector<TILE>(cm_DungeonHeight, Wall));
	m_Regions.resize(cm_DungeonWidth, std::vector<int>(cm_DungeonHeight, currentRegion));
	
	AddRooms();

	for (int x = 1; x < cm_DungeonWidth; x += 2)
	{
		for (int y = 1; y < cm_DungeonHeight; y += 2)
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

	InitMap(MainDisplay);
	
	//MultiplyDungeon(2);
}

void DungeonGenerator::PrintCurrentMap()
{
	for (int y = 0; y < cm_DungeonHeight; y++)
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
	
	for (int ix = 1; ix < cm_DungeonWidth - 1; ix++)
	{
		for (int iy = 1; iy < cm_DungeonHeight - 1; iy++)
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

	for (int i = 0; i <= currentRegion; i++)
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

		for (int i = 0; i <= currentRegion; i++)
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

	if (Random(0, 101) < ExtraConnectorChance)
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

		for (int ix = 1; ix < cm_DungeonWidth - 1; ix++)
		{
			for (int iy = 1; iy < cm_DungeonHeight - 1; iy++)
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

void DungeonGenerator::StreamLineCorridors()
{
	std::vector<Vec2i> Corridors;
	std::vector<std::vector<Vec2i>> Traces;

	for (int ix = 1; ix < cm_DungeonWidth - 1; ix++)
	{
		for (int iy = 1; iy < cm_DungeonHeight - 1; iy++)
		{
			if (m_Dungeon[ix][iy] == Floor)
			{
				Corridors.push_back(Vec2i(ix, iy));
			}
		}
	}

	int Failsave = 1000;

	while (Corridors.size() > 0 && Failsave > 0)
	{
		if (Failsave == 1)
		{
			std::cout << "This shouldn't happen";
		}

		Failsave--;

		std::vector<Vec2i> segment;
		Vec2i Current = Corridors[0];
		BuildLineSegment(Current, Corridors, segment, 0, true);
	}

}

int DungeonGenerator::BuildLineSegment(Vec2i Current, std::vector<Vec2i>& Source, std::vector<Vec2i>& Target, int CurrentDepth, bool AddAtEnd)
{
	if (CurrentDepth > 1000)
	{
		return CurrentDepth + 1;
	}

	int exits = 0;

	for (int i = 0; i < m_Cardinal.size(); i++)
	{
		if (Get_Tile(Current + m_Cardinal[i]) != Wall)
		{
			exits++;
		}
	}
	if (exits > 2)
	{
		//Source.
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

	currentRegion++;

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

			if (contains && Random(0, 101) > WindingPercent)
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

	for (int i = 0; i < NumRoomTries; i++)
	{
		int Size = Random(1, 3 + RoomExtraSize) * 2 + 1;
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

		int x = Random(0, (cm_DungeonWidth - width) / 2) * 2 + 1;
		int y = Random(0, (cm_DungeonHeight - height) / 2) * 2 + 1;

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

		if (x + width > cm_DungeonWidth || y + height > cm_DungeonHeight)
		{
			std::cout << "room generation error" << std::endl;
		}

		TempRooms.push_back(TempRoom);

		currentRegion++;

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

	Rect Bounds(0, 0, cm_DungeonWidth, cm_DungeonHeight);

	if (!Bounds.ContainsPoint(iv2.x(), iv2.y())) return 0;

	return (Get_Tile(Pos + Direction * 2) == Wall);
}

void DungeonGenerator::Carve(Vec2i pos, TILE tile)
{
	DungeonGenerator::Set_Tile(pos, tile);
	
	m_Regions[pos.x()][pos.y()] = currentRegion;
}


void DungeonGenerator::MultiplyDungeon(int Factor)
{
	std::vector<std::vector<TILE>> NewDungeon;
	std::vector<std::vector<int>> NewRegions;

	NewDungeon.resize(cm_DungeonWidth * Factor, std::vector<TILE>(cm_DungeonHeight * Factor, Wall));
	NewRegions.resize(cm_DungeonWidth * Factor, std::vector<int>(cm_DungeonHeight * Factor, currentRegion));

	for (int x = 0; x < cm_DungeonWidth; x++)
	{
		for (int y = 0; y < cm_DungeonHeight; y++)
		{
			int nx = 0;
			int ny = 0;

			for (int i = 0; i < Factor*Factor; i++)
			{
				NewDungeon[x*Factor + nx][y*Factor + ny] = m_Dungeon[x][y];
				NewRegions[x*Factor + nx][y*Factor + ny] = m_Regions[x][y];

				if (nx == Factor - 1)
				{
					ny++; 
				}
				nx = (nx + 1);
			}
		}
	}

	m_Dungeon = NewDungeon;
	m_Regions = NewRegions;
}

void DungeonGenerator::InitMap(Display &MainDisplay)
{
	
	std::vector<std::vector<TerrainTile>> Layer;
	Layer.resize(cm_DungeonWidth, std::vector<TerrainTile>(cm_DungeonHeight, TerrainTile(NULL,0,0,Floor,0,0)));

	for (int x = 0; x < m_Dungeon.size(); x++)
	{
		for (int y = 0; y < m_Dungeon[0].size(); y++)
		{
			TILE CurTileType = Get_Tile(Vec2i(x, y));
			
			if (CurTileType == Wall)
			{
				Layer[x][y] = TerrainTile(NULL, x * TileSize, y * TileSize, Wall, TileSize, TileSize, true);
			}
			else if (CurTileType == Floor)
			{
				Layer[x][y] = TerrainTile(NULL, x * TileSize, y * TileSize, Floor, TileSize, TileSize, false);
			}
			else if (CurTileType == Door)
			{
				Layer[x][y] = TerrainTile(NULL, x * TileSize, y * TileSize, Door, TileSize, TileSize, false);
			}
		}
	}
	TerrainLayer *Temp = new TerrainLayer(Layer);
	Temp->CreateBitmap(MainDisplay);
	m_Map = new TerrainMap(Temp);
	
	
}

void DungeonGenerator::Draw()
{
	m_Map->Draw();
	
	al_draw_filled_circle(m_StartPosition.x() * TileSize, m_StartPosition.y() * TileSize, 5, al_map_rgb(255, 255, 255));
}

void DungeonGenerator::SetStartPosition()
{
	std::vector<Rect> TempRooms;
	
	Rect TempRoom(cm_DungeonWidth, cm_DungeonHeight, 1, 1);

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