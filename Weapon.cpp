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
	m_DamageModifier = 0;
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
	//al_draw_filled_rectangle(DrawXCoordinate, DrawYCoordinate - 9, DrawXCoordinate + 10, DrawYCoordinate - 7, al_map_rgb(255, 0, 0));
	//al_draw_filled_rectangle(DrawXCoordinate, DrawYCoordinate + 9, DrawXCoordinate + 10, DrawYCoordinate + 7, al_map_rgb(255, 0, 0));
	//al_draw_filled_triangle(DrawXCoordinate - 12, DrawYCoordinate - 17, DrawXCoordinate + 12, DrawYCoordinate, DrawXCoordinate - 12, DrawYCoordinate + 17, al_map_rgb(0, 255, 0));
	//al_draw_filled_rectangle(DrawXCoordinate - 12, DrawYCoordinate - 2, DrawXCoordinate + 15, DrawYCoordinate + 2, al_map_rgb(0, 0, 255));

	if(m_IsActive)
	{
		al_draw_line(DrawXCoordinate, DrawYCoordinate, DrawXCoordinate + (30 * XDirection), DrawYCoordinate + (5 * YDirection), al_map_rgb(255, 0, 0), 10);
	}

	else
	{
		al_draw_line(DrawXCoordinate, DrawYCoordinate, DrawXCoordinate + (30 * XDirection), DrawYCoordinate + (5 * YDirection), al_map_rgb(0, 0, 255), 10);
	}
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