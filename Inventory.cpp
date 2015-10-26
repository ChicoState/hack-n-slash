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
	//if the input inventory slot exists
	if(InputWeaponSlot <= m_WeaponInventory.size() && InputWeaponSlot > 0)
	{
		//keep track of the last weapon asked for
		m_LastWeaponSelected = InputWeaponSlot;

		//return weapon asked for
		return m_WeaponInventory[(InputWeaponSlot - 1)];
	}
}

//!Gets and returns the next weapon in the inventory after the currently selected weapon
//Out -
//		Weapon* - the weapon in the next inventory slot from the currently selected weapon
Weapon* Inventory::GetNextCycledWeapon()
{
	//if the next weapon is within the inventory indexes
	if((m_LastWeaponSelected + 1) <= m_WeaponInventory.size())
	{
		//keep track of the currently selected weapon and return the weapon asked for
		int TempLastWeaponSelected = m_LastWeaponSelected;
		m_LastWeaponSelected++;
		return m_WeaponInventory[(TempLastWeaponSelected)];
	}

	//reset to the first index if the next index is greater than the vector size
	else if(m_LastWeaponSelected == m_WeaponInventory.size() && m_LastWeaponSelected > 0)
	{
		//keep track of the currently selected weapon and return the weapon asked for
		m_LastWeaponSelected = 1;
		return m_WeaponInventory[0];
	}
}