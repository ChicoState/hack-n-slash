//Scott Backer

//LongSword Weapon Class CPP: Creates a long sword weapon for use with a player/AI

#include "LongSwordWeapon.h"

LongSwordWeapon::LongSwordWeapon(ALLEGRO_EVENT& InputAlEvent) : Weapon(InputAlEvent, 16, 16, false, 1, 10)
{

}

void LongSwordWeapon::Draw(int DrawXCoordinate, int DrawYCoordinate, int XDirection, int YDirection)
{
	if(m_IsActive)
	{
		al_draw_line(DrawXCoordinate, DrawYCoordinate, DrawXCoordinate + (200 * XDirection), DrawYCoordinate + (200 * YDirection), al_map_rgb(255, 0, 0), 10);
	}

	else
	{
		al_draw_line(DrawXCoordinate, DrawYCoordinate, DrawXCoordinate + (200 * XDirection), DrawYCoordinate + (200 * YDirection), al_map_rgb(0, 0, 255), 10);
	}
}
