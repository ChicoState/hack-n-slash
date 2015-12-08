//Created by:		Ryan Nolan-Hieb

#include "TerrainTile.h"
#include "Utility.h"

#include <allegro5\allegro_primitives.h>

void TerrainTile::Draw()
{
	//don't draw if the tile is retired
	if (m_Retired)
		return;

	//if the image data is NULL we can still draw some basic shapes to represent that area
	if (m_Image == NULL)
	{
		if (m_TileType == Wall)
		{
			al_draw_filled_rectangle(m_PosX, m_PosY, m_PosX + m_FrameWidth, m_PosY + m_FrameHeight, al_map_rgb(22, 18, 17));
		}
		else if (m_TileType == Floor)
		{
			al_draw_filled_rectangle(m_PosX, m_PosY, m_PosX + m_FrameWidth, m_PosY + m_FrameHeight, al_map_rgb(0, 255, 0));
		}
		else if (m_TileType == Door)
		{
			al_draw_filled_rectangle(m_PosX, m_PosY, m_PosX + m_FrameWidth, m_PosY + m_FrameHeight, al_map_rgb(0, 0, 255));
		}
		else if (m_TileType == Fog)
		{
			al_draw_filled_rectangle(m_PosX, m_PosY, m_PosX + m_FrameWidth, m_PosY + m_FrameHeight, al_map_rgb(50, 46, 45));
		}
	}
	else if (m_Animated == true)
	{
		//if this tile has animation play it through the base sprite draw, which has animation support already
		Sprite::Draw();
	}
	else //the tile is a static sprite that only needs to have one section of the sprite sheet to be drawn
	{
		al_draw_bitmap_region(m_Image, m_StartFrameX * m_FrameWidth, m_StartFrameY * m_FrameHeight, m_FrameWidth, m_FrameHeight, m_PosX, m_PosY, 0);
	}
}

int TerrainTile::Event_Handler(ALLEGRO_EVENT &EV)
{
	//we don't care about events on retired tiles
	if (m_Retired)
		return 0;

	//run through basic sprite events
	Sprite::Event_Handler(EV);
	
	//if it's not a trigger tile we can leave now
	if (!m_TriggerTile)
		return 0;

	//Trigger events
	if (m_TriggerType == TR_LOOT)
	{
		if (m_HP <= 0) //did this tile die?
		{	
			//if so then emit an event saying so and then retire the tile
			EV.user.type = CUSTOM_EVENT_ID(TERRAINTILE_TRIGGER_EVENT);
			EV.user.data1 = (intptr_t)m_TriggerType;
			EV.user.data2 = (intptr_t)(m_PosX / m_FrameWidth);
			EV.user.data3 = (intptr_t)(m_PosY / m_FrameHeight);
			al_emit_user_event(&m_TerrainTriggerEvent, &EV, NULL);
			RetireTile();
		}
	}
	else if (m_TriggerType == TR_FOG && EV.type == PLAYERPOSITION_EVENT) //this tile is Fog and the player is moving
	{
		//lets check to see if the player is walking into fog
		int PlayerX = EV.user.data1;
		int PlayerY = EV.user.data2;

		if (CheckCollision(AVec2f(PlayerX, PlayerY)))
		{
			//the player walked into a fog zone. Emit an event saying so
			EmitEvent(EV);
		}
		
	}
	else if (m_TriggerType == TR_BOSS && EV.type == PLAYERPOSITION_EVENT) //this tile is a boss spawner and the player is moving
	{
		//lets check to see if the player is walking onto the boss spawner
		int PlayerX = EV.user.data1;
		int PlayerY = EV.user.data2;

		if (CheckCollision(AVec2f(PlayerX, PlayerY)))
		{
			//the player walked into a boss zone. Emit an event saying so
			EmitEvent(EV);
		}
	}
	else if (m_TriggerType == TR_BOSS && EV.type == BOSS_KILLED_EVENT) //this tile is a boss spawner and the boss has been killed
	{
		//the trigger type will be changed so that the boss spawner turns into a portal to start the next dungeon
		m_TriggerType = TR_RETURN;
		//changing the texture to use
		m_StartFrameX += 1;
	}
	else if (m_TriggerType == TR_RETURN && EV.type == PLAYERPOSITION_EVENT) //this tile is a new dungeon spawner and the player is moving
	{
		//lets check to see if the player is walking onto the tile
		int PlayerX = EV.user.data1;
		int PlayerY = EV.user.data2;

		if (CheckCollision(AVec2f(PlayerX, PlayerY)))
		{
			//emit the event source that the Dungeon has been completed
			EV.user.type = CUSTOM_EVENT_ID(DUNGEON_COMPLETE_EVENT);
			al_emit_user_event(&m_TerrainTriggerEvent, &EV, NULL);
		}
	}

	return 0;
}

void TerrainTile::RetireTile()
{
	//retire the tile
	m_Retired = true;
	m_HP = 0;
	m_Collidable = false;
	if (m_TriggerTile)
	{
		al_unregister_event_source(m_EventQueue, &m_TerrainTriggerEvent);
		m_TriggerTile = false;
	}
}

bool TerrainTile::CheckCollision(AVec2f Pos)
{
	//returns true if the given position is contained within the tile, else returns false
	if (int(Pos.x()) / m_FrameWidth == m_PosX / m_FrameWidth && int(Pos.y()) / m_FrameHeight == m_PosY / m_FrameHeight)
	{
		return true;
	}
	return false;
}

void TerrainTile::EmitEvent(ALLEGRO_EVENT& EV)
{
	//emit a custom event
	EV.user.type = CUSTOM_EVENT_ID(TERRAINTILE_TRIGGER_EVENT);
	EV.user.data1 = (intptr_t)m_TriggerType; //the trigger type of the tile
	EV.user.data2 = (intptr_t)m_PosX; //the x postion of the tile
	EV.user.data3 = (intptr_t)m_PosY; //the y postion of the tile
	al_emit_user_event(&m_TerrainTriggerEvent, &EV, NULL);
}