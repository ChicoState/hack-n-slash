//Scott Backer

//Inventory Class H: A tool for the Player class for maintaining an in game inventory system


#include <allegro5\allegro.h>
#include <vector>

#include "Weapon.h"

#ifndef INVENTORY_H
#define INVENTORY_H

class Inventory
{
public:
	Inventory(); //!Constructor for the inventory class
	~Inventory(); //!Deconstructor for the inventory class
	void AddWeapon(Weapon* InputWeapon); //!Adds a weapon to the inventory

	Weapon* GetWeaponFromSlot(int InputWeaponSlot); //!Gets and returns a weapon from a specific slot in the inventory
	Weapon* GetNextCycledWeapon(); //!Gets and returns the next weapon in the inventory after the currently selected weapon

private:
	std::vector <Weapon*> m_WeaponInventory; //vector to store weapons in the inventory
	int m_LastWeaponSelected; //the last selected weapon index of the weapon vector
};

#endif