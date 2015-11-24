//Scott Backer

//Projectile Class CPP: Projectile class is a projectile that can be used by other classes to create a projectile attack in game.

#include "Projectile.h"

//!Constructor for the Projectile class
//In - 
//		int XBound - the x bound of the projectile
//		int YBound - the y bound of the projectile
//		int ProjectileSpeed - the speed for the projectile
//		int StartingXPosition - the starting x positon of the projectile
//		int StartingYPosition - the starting y positon of the projectile
//		int XDirection - the direction the projectile will travel on the x axis
//		int YDirection - the direction the projectile will travel on the y axis
Projectile::Projectile(int XBound, int YBound, int ProjectileSpeed, int StartingXPosition, int StartingYPosition, int XDirection, int YDirection)
{
	m_XBound = XBound;
	m_YBound = YBound;
	m_ProjectileSpeed = ProjectileSpeed;
	m_CurrentXPosition = StartingXPosition;
	m_CurrentYPosition = StartingYPosition;
	m_CurrentXDirection = XDirection;
	m_CurrentYDirection = YDirection;

	m_RestingXPosition = 0;
	m_RestingYPosition = 0;
}

//!Draws the projectile
void Projectile::Draw()
{
	//if not at reset (standby) locatioon draw projectile
	//if(m_CurrentXPosition != m_RestingXPosition && m_CurrentYPosition != m_RestingYPosition)
	//{
		//draw sprite up
		if(m_CurrentXDirection == 0 && m_CurrentYDirection == -1)
		{
			//Set_CurRow(3, false);
			//al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, (m_CurrentXPosition - m_FrameWidth / 2), (m_CurrentYPosition - m_FrameHeight / 2), 0);

			al_draw_rectangle(m_CurrentXPosition - 6, m_CurrentYPosition - 8, m_CurrentXPosition + 6, m_CurrentYPosition + 8, al_map_rgb(255, 0, 2550), 5);
		}

		//draw sprite down
		else if(m_CurrentXDirection == 0 && m_CurrentYDirection == 1)
		{
			//Set_CurRow(0, false);
			//al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, (m_CurrentXPosition - m_FrameWidth / 2), (m_CurrentYPosition - m_FrameHeight / 2), 0);

			al_draw_rectangle(m_CurrentXPosition - 6, m_CurrentYPosition - 8, m_CurrentXPosition + 6, m_CurrentYPosition + 8, al_map_rgb(255, 0, 2550), 5);
		}

		//draw sprite left
		else if(m_CurrentXDirection == -1 && m_CurrentYDirection == 0)
		{
			//Set_CurRow(1, false);
			//al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, (m_CurrentXPosition - m_FrameWidth / 2), (m_CurrentYPosition - m_FrameHeight / 2), 0);

			al_draw_rectangle(m_CurrentXPosition - 6, m_CurrentYPosition - 8, m_CurrentXPosition + 6, m_CurrentYPosition + 8, al_map_rgb(255, 0, 2550), 5);
		}

		//draw sprite right
		else if(m_CurrentXDirection == 1 && m_CurrentYDirection == 0)
		{
			//Set_CurRow(2, false);
			//al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, (m_CurrentXPosition - m_FrameWidth / 2), (m_CurrentYPosition - m_FrameHeight / 2), 0);

			al_draw_rectangle(m_CurrentXPosition - 6, m_CurrentYPosition - 8, m_CurrentXPosition + 6, m_CurrentYPosition + 8, al_map_rgb(255, 0, 2550), 5);
		}

		//else draw a default direction
		else
		{
			//Set_CurRow(1, false);
			//al_draw_bitmap_region(m_Image, m_CurColumn * m_FrameWidth, m_CurRow * m_FrameHeight, m_FrameWidth, m_FrameHeight, (m_CurrentXPosition - m_FrameWidth / 2), (m_CurrentYPosition - m_FrameHeight / 2), 0);
			al_draw_rectangle(m_CurrentXPosition - 6, m_CurrentYPosition - 8, m_CurrentXPosition + 6, m_CurrentYPosition + 8, al_map_rgb(255, 0, 2550), 5);
		}
	//}
}

//Re-sends the projectile in the attacking direction
//In - 
//		int StartingXPosition - the starting x positon of the projectile
//		int StartingYPosition - the starting y positon of the projectile
//		int XDirection - the direction the projectile will travel on the x axis
//		int YDirection - the direction the projectile will travel on the y axis
void Projectile::SendProjecile(int StartingXPosition, int StartingYPosition, int XDirection, int YDirection)
{
	m_CurrentXPosition = StartingXPosition;
	m_CurrentYPosition = StartingYPosition;
	m_CurrentXDirection = XDirection;
	m_CurrentYDirection = YDirection;
}

