//Scott Backer

//Sword Weapon Class CPP: Creates a sword weapon for use with a player/AI

#include "SwordWeapon.h"

SwordWeapon::SwordWeapon(ALLEGRO_EVENT& InputAlEvent) : Weapon(InputAlEvent, 16, 16, false, 2, 10)
{

}

void SwordWeapon::Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection)
{
	if(m_IsActive)
	{
		al_draw_line(DrawXCoordinate, DrawYCoordinate, DrawXCoordinate + (30 * XDirection), DrawYCoordinate + (30 * YDirection), al_map_rgb(255, 0, 0), 10);
	}

	else
	{
		al_draw_line(DrawXCoordinate, DrawYCoordinate, DrawXCoordinate + (30 * XDirection), DrawYCoordinate + (30 * YDirection), al_map_rgb(0, 0, 255), 10);
	}
}

