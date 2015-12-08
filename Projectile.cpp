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
		:	m_ProjectileSprite(StartingXPosition, StartingYPosition, 44, 44, true, false, false, false, 6)
{
	//set member variables
	m_XBound = XBound;
	m_YBound = YBound;
	m_ProjectileSpeed = ProjectileSpeed;
	m_CurrentXPosition = StartingXPosition;
	m_CurrentYPosition = StartingYPosition;
	m_CurrentXDirection = XDirection;
	m_CurrentYDirection = YDirection;

	//initialize member variables
	m_RestingXPosition = 0;
	m_RestingYPosition = 0;
}

//!Draws the projectile
void Projectile::Draw()
{
	m_ProjectileSprite.Draw(m_CurrentXPosition, m_CurrentYPosition, m_CurrentXDirection, m_CurrentYDirection);
}

//Re-sends the projectile in the attacking direction
//In - 
//		int StartingXPosition - the starting x positon of the projectile
//		int StartingYPosition - the starting y positon of the projectile
//		int XDirection - the direction the projectile will travel on the x axis
//		int YDirection - the direction the projectile will travel on the y axis
void Projectile::SendProjecile(int StartingXPosition, int StartingYPosition, int XDirection, int YDirection)
{
	//set initial position and direction
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
	//depending on the direction, get the hitbox direction

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
	//depending on the direction, get the hitbox direction

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
	//depending on the direction, get the hitbox direction

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
	//depending on the direction, get the hitbox direction

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