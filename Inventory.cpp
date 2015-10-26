//Scott Backer

//Inventory Class CPP: A tool for the Player class for maintaining an in game inventory system

#include "Inventory.h"

Inventory::Inventory()
{
	m_LastWeaponSelected = 0;
}

void Inventory::AddWeapon(Weapon* InputWeapon)
{
	m_WeaponInventory.push_back(InputWeapon);
}

Inventory::~Inventory()
{
	m_WeaponInventory.clear();
}

Weapon* Inventory::GetWeaponFromSlot(int InputWeaponSlot)
{
	if(InputWeaponSlot <= m_WeaponInventory.size() && InputWeaponSlot > 0)
	{
		m_LastWeaponSelected = InputWeaponSlot;

		return m_WeaponInventory[(InputWeaponSlot - 1)];
	}
}

Weapon* Inventory::GetNextCycledWeapon()
{
	if((m_LastWeaponSelected + 1) <= m_WeaponInventory.size())
	{
		int TempLastWeaponSelected = m_LastWeaponSelected;
		m_LastWeaponSelected++;
		return m_WeaponInventory[(TempLastWeaponSelected)];
	}

	else if(m_LastWeaponSelected == m_WeaponInventory.size() && m_LastWeaponSelected > 0)
	{
		m_LastWeaponSelected = 1;
		return m_WeaponInventory[0];
	}
}