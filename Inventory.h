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
	Inventory();
	~Inventory();
	void AddWeapon(Weapon* InputWeapon);

	Weapon* GetWeaponFromSlot(int InputWeaponSlot);
	Weapon* GetNextCycledWeapon();

private:
	std::vector <Weapon*> m_WeaponInventory;
	int m_LastWeaponSelected;
};

#endif