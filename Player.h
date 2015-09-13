//

#include <allegro5\allegro.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

#include <string>
#include <map>

//#include "Utility.h"

class Player
{
public:

	Player(int InputScreenWidth, int InputScreenHeight, ALLEGRO_EVENT_QUEUE* InputEventQueue);

	~Player();

	void EventHandler();
	
	int GetXPosition();
	int GetYPosition();

private:

	void DrawPlayer();
	void CheckMovement();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

	ALLEGRO_EVENT_QUEUE *m_EventQueue;
	ALLEGRO_EVENT m_AlEvent;

	int m_ScreenWidth;
	int m_ScreenHeight;

	int m_PositionX;
	int m_PositionY;
	std::map <std::string, bool> m_KeyboardMap;
	int m_MovementSpeed;
};