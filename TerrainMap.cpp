#include "TerrainMap.h"
#include <iostream>

void TerrainMap::InitInfoLayer()
{
	m_InfoLayer.resize(m_MapSizeX, std::vector<TerrainTile*>(m_MapSizeY, NULL));

	TerrainTile *Temp = NULL;

	for (int i = 0; i < m_Map.size(); i++)
	{
		for (int x = 0; x < m_MapSizeX; x++)
		{
			for (int y = 0; y < m_MapSizeY; y++)
			{
				 Temp = &m_Map[i]->Get_Tile(Vec2i(x, y));

				if (Temp->Get_Collidable())
				{
					m_InfoLayer[x][y] = Temp;
				}
			}
		}
	}
}

bool TerrainMap::CheckMapCollision(Vec2f Pos)
{
	return	m_InfoLayer[int(Pos.x()) / m_TileSize][int(Pos.y()) / m_TileSize];
}

void TerrainMap::AddLayer(TerrainLayer* Layer, bool UpdateInfoLayerAfter = true)
{
	m_Map.push_back(Layer);

	if (UpdateInfoLayerAfter)
		UpdateInfoLayer(m_Map.size());
}


void TerrainMap::UpdateInfoLayer(int LayerChangeIndex)
{
	TerrainTile *Temp = NULL;
	for (int x = 0; x < m_MapSizeX; x++)
	{
		for (int y = 0; y < m_MapSizeY; y++)
		{
			Temp = &m_Map[LayerChangeIndex]->Get_Tile(Vec2i(x, y));
			if (Temp->Get_Collidable())
			{
				m_InfoLayer[x][y] = Temp;
			}
		}
	}
}

void TerrainMap::Event_Handler(ALLEGRO_EVENT &EV)
{
	static std::vector<Projectile*> Temp;

	for (int i = 0; i < m_Map.size(); i++)
	{
		m_Map[i]->Event_Handler(EV);
	}

	if (EV.type == PROJECTILE_EVENT)
	{
		Temp.push_back((Projectile*)EV.user.data1);
	}
	else if (EV.type == PLAYERMOVE_EVENT)
	{
		int PlayerX = EV.user.data1;
		int PlayerY = EV.user.data2;
	}

	if (Temp.size() > 0)
	{
 		for (int i = 0; i < Temp.size(); i++)
		{
			if (Temp[i] == NULL)
			{
				Temp.erase(Temp.begin() + i);
				i -= 1;
			}
			else if (CheckMapCollision(Vec2f(Temp[i]->GetHitBoxXBoundOne(), Temp[i]->GetHitBoxYBoundOne())))
			{
				Temp[i]->ResetProjectile();
			}
			else if (CheckMapCollision(Vec2f(Temp[i]->GetHitBoxXBoundTwo(), Temp[i]->GetHitBoxYBoundTwo())))
			{
				Temp[i]->ResetProjectile();
			}
		}
	}
}

void TerrainMap::Draw()
{
	for (int i = 0; i < m_Map.size(); i++)
	{
		m_Map[i]->Draw();
	}
}