//!Update sthe position of the projectile
void Projectile::UpdatePosition()
{
	//move up
	if(m_CurrentXDirection == 0 && m_CurrentYDirection == -1)
	{
		m_CurrentYPosition -= m_ProjectileSpeed;
	}

	//move down
	else if(m_CurrentXDirection == 0 && m_CurrentYDirection == 1)
	{
		m_CurrentYPosition += m_ProjectileSpeed;
	}

	//move left
	else if(m_CurrentXDirection == -1 && m_CurrentYDirection == 0)
	{
		m_CurrentXPosition += m_ProjectileSpeed;
	}

	//move right
	else if(m_CurrentXDirection == 1 && m_CurrentYDirection == 0)
	{
		m_CurrentXPosition -= m_ProjectileSpeed;
	}

	else
	{
		m_CurrentYPosition -= m_ProjectileSpeed;
	}
}

//!Gets and returns the x bound one of the projectile
//Out - 
//		int - the x bound one of the projectile
int Projectile::GetHitBoxXBoundOne()
{
	if(m_CurrentXDirection == 0 && m_CurrentYDirection == -1)
	{
		return (m_CurrentXPosition - (m_XBound / 2));
	}

	else if(m_CurrentXDirection == 0 && m_CurrentYDirection == 1)
	{
		return (m_CurrentXPosition + (m_XBound / 2));
	}

	else if(m_CurrentXDirection == -1 && m_CurrentYDirection == 0)
	{
		return (m_CurrentXPosition + (m_XBound / 2));
	}

	else if(m_CurrentXDirection == 1 && m_CurrentYDirection == 0)
	{
		return (m_CurrentXPosition - (m_XBound / 2));
	}

	else
	{
		return 0;
	}
}

//!Gets and returns the y bound one of the projectile
//Out - 
//		int - the y bound one of the projectile
int Projectile::GetHitBoxYBoundOne()
{
	if(m_CurrentXDirection == 0 && m_CurrentYDirection == -1)
	{
		return (m_CurrentYPosition - (m_YBound / 2));
	}

	else if(m_CurrentXDirection == 0 && m_CurrentYDirection == 1)
	{
		return (m_CurrentYPosition + (m_YBound / 2));
	}

	else if(m_CurrentXDirection == -1 && m_CurrentYDirection == 0)
	{
		return (m_CurrentYPosition - (m_YBound / 2));
	}

	else if(m_CurrentXDirection == 1 && m_CurrentYDirection == 0)
	{
		return (m_CurrentYPosition + (m_YBound / 2));
	}

	else
	{
		return 0;
	}
}

//!Gets and returns the x bound two of the projectile
//Out - 
//		int - the x bound two of the projectile
int Projectile::GetHitBoxXBoundTwo()
{
	if(m_CurrentXDirection == 0 && m_CurrentYDirection == -1)
	{
		return (m_CurrentXPosition + (m_XBound / 2));
	}

	else if(m_CurrentXDirection == 0 && m_CurrentYDirection == 1)
	{
		return (m_CurrentXPosition - (m_XBound / 2));
	}

	else if(m_CurrentXDirection == -1 && m_CurrentYDirection == 0)
	{
		return (m_CurrentXPosition - (m_XBound / 2));
	}

	else if(m_CurrentXDirection == 1 && m_CurrentYDirection == 0)
	{
		return (m_CurrentXPosition + (m_XBound / 2));
	}

	else
	{
		return 0;
	}
}

//!Gets and returns the y bound two of the projectile
//Out - 
//		int - the y bound two of the projectile
int Projectile::GetHitBoxYBoundTwo()
{
	if(m_CurrentXDirection == 0 && m_CurrentYDirection == -1)
	{
		return (m_CurrentYPosition + (m_YBound / 2));
	}

	else if(m_CurrentXDirection == 0 && m_CurrentYDirection == 1)
	{
		return (m_CurrentYPosition - (m_YBound / 2));
	}

	else if(m_CurrentXDirection == -1 && m_CurrentYDirection == 0)
	{
		return (m_CurrentYPosition + (m_YBound / 2));
	}

	else if(m_CurrentXDirection == 1 && m_CurrentYDirection == 0)
	{
		return (m_CurrentYPosition - (m_YBound / 2));
	}

	else
	{
		return 0;
	}
}

//!Resets the projectile to its resting position (destroys it technically)
void Projectile::ResetProjectile()
{
	//reset the projectile to resting position
	m_CurrentXPosition = m_RestingXPosition;
	m_CurrentYPosition = m_RestingYPosition;
}

//!Checks to see if the projectile is at its resting position (destroyed technically)
//Out - 
//		bool - //return true if the projectile is at its resting position else false
bool Projectile::IsAtRestingPosition()
{
	//return true if the projectile is at its resting position else false
	if(m_CurrentXPosition == m_RestingXPosition && m_CurrentYPosition == m_RestingYPosition)
	{
		return true;
	}

	else
	{
		return false;
	}
}