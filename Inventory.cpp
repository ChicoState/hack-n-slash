//Scott Backer

//Inventory Class CPP: A tool for the Player class for maintaining an in game inventory system

#include "Inventory.h"

//!Constructor for the inventory class
Inventory::Inventory()
{
	//initialize member variables
	m_LastWeaponSelected = 0;
}

//!Deconstructor for the inventory class
Inventory::~Inventory()
{
	//clear the inventory vectors
	m_WeaponInventory.clear();
}

//!Adds a weapon to the inventory
//In - 
//		Weapon* InputWeapon - the input weapon to add to the inventory
void Inventory::AddWeapon(Weapon* InputWeapon)
{
	//add the input weapon to tthe inventory
	m_WeaponInventory.push_back(InputWeapon);
}

//!Gets and returns a weapon from a specific slot in the inventory
//In - 
//		int InputWeaponSlot - the weapon slot number to get from the inventory
//Out - 
//		Weapon* - the specific weapon from the slot asked for
Weapon* Inventory::GetWeaponFromSlot(int InputWeaponSlot)
{
	/*
	//if there is only one weapon in the inventory no need to switch
	if(m_WeaponInventory.size() == 1)
	{
		return NULL;
	}
	*/

	//if the input inventory slot exists
	if(InputWeaponSlot <= m_WeaponInventory.size() && InputWeaponSlot > 0)
	{
		//reset the previous weapon asked for
		m_WeaponInventory[m_LastWeaponSelected]->ResetWeapon();

		//keep track of the last weapon asked for
		m_LastWeaponSelected = (InputWeaponSlot - 1);

		//return weapon asked for
		return m_WeaponInventory[(InputWeaponSlot - 1)];
	}

	//else return NULL
	else
	{
		return NULL;
	}
}

//!Gets and returns the next weapon in the inventory after the currently selected weapon
//Out -
//		Weapon* - the weapon in the next inventory slot from the currently selected weapon
Weapon* Inventory::GetNextCycledWeapon()
{
	//if the next weapon is within the inventory indexes
	if((m_LastWeaponSelected + 1) <= (m_WeaponInventory.size() - 1))
	{
		//reset the previous weapon asked for
		m_WeaponInventory[m_LastWeaponSelected]->ResetWeapon();

		m_LastWeaponSelected++;

		return m_WeaponInventory[(m_LastWeaponSelected)];
	}

	//reset to the first index if the next index is greater than the vector size
	else if(m_LastWeaponSelected == (m_WeaponInventory.size() - 1))
	{
		//reset the previous weapon asked for
		m_WeaponInventory[m_LastWeaponSelected]->ResetWeapon();

		//keep track of the currently selected weapon and return the weapon asked for
		m_LastWeaponSelected = 0;
		return m_WeaponInventory[m_LastWeaponSelected];
	}

	else
	{
		return NULL;
	}
}