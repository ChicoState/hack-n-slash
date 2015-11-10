//Scott Backer

//Weapon Class H: Creates the use of a weapon for a player/AI and a parent class for all weapon types.

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

#ifndef WEAPON_H
#define WEAPON_H

#include "Sprite.h"
#include "Projectile.h"
#include "Utility.h"

class Weapon
{
public:
	Weapon(ALLEGRO_EVENT_QUEUE* InputEventQueue, ALLEGRO_EVENT& InputAlEvent, int InputXBound, int InputYBound, bool IsRangedWeapon, float InputAttackTime, float InputDamage); //!Constructor for the weapon class
	~Weapon(); //Destructor for the weapon class
	virtual void EventHandler(); //!Handles allegro events for the weapon class
	virtual void Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection); //Draws the weapon sprite to the screen
	virtual void Attack(); //!Attacks with the weapon to make it active
	void ResetWeapon(); //!Resets the weapon to a non active state

	bool IsActive(); //!Returns whether or not the weapon is active
	int GetXBound(); //!Gets and returns the weapon x bound
	int GetYBound(); //!Gets and returns the weapon y bound
	int GetHitBoxXBoundOne();//!Gets and returns the weapon/projectile first hitbox x bound (will always be top left in direction)
	int GetHitBoxYBoundOne();//!Gets and returns the weapon/projectile first hitbox y bound (will always be top left in direction)
	int GetHitBoxXBoundTwo();//!Gets and returns the weapon/projectile second hitbox x bound (will always be bottom right in direction)
	int GetHitBoxYBoundTwo();//!Gets and returns the weapon/projectile second hitbox y bound (will always be bottom right in direction)
	Projectile* GetProjectile(); //!Gets and returns the current projectile of the weapon
	float GetDamage(); //!Gets and returns the damage the weapon deals
	void SetDamageModifier(float InputDamageModifier); //Sets the damage modifier for the weapon

protected:

	//Weapon class enum for direction and which way the weapon/projectile is moving and facing
	enum Direction
	{
		North,
		South,
		East,
		West
	};

	ALLEGRO_EVENT_QUEUE *m_EventQueue; //The event queue for the player class
	ALLEGRO_EVENT m_AlEvent; //the event variable for the weapon class

	int m_XBound; //the x bound of the weapon
	int m_YBound; //the y bound of the weapon
	bool m_IsRangedWeapon; //true if the weapon is a ranged weapon else false
	float m_AttackTime; //the attack time of the weapon how long it stays active in seconds
	float m_Damage; //the damage a weapon does

	Projectile* m_Projectile; //vector of projectiles spawned by a ranged weapon
	int m_ProjectileXBound; //the x bound for the projectile of the weapon
	int m_ProjectileYBound; //the y bound for the projectile of the weapon
	int m_ProjectileSpeed; //speed at which the projectile moves

	int m_LastDrawnXPosition; //The last drawn x position of the weapon
	int m_LastDrawnYPosition; //The last drawn y position of the weapon
	Direction m_LastDrawnDirection; //The last direction the weapon was drawn
	bool m_IsActive; //true if the weapon is active esle false
	float m_CurrentAttackCount; //the attack timer for when a weapon is active
	float m_DamageModifier; //the current damage modifier of the weapon

private:

};

#endif