//Created by:		Ryan Nolan-Hieb
//Date updated:		9/25/15
//Last update by:	N/A
//Reason for update:N/A

#include "TerrainTile.h"

#include <allegro5\allegro_primitives.h>
#include <iostream>
void TerrainTile::Draw()
{
	/*Once I get some graphics to use this will all obviously change was we will not be drawing primitive rectangles*/
	if (m_TileType == Wall)
	{
		al_draw_filled_rectangle(m_PosX, m_PosY, m_PosX + m_Tile_Width, m_PosY + m_Tile_Height, al_map_rgb(255, 0, 0));
	}
	else if (m_TileType == Floor)
	{
		al_draw_filled_rectangle(m_PosX, m_PosY, m_PosX + m_Tile_Width, m_PosY + m_Tile_Height, al_map_rgb(0, 255, 0));
	}
	else if (m_TileType == Door)
	{
		al_draw_filled_rectangle(m_PosX, m_PosY, m_PosX + m_Tile_Width, m_PosY + m_Tile_Height, al_map_rgb(0, 0, 255));
	}
	else if (m_TileType == Grass)
	{
		al_draw_filled_rectangle(m_PosX, m_PosY, m_PosX + m_Tile_Width, m_PosY + m_Tile_Height, al_map_rgb(0, 255, 0));
	}
	else if (m_TileType == Water)
	{
		al_draw_filled_rectangle(m_PosX, m_PosY, m_PosX + m_Tile_Width, m_PosY + m_Tile_Height, al_map_rgb(0, 0, 255));
	}
	else if (m_TileType == Dirt)
	{
		al_draw_filled_rectangle(m_PosX, m_PosY, m_PosX + m_Tile_Width, m_PosY + m_Tile_Height, al_map_rgb(165, 42, 42));
	}
	else{
		std::cout << "failure" << std::endl;
	}
}