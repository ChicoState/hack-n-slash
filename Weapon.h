//Scott Backer

//Weapon Class H: Creates the use of a weapon for a player/AI and a parent class for all weapon types.

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

#ifndef WEAPON_H
#define WEAPON_H

#include "Sprite.h"

class Weapon
{
public:
	Weapon(ALLEGRO_EVENT& InputAlEvent, /*InputWeaponSprite, */int InputXBound, int InputYBound, bool IsRangedWeapon, float InputAttackTime, float InputDamage);
	void EventHandler(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection);
	virtual void Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection);
	virtual void Attack();
	int GetXBound();
	int GetYBound();
	bool IsActive();
	float GetDamage();
	void SetDamageModifier(float InputDamageModifier);

protected:
	ALLEGRO_EVENT m_AlEvent; //the event variable for the weapon class

	//Sprite m_Sprite;
	int m_XBound;
	int m_YBound;
	bool m_IsRangedWeapon;

	bool m_IsActive;
	float m_AttackTime;
	float m_CurrentAttackCount;
	float m_Damage;
	float m_DamageModifier;

private:

};

#endif