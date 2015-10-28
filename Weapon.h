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
	Weapon(ALLEGRO_EVENT& InputAlEvent, int InputXBound, int InputYBound, bool IsRangedWeapon, float InputAttackTime, float InputDamage); //!Constructor for the weapon class
	virtual void EventHandler(); //!Handles allegro events for the weapon class
	virtual void Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection); //Draws the weapon sprite to the screen
	virtual void Attack(); //!Attacks with the weapon to make it active
	void ResetWeapon(); //!Resets the weapon to a non active state

	bool IsActive(); //!Returns whether or not the weapon is active
	int GetXBound(); //!Gets and returns the weapon x bound
	int GetYBound(); //!Gets and returns the weapon y bound
	float GetDamage(); //!Gets and returns the damage the weapon deals
	void SetDamageModifier(float InputDamageModifier); //Sets the damage modifier for the weapon

protected:
	ALLEGRO_EVENT m_AlEvent; //the event variable for the weapon class

	int m_XBound; //the x bound of the weapon
	int m_YBound; //the y bound of the weapon
	bool m_IsRangedWeapon; //true if the weapon is a ranged weapon else false

	bool m_IsActive; //true if the weapon is active esle false
	float m_AttackTime; //the attack time of the weapon how long it stays active in seconds
	float m_CurrentAttackCount; //the attack timer for when a weapon is active
	float m_Damage; //the damage a weapon does
	float m_DamageModifier; //the current damage modifier of the weapon

private:

};

#endif