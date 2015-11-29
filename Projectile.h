//Scott Backer

//Projectile Class H: Projectile class is a projectile that can be used by other classes to create a projectile attack in game.

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

<<<<<<< HEAD
#include "ProjectileTile.h"
=======
#include <iostream>
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31

#ifndef PROJECTILE_H
#define PROJECTILE_H

class Projectile
{
public:
	Projectile(int XBound, int YBound, int ProjectileSpeed, int StartingXPosition, int StartingYPosition, int XDirection, int YDirection); //!Constructor for the Projectile class
	void Draw(); //!Draws the projectile
	void SendProjecile(int StartingXPosition, int StartingYPosition, int XDirection, int YDirection); //Re-sends the projectile in the attacking direction
	void UpdatePosition(); //!Update sthe position of the projectile
	void ResetProjectile(); //!Resets the projectile to resting position (destroys it technically)

	bool IsAtRestingPosition(); 
	int GetHitBoxXBoundOne();//!Gets and returns the x bound one of the projectile
	int GetHitBoxYBoundOne(); //!Gets and returns the y bound one of the projectile
	int GetHitBoxXBoundTwo(); //!Gets and returns the x bound two of the projectile
	int GetHitBoxYBoundTwo(); //!Gets and returns the y bound two of the projectile

private:
<<<<<<< HEAD
	
	ProjectileTile m_ProjectileSprite; //sprite for the projectile

=======
>>>>>>> b717b7fd3314e05f4e93e3e059cd35a8412fdc31
	int m_XBound; //x bound of the projectile
	int m_YBound; //y bound of the projectile
	int m_ProjectileSpeed; //the projectile speed
	int m_CurrentXPosition; //the current x position of the projectile
	int m_CurrentYPosition; //the current y position of the projectile
	int m_CurrentXDirection; //the current x direction of the projectile
	int m_CurrentYDirection; //the current y direction of the projectile

	int m_RestingXPosition; //the resting x position for the projectile when it is destroyed
	int m_RestingYPosition; //the resting x position for the projectile when it is destroyed
};

#endif