//Created by:		Ryan Nolan-Hieb
//Date updated:		9/25/15
//Last update by:	N/A
//Reason for update:N/A

#include "Sprite.h"


void Sprite::Set_CurRow(int Row, bool ResetCurColumn)
{
	if (!m_Continuous && Row >= 0 && Row <= m_MaxFramesY)
	{
		m_CurRow = Row;
	}

	if (ResetCurColumn)
	{
		m_CurColumn = 0;
	}
}

void Sprite::Set_ImageAlpha(int R, int G, int B)
{
	al_convert_mask_to_alpha(m_Image, al_map_rgb(R, G, B));
}

void Sprite::Draw()
{
	al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, m_PosX, m_PosY, 0);
}


int Sprite::Event_Handler(ALLEGRO_EVENT& EV)
{
	if (EV.type == ALLEGRO_EVENT_TIMER)
	{
		Update();
	}
	return 0;
}

void Sprite::Update()
{
	if (m_Animated)
	{
		if (++m_FrameCount >= m_AnimFPS)
		{
			m_CurColumn++;
			
			if (m_CurColumn > m_MaxFramesX && m_LoopedAnim)
			{
				if (m_Continuous)
				{
					m_CurRow++;
					m_CurColumn = 0;
					if (m_CurRow > m_MaxFramesY)
					{
						m_CurRow = 0;
					}
				}
				else
				{
					m_CurColumn = 0;
				}
			}
			else if (m_CurColumn > m_MaxFramesX && !m_LoopedAnim)
			{
				if (m_Continuous)
				{
					m_CurRow++;
					if (m_CurRow > m_MaxFramesY)
					{
						m_CurRow--;
						m_CurColumn--;
					}
					else
					{
						m_CurColumn = 0;
					}
				}
				else
				{
					m_CurColumn--;
				}	
			}
			
			m_FrameCount = 0;
		}
	}
}