//Scott Backer

//Weapon Class CPP: Creates the use of a weapon for a player/AI and a parent class for all weapon types.

#include "Weapon.h"

//!Constructor for the weapon class
//In - 
//		ALLEGRO_EVENT& InputAlEvent - the allegro event of the game system for the weapon to use
//		int InputXBound - the x bound for the weapon
//		int InputYBound - the y bound for the weapon
//		bool IsRangedWeapon - true if the weapon is a ranged weapon else false
//		float InputAttackTime - the time in seconds the weapon will be active when used to attack
//		float InputDamage - the damage the weapon does
//		ALLEGRO_BITMAP *SpriteImage - the sprite image of the player tile
//		int InputPlayerXPosition - the starting x position of the player
//		int InputPlayerXPosition - the starting y position of the player
//		int FrameWidth - the width of one sprite frame
//		int FrameHeight - the height of one sprite frame
//		bool Collidable - true if the tile is collidable else false
//		bool Animated - true if the tile is animated else false
//		bool Continuous - true if the tile is continues else false
//		bool Looped - true if the tile should loop else false
//		int AnimFPS - the fps for the animated tile
Weapon::Weapon(ALLEGRO_EVENT& InputAlEvent, int InputXBound, int InputYBound, bool IsRangedWeapon, float InputAttackTime, float InputDamage)
{
	//initialize member variables
	m_AlEvent = InputAlEvent;
	m_XBound = InputXBound;
	m_YBound = InputYBound;
	m_IsRangedWeapon = IsRangedWeapon;
	m_ProjectileXBound = 0;
	m_ProjectileYBound = 0;
	m_ProjectileXPosition = 0;
	m_ProjectileYPosition = 0;
	m_ProjectileSpeed = 6;
	m_IsActive = false;
	m_LastDrawnDirection = Direction(North);
	m_CurrentProjectileDirection = Direction(North);
	m_AttackTime = (InputAttackTime * 60);
	m_CurrentAttackCount = 0;
	m_Damage = InputDamage;
	m_DamageModifier = 1;
}

//!Handles allegro events for the weapon class
void Weapon::EventHandler()
{
	if(m_AlEvent.type = ALLEGRO_EVENT_TIMER)
	{
		//if the weapon is active watch the active timer
		if(m_IsActive)
		{
			//iterate the timer
			m_CurrentAttackCount++;

			//move the projectile if a ranged weapon
			if(m_IsRangedWeapon)
			{
				if(m_CurrentProjectileDirection == Direction(North))
				{
					m_ProjectileYPosition -= m_ProjectileSpeed;
				}

				else if(m_CurrentProjectileDirection == Direction(South))
				{
					m_ProjectileYPosition += m_ProjectileSpeed;
				}

				else if(m_CurrentProjectileDirection == Direction(East))
				{
					m_ProjectileXPosition += m_ProjectileSpeed;
				}

				else if(m_CurrentProjectileDirection == Direction(West))
				{
					m_ProjectileXPosition -= m_ProjectileSpeed;
				}

				else
				{
					m_ProjectileYPosition -= m_ProjectileSpeed;
				}
			}

			//if the active timer is reached
			if(m_CurrentAttackCount >= m_AttackTime)
			{
				//make weapon unactive and reset timer
				m_IsActive = false;
				m_CurrentAttackCount = 0;
				m_ProjectileXPosition = 0;
				m_ProjectileYPosition = 0;

			}
		}
	}
}

//!Handles drawing for the weapon class
//In - 
//		int DrawXCoordinate - DrawXCoordinate - the x coordinate to draw the weapon at
//		int DrawYCoordinate - DrawXCoordinate - the y coordinate to draw the weapon at
//		int XDirection - the x direction relative to the player/AI using the weapon to draw at (use graph coordinates to calculate (Ex. 0, 1 is North or Up))
//		int YDirection - the y direction relative to the player/AI using the weapon to draw at (use graph coordinates to calculate (Ex. 0, 1 is North or Up))
void Weapon::Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection)
{
	m_LastDrawnXPosition = DrawXCoordinate;
	m_LastDrawnYPosition = DrawYCoordinate;

	if(XDirection = 0 && YDirection == -1)
	{
		m_LastDrawnDirection = Direction(North);
	}

	else if(XDirection = 0 && YDirection == 1)
	{
		m_LastDrawnDirection = Direction(South);
	}

	else if(XDirection = 1 && YDirection == 0)
	{
		m_LastDrawnDirection = Direction(East);
	}

	else if(XDirection = -1 && YDirection == 0)
	{
		m_LastDrawnDirection = Direction(West);
	}

	else
	{
		m_LastDrawnDirection = Direction(North);
	}
}

