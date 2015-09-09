#ifndef __SPRITE_MANAGER__
#define __SPRITE_MANAGER__

#include <allegro5\allegro.h>

class Sprite
{
private:
	ALLEGRO_BITMAP *m_Image;

	int m_Img_Width;	//Width of the Sprite image to be used
	int m_Img_Height;	//Height of the Sprite image to be used

	int m_Img_LocationOnSpriteSheetX; //X location of where the image is located on a sprite sheet
	int m_Img_LocationOnSpriteSheetY; //Y location of where the image is located on a sprite sheet

	bool m_Animated; //If the image is animated or not


public:
	
};


#endif