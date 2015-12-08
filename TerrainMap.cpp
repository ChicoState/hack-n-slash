//Created By:	Ryan Nolan-Hieb

#include "TerrainMap.h"
#include "Utility.h"
#include <iostream>



void TerrainMap::InitInfoLayer()
{
	//file the infor layer with a bunch of null pointers
	m_InfoLayer.resize(m_MapSizeX, std::vector<TerrainTile*>(m_MapSizeY, NULL));

	//then updates it
	UpdateInfoLayer(0);
}

bool TerrainMap::CheckMapCollision(AVec2f Pos)
{
	//checks the info layer to see if there's a valid pointer there. 
	//If there is then there's collision, else no collision
	int X = int(Pos.x()) / m_TileSize;
	int Y = int(Pos.y()) / m_TileSize;
	return	m_InfoLayer[X][Y];
}

void TerrainMap::AddLayer(TerrainLayer* Layer, bool UpdateInfoLayerAfter = true)
{
	m_Map.push_back(Layer);

	//update the info layer after the new one gets added
	if (UpdateInfoLayerAfter)
		UpdateInfoLayer(m_Map.size() - 1);
}


void TerrainMap::UpdateInfoLayer(int LayerChangeIndex)
{
	//if the index is out of bounds get out
	if (LayerChangeIndex < 0 || LayerChangeIndex > m_Map.size())
		return;

	TerrainTile *Temp = NULL;

	for (int x = 0; x < m_MapSizeX; x++)
	{
		for (int y = 0; y < m_MapSizeY; y++)
		{
			Temp = &m_Map[LayerChangeIndex]->Get_Tile(AVec2i(x, y));

			if (Temp->Get_Collidable())
			{
				//if the tile has collision add it to the info layer
				m_InfoLayer[x][y] = Temp;
			}
		}
	}
}

void TerrainMap::Event_Handler(ALLEGRO_EVENT &EV)
{
	//run the event handeler for all of the pickup objects
	m_ObjectManager.Event_Handler(EV);

	//run the event handeler for all of the map layers
	for (unsigned int i = 0; i < m_Map.size(); i++)
	{
		m_Map[i]->Event_Handler(EV);
	}

	//this really should be changed in the future. But currently whenever a projectile gets fired by the player
	//a pointer to the projectile is statically stored here and then constanstly checked every tick to see if it 
	//collides anywhere with the dungeon
	static std::vector<Projectile*> GameProjectiles;

	if (EV.type == PROJECTILE_EVENT) //a projectile was just created
	{
		//store it
		GameProjectiles.push_back((Projectile*)EV.user.data1);
	}
	else if (EV.type == MELEEATTACK_EVENT) //user attacked with a melee weapon
	{
		AVec2f Pos(EV.user.data1, EV.user.data2);
		
		//check to see if it collided with anything in the terrain
		if (CheckMapCollision(Pos))
		{
			int TileHP = m_InfoLayer[Pos.x() / m_TileSize][Pos.y() / m_TileSize]->Get_TileHP();
			if (TileHP < 1000)
			{
				//if it did and the tild can be damaged make it take damage
				m_InfoLayer[Pos.x() / m_TileSize][Pos.y() / m_TileSize]->Set_TileHP(TileHP - m_MainPlayer->GetWeaponDamage());
			}
		}
	}
	else if (EV.type == TERRAINTILE_TRIGGER_EVENT) //checks for events emitted by individual tiles
	{
		if ((TRIGGER)EV.user.data1 == TR_LOOT) //loot needs to be dropped
		{
			AVec2i Pos(EV.user.data2, EV.user.data3);

			for (unsigned int i = 0; i < m_Map.size(); i++)
			{
				if (m_Map[i]->Get_Tile(Pos).Get_TriggerType() == TR_LOOT)
				{
					//finds the layer the tile is on then... 
					m_InfoLayer[Pos.x()][Pos.y()] = NULL; //gets rid of the pointer in the info layer
					m_Map[i]->CreateBitmap(NULL); //tells that layer to update it's image
					CreatePickupObjects(Pos); //creates the pickup object
					break; //get out since we're done now
				}					
			}

		}
		else if ((TRIGGER)EV.user.data1 == TR_FOG) //the player ran into fog
		{
			AVec2i Pos(EV.user.data2 / m_TileSize, EV.user.data3 / m_TileSize);

			for (unsigned int i = 0; i < m_Map.size(); i++)
			{
				if (m_Map[i]->Get_Tile(Pos).Get_TriggerType() == TR_FOG)
				{
					//finds the layer the tile is on then... 
					ClearFog(Pos, i); //gets rid of the fog in that room
					m_Map[i]->CreateBitmap(NULL); //recreates the image for that layer
				}
			}
		}
	}

	if (GameProjectiles.size() > 0) //if there's projectiles on the map
	{
		CheckProjectiles(GameProjectiles); //check to make sure they aren't colliding with anything
	}
}

