//Scott Backer

//Weapon Class CPP: Creates the use of a weapon for a player/AI and a parent class for all weapon types.

#include "Weapon.h"

Weapon::Weapon(ALLEGRO_EVENT& InputAlEvent, /*InputWeaponSprite, */int InputXBound, int InputYBound, bool IsRangedWeapon, float InputAttackTime, float InputDamage)
{
	m_AlEvent = InputAlEvent;
	//m_Sprite = InputWeaponSprite;
	m_XBound = InputXBound;
	m_YBound = InputYBound;
	m_IsRangedWeapon = IsRangedWeapon;
	m_IsActive = false;
	m_AttackTime = (InputAttackTime * 60);
	m_CurrentAttackCount = 0;
	m_Damage = InputDamage;
	m_DamageModifier = 1;
}

void Weapon::EventHandler(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection)
{
	if(m_AlEvent.type = ALLEGRO_EVENT_TIMER)
	{
		//draw the weapon sprite
		Draw(DrawXCoordinate, DrawYCoordinate, XDirection, YDirection);

		if(m_IsActive)
		{
			m_CurrentAttackCount++;

			if(m_CurrentAttackCount >= m_AttackTime)
			{
				m_IsActive = false;
				m_CurrentAttackCount = 0;
			}
		}
	}

}

void Weapon::Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection)
{
	
}

void Weapon::Attack()
{
	m_IsActive = true;
	m_CurrentAttackCount = 0;
}

int Weapon::GetXBound()
{
	return m_XBound;
}

int Weapon::GetYBound()
{
	return m_YBound;
}

bool Weapon::IsActive()
{
	return m_IsActive;
}

float Weapon::GetDamage()
{
	return (m_Damage * m_DamageModifier);
}

void Weapon::SetDamageModifier(float InputDamageModifier)
{
	m_DamageModifier = InputDamageModifier;
}