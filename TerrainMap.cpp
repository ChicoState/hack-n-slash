#include "TerrainMap.h"
#include "Utility.h"
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
	int X = int(Pos.x()) / m_TileSize;
	int Y = int(Pos.y()) / m_TileSize;
	return	m_InfoLayer[X][Y];
}

void TerrainMap::AddLayer(TerrainLayer* Layer, bool UpdateInfoLayerAfter = true)
{
	m_Map.push_back(Layer);

	if (UpdateInfoLayerAfter)
		UpdateInfoLayer(m_Map.size() - 1);
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

	m_ObjectManager.Event_Handler(EV);

	for (int i = 0; i < m_Map.size(); i++)
	{
		m_Map[i]->Event_Handler(EV);
	}

	if (EV.type == PROJECTILE_EVENT)
	{
		Temp.push_back((Projectile*)EV.user.data1);
	}
	else if (EV.type == TERRAINTILE_TRIGGER_EVENT)
	{
		if ((TRIGGER)EV.user.data1 == TR_BOSS)
		{
			
		}
		else if ((TRIGGER)EV.user.data1 == TR_LOOT)
		{
			Vec2i Pos(EV.user.data2, EV.user.data3);

			for (int i = 0; i < m_Map.size(); i++)
			{
				if (m_Map[i]->Get_Tile(Pos).Get_TriggerType() == TR_LOOT)
				{
					m_InfoLayer[Pos.x()][Pos.y()] = NULL;
					m_Map[i]->CreateBitmap(NULL);
					CreatePickupObjects(Pos);
				}					
			}

		}
	}

	if (Temp.size() > 0)
	{
 		for (int i = 0; i < Temp.size(); i++)
		{
			if (Temp[i] == NULL)
			{
				Temp.erase(Temp.begin() + i);
				i -= 1;
				continue;
			}

			Vec2f Pos(Temp[i]->GetHitBoxXBoundOne(), Temp[i]->GetHitBoxYBoundOne());

			if (std::abs(Pos.x()) == 8)
			{
				continue;
			}


			if (CheckMapCollision(Pos))
			{
				int TileHP = m_InfoLayer[Pos.x() / m_TileSize][Pos.y() / m_TileSize]->Get_TileHP();
				if (TileHP < 1000)
				{
					m_InfoLayer[Pos.x() / m_TileSize][Pos.y() / m_TileSize]->Set_TileHP(TileHP - m_MainPlayer->GetWeaponDamage());
				}
				Temp[i]->ResetProjectile();
				continue;
			}

			Pos = Vec2f(Temp[i]->GetHitBoxXBoundTwo(), Temp[i]->GetHitBoxYBoundTwo());
			if (CheckMapCollision(Pos))
			{
				int TileHP = m_InfoLayer[Pos.x() / m_TileSize][Pos.y() / m_TileSize]->Get_TileHP();
				if (TileHP < 1000)
				{
					m_InfoLayer[Pos.x() / m_TileSize][Pos.y() / m_TileSize]->Set_TileHP(TileHP - m_MainPlayer->GetWeaponDamage());
				}
				Temp[i]->ResetProjectile();
				continue;
			}
		}
	}
}


void TerrainMap::CreatePickupObjects(Vec2i Pos)
{
	int X = Random(Pos.x()*m_TileSize, Pos.x()*m_TileSize + m_TileSize);
	int Y = Random(Pos.y()*m_TileSize, Pos.y()*m_TileSize + m_TileSize);

	m_ObjectManager.SpawnObjectRandom(Vec2i(X, Y), 35);
}

void TerrainMap::Draw()
{
	for (int i = 0; i < m_Map.size(); i++)
	{
		m_Map[i]->Draw();
	}

	m_ObjectManager.Draw();
}