//!Resets the weapon to a non active state
void Weapon::ResetWeapon()
{
	m_IsActive = false;
	m_CurrentAttackCount = 0;
}

//!Returns whether or not the weapon is active
//Out- 
//		bool - true if the weapon is active else false
bool Weapon::IsActive()
{
	return m_IsActive;
}

//!Attacks with the weapon to make it active
void Weapon::Attack()
{
	m_IsActive = true;
	m_CurrentAttackCount = 0;

	if(m_IsRangedWeapon)
	{
		m_ProjectileXPosition = m_LastDrawnXPosition;
		m_ProjectileYPosition = m_LastDrawnYPosition;
		m_CurrentProjectileDirection = m_LastDrawnDirection;
	}
}

//!Gets and returns the weapon x bound
//Out - 
//		int - x bound of the weapon
int Weapon::GetXBound()
{
	return m_XBound;
}

//!Gets and returns the weapon y bound
//Out - 
//		int - y bound of the weapon
int Weapon::GetYBound()
{
	return m_YBound;
}

//!Gets and returns the weapon/projectile first hitbox x bound (will always be top left in direction)
//Out - 
//		int - x bound of the weapon/projectile hitbox
int Weapon::GetHitBoxXBoundOne()
{
	if(m_IsActive)
	{
		if(m_IsRangedWeapon)
		{
			if(m_CurrentProjectileDirection == Direction(North))
			{
				return (m_ProjectileXPosition - (m_ProjectileXBound / 2));
			}

			if(m_CurrentProjectileDirection == Direction(South))
			{
				return (m_ProjectileXPosition + (m_ProjectileXBound / 2));
			}

			if(m_CurrentProjectileDirection == Direction(East))
			{
				return (m_ProjectileXPosition + (m_ProjectileXBound / 2));
			}

			if(m_CurrentProjectileDirection == Direction(West))
			{
				return (m_ProjectileXPosition - (m_ProjectileXBound / 2));
			}
		}

		else
		{
			if(m_LastDrawnDirection == Direction(North))
			{
				return (m_LastDrawnXPosition - (m_XBound / 2));
			}

			if(m_LastDrawnDirection == Direction(South))
			{
				return (m_LastDrawnXPosition + (m_XBound / 2));
			}

			if(m_LastDrawnDirection == Direction(East))
			{
				return (m_LastDrawnXPosition + (m_XBound / 2));
			}

			if(m_LastDrawnDirection == Direction(West))
			{
				return (m_LastDrawnXPosition - (m_XBound / 2));
			}
		}
	}

	else
	{
		return 0;
	}
}

//!Gets and returns the weapon/projectile first hitbox y bound (will always be top left in direction)
//Out - 
//		int - y bound of the weapon/projectile hitbox
int Weapon::GetHitBoxYBoundOne()
{
	if(m_IsActive)
	{
		if(m_IsRangedWeapon)
		{
			if(m_CurrentProjectileDirection == Direction(North))
			{
				return (m_ProjectileYPosition - (m_ProjectileYBound / 2));
			}

			if(m_CurrentProjectileDirection == Direction(South))
			{
				return (m_ProjectileYPosition + (m_ProjectileYBound / 2));
			}

			if(m_CurrentProjectileDirection == Direction(East))
			{
				return (m_ProjectileYPosition - (m_ProjectileYBound / 2));
			}

			if(m_CurrentProjectileDirection == Direction(West))
			{
				return (m_ProjectileYPosition + (m_ProjectileYBound / 2));
			}
		}

		else
		{
			if(m_LastDrawnDirection == Direction(North))
			{
				return (m_LastDrawnYPosition - (m_YBound / 2));
			}

			if(m_LastDrawnDirection == Direction(South))
			{
				return (m_LastDrawnYPosition + (m_YBound / 2));
			}

			if(m_LastDrawnDirection == Direction(East))
			{
				return (m_LastDrawnYPosition - (m_YBound / 2));
			}

			if(m_LastDrawnDirection == Direction(West))
			{
				return (m_LastDrawnYPosition + (m_YBound / 2));
			}
		}
	}

	else
	{
		return 0;
	}
}

