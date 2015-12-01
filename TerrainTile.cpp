//Created by:		Ryan Nolan-Hieb
//Date updated:		9/25/15
//Last update by:	N/A
//Reason for update:N/A

#include "TerrainTile.h"
#include "Utility.h"

#include <allegro5\allegro_primitives.h>

void TerrainTile::Draw()
{
	if (m_Retired)
		return;

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
		Sprite::Draw();
	}
	else
	{
		al_draw_bitmap_region(m_Image, m_StartFrameX * m_FrameWidth, m_StartFrameY * m_FrameHeight, m_FrameWidth, m_FrameHeight, m_PosX, m_PosY, 0);
	}
}

int TerrainTile::Event_Handler(ALLEGRO_EVENT &EV)
{
	if (m_Retired)
		return 0;

	if (!m_TriggerTile)
	{
		return Sprite::Event_Handler(EV);
	}
	else if (m_TriggerType == TR_LOOT)
	{
		if (m_HP <= 0)
		{
			
			EV.user.type = CUSTOM_EVENT_ID(TERRAINTILE_TRIGGER_EVENT);
			EV.user.data1 = (intptr_t)m_TriggerType;
			EV.user.data2 = (intptr_t)(m_PosX / m_FrameWidth);
			EV.user.data3 = (intptr_t)(m_PosY / m_FrameHeight);
			al_emit_user_event(&m_TerrainTriggerEvent, &EV, NULL);
			RetireTile();
		}
	}
	if (m_TriggerType == TR_FOG && EV.type == PLAYERPOSITION_EVENT)
	{
		
		int PlayerX = EV.user.data1;
		int PlayerY = EV.user.data2;

		if (CheckCollision(Vec2f(PlayerX, PlayerY)))
		{
			EmitEvent(EV);
		}
		
	}
	else if (m_TriggerType == TR_BOSS && EV.type == PLAYERPOSITION_EVENT)
	{
		int PlayerX = EV.user.data1;
		int PlayerY = EV.user.data2;

		if (CheckCollision(Vec2f(PlayerX, PlayerY)))
		{
			EmitEvent(EV);
		}
	}
	else if (m_TriggerType == TR_BOSS && EV.type == BOSS_KILLED_EVENT)
	{
		m_TriggerType = TR_RETURN;
		m_StartFrameX += 1;
	}
	else if (m_TriggerType == TR_RETURN && EV.type == PLAYERPOSITION_EVENT)
	{
		int PlayerX = EV.user.data1;
		int PlayerY = EV.user.data2;

		if (CheckCollision(Vec2f(PlayerX, PlayerY)))
		{
			//emit the event source that the projectile has moved
			EV.user.type = CUSTOM_EVENT_ID(DUNGEON_COMPLETE_EVENT);
			al_emit_user_event(&m_TerrainTriggerEvent, &EV, NULL);
		}
	}
}

void TerrainTile::RetireTile()
{
	m_Retired = true;
	m_HP = 0;
	m_Collidable = false;
	if (m_TriggerTile)
	{
		al_unregister_event_source(m_EventQueue, &m_TerrainTriggerEvent);
		m_TriggerTile = false;
	}
}

bool TerrainTile::CheckCollision(Vec2f Pos)
{
	if (int(Pos.x()) / m_FrameWidth == m_PosX / m_FrameWidth && int(Pos.y()) / m_FrameHeight == m_PosY / m_FrameHeight)
	{
		return true;
	}
	return false;
}

void TerrainTile::EmitEvent(ALLEGRO_EVENT& EV)
{
	//emit the event source that the projectile has moved
	EV.user.type = CUSTOM_EVENT_ID(TERRAINTILE_TRIGGER_EVENT);
	EV.user.data1 = (intptr_t)m_TriggerType;
	EV.user.data2 = (intptr_t)m_PosX;
	EV.user.data3 = (intptr_t)m_PosY;
	al_emit_user_event(&m_TerrainTriggerEvent, &EV, NULL);
}