void TerrainMap::CheckProjectiles(std::vector<Projectile*>& GameProjectiles)
{
	for (unsigned int i = 0; i < GameProjectiles.size(); i++)
	{
		if (GameProjectiles[i] == NULL)
		{
			//if the projectile doesn't exist anymore get rid of it
			GameProjectiles.erase(GameProjectiles.begin() + i);
			i -= 1;
			continue;
		}

		AVec2f Pos(GameProjectiles[i]->GetHitBoxXBoundOne(), GameProjectiles[i]->GetHitBoxYBoundOne());

		//if the projectile is at the resting position get out
		if (std::abs(Pos.x()) == 8)
		{
			continue;
		}

		//check if the first bounding boxes collide with any tiles
		if (CheckMapCollision(Pos))
		{
			//if it hits then check if it hit a destructible tile
			int TileHP = m_InfoLayer[Pos.x() / m_TileSize][Pos.y() / m_TileSize]->Get_TileHP();
			if (TileHP < 1000)
			{
				//take damage
				m_InfoLayer[Pos.x() / m_TileSize][Pos.y() / m_TileSize]->Set_TileHP(TileHP - m_MainPlayer->GetWeaponDamage());
			}
			//reset the projectile
			GameProjectiles[i]->ResetProjectile();
			continue;
		}

		Pos = AVec2f(GameProjectiles[i]->GetHitBoxXBoundTwo(), GameProjectiles[i]->GetHitBoxYBoundTwo());

		//check if the second bounding boxes collide with any tiles
		if (CheckMapCollision(Pos))
		{
			//if it hits then check if it hit a destructible tile
			int TileHP = m_InfoLayer[Pos.x() / m_TileSize][Pos.y() / m_TileSize]->Get_TileHP();
			if (TileHP < 1000)
			{
				//take damage
				m_InfoLayer[Pos.x() / m_TileSize][Pos.y() / m_TileSize]->Set_TileHP(TileHP - m_MainPlayer->GetWeaponDamage());
			}
			//reset the projectile
			GameProjectiles[i]->ResetProjectile();
			continue;
		}
	}
}

void TerrainMap::ClearFog(AVec2i Pos, int FogLayer)
{
	//Cardinal system similar to the one found in the dungeon generator
	std::vector<AVec2i> Cardinal;
	Cardinal.push_back(AVec2i(0, 1));
	Cardinal.push_back(AVec2i(1, 0));
	Cardinal.push_back(AVec2i(0, -1));
	Cardinal.push_back(AVec2i(-1, 0));

	//retire the current tile
	m_Map[FogLayer]->Get_Tile(Pos).RetireTile();

	//go in every direction
	for (unsigned int i = 0; i < Cardinal.size(); i++)
	{
		//and clear fog on any of those tiles that are fog tiles that are also not retired yet
		if (m_Map[FogLayer]->Get_Tile(Pos + Cardinal[i]).Get_TileType() == Fog && !m_Map[FogLayer]->Get_Tile(Pos + Cardinal[i]).Get_Retired())
		{
			ClearFog(Pos + Cardinal[i], FogLayer);
		}
	}
}

void TerrainMap::CreatePickupObjects(AVec2i Pos)
{
	int X = Random(Pos.x()*m_TileSize, Pos.x()*m_TileSize + m_TileSize);
	int Y = Random(Pos.y()*m_TileSize, Pos.y()*m_TileSize + m_TileSize);

	//35% chance to spawn something
	m_ObjectManager.SpawnObjectRandom(AVec2i(X, Y), 35);
}

void TerrainMap::Draw(bool PrePlayerDraw)
{
	//draw all of the layers that are supposed to be drawn before the player and AI
	if (PrePlayerDraw)
	{
		for (unsigned int i = 0; i < m_Map.size(); i++)
		{
			if (m_Map[i]->Get_PrePlayerDraw())
			{
				m_Map[i]->Draw();
			}
		}
		//that includes all of the pickup objects
		m_ObjectManager.Draw();
	}
	else //draw all of the layers that are supposed to be drawn after the player and AI
	{
		for (unsigned int i = 0; i < m_Map.size(); i++)
		{
			if (!m_Map[i]->Get_PrePlayerDraw())
			{
				m_Map[i]->Draw();
			}
		}
	}


	
}