//!Gets and returns the weapon/projectile second hitbox x bound (will always be bottom right in direction)
//Out - 
//		int - x bound of the weapon/projectile hitbox
int Weapon::GetHitBoxXBoundTwo()
{
	if(m_IsActive)
	{
		if(m_IsRangedWeapon)
		{
			if(m_CurrentProjectileDirection == Direction(North))
			{
				return (m_ProjectileXPosition + (m_ProjectileXBound / 2));
			}

			if(m_CurrentProjectileDirection == Direction(South))
			{
				return (m_ProjectileXPosition - (m_ProjectileXBound / 2));
			}

			if(m_CurrentProjectileDirection == Direction(East))
			{
				return (m_ProjectileXPosition - (m_ProjectileXBound / 2));
			}

			if(m_CurrentProjectileDirection == Direction(West))
			{
				return (m_ProjectileXPosition + (m_ProjectileXBound / 2));
			}
		}

		else
		{
			if(m_LastDrawnDirection == Direction(North))
			{
				return (m_LastDrawnXPosition + (m_XBound / 2));
			}

			if(m_LastDrawnDirection == Direction(South))
			{
				return (m_LastDrawnXPosition - (m_XBound / 2));
			}

			if(m_LastDrawnDirection == Direction(East))
			{
				return (m_LastDrawnXPosition - (m_XBound / 2));
			}

			if(m_LastDrawnDirection == Direction(West))
			{
				return (m_LastDrawnXPosition + (m_XBound / 2));
			}
		}
	}

	else
	{
		return 0;
	}
}

//!Gets and returns the weapon/projectile second hitbox y bound (will always be bottom right in direction)
//Out - 
//		int - y bound of the weapon/projectile hitbox
int Weapon::GetHitBoxYBoundTwo()
{
	if(m_IsActive)
	{
		if(m_IsRangedWeapon)
		{
			if(m_CurrentProjectileDirection == Direction(North))
			{
				return (m_ProjectileYPosition + (m_ProjectileYBound / 2));
			}

			if(m_CurrentProjectileDirection == Direction(South))
			{
				return (m_ProjectileYPosition - (m_ProjectileYBound / 2));
			}

			if(m_CurrentProjectileDirection == Direction(East))
			{
				return (m_ProjectileYPosition + (m_ProjectileYBound / 2));
			}

			if(m_CurrentProjectileDirection == Direction(West))
			{
				return (m_ProjectileYPosition - (m_ProjectileYBound / 2));
			}
		}

		else
		{
			if(m_LastDrawnDirection == Direction(North))
			{
				return (m_LastDrawnYPosition + (m_YBound / 2));
			}

			if(m_LastDrawnDirection == Direction(South))
			{
				return (m_LastDrawnYPosition - (m_YBound / 2));
			}

			if(m_LastDrawnDirection == Direction(East))
			{
				return (m_LastDrawnYPosition + (m_YBound / 2));
			}

			if(m_LastDrawnDirection == Direction(West))
			{
				return (m_LastDrawnYPosition - (m_YBound / 2));
			}
		}
	}

	else
	{
		return 0;
	}
}

//!Gets and returns the damage the weapon deals
//Out - 
//		float - the damage the weapon deals
float Weapon::GetDamage()
{
	if(m_IsActive)
	{
		return (m_Damage * m_DamageModifier);
	}

	else
	{
		return 0;
	}
}

//Sets the damage modifier for the weapon
//In - 
//		float InputDamageModifier - the input damage modifier
void Weapon::SetDamageModifier(float InputDamageModifier)
{
	m_DamageModifier = InputDamageModifier;
}