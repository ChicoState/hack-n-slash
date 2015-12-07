//Created by:		Ryan Nolan-Hieb

#include "Sprite.h"

//sets the current row of animtion
void Sprite::Set_CurRow(int Row, bool ResetCurColumn)
{
	//simple bounds check
	if (!m_Continuous && Row >= 0 && Row <= m_MaxFramesY)
	{
		m_CurRow = Row;
	}

	//reseting the column back to 0
	if (ResetCurColumn)
	{
		m_CurColumn = 0;
	}
}

void Sprite::Set_ImageAlpha(int R, int G, int B)
{
	//set alphas on sprites
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
		Update(); //updates animations
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