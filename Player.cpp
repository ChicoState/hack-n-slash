//Scott Backer

//Player Class CPP: Player class will handle all systems having to do with the character that the player controls. 

#include "Player.h"

//!The constructor for the player class
//In - 
//		int InputScreenWidth - the input screen width dimension of the game
//		int INputScreenHeight - the input screen height dimension of the game
//		ALLEGRO_EVENT_QUEUE* InputEventQueue - the overall game event queue input into the player class
Player::Player(int InputScreenWidth, int InputScreenHeight, ALLEGRO_EVENT_QUEUE* InputEventQueue) : m_EventQueue(InputEventQueue)
{
	//Set input variables to member variables
	m_ScreenWidth = InputScreenWidth;
	m_ScreenHeight = InputScreenHeight;

	//register the event sources to the event queue
	al_register_event_source(m_EventQueue, al_get_keyboard_event_source());

	//initialize the member variables
	ClassTag = "Player";
	m_XBound = 50;
	m_YBound = 50;
	m_XPosition = m_ScreenWidth / 2;
	m_YPosition = m_ScreenHeight / 2;
	m_KeyboardMap["W"] = false; //W used to move player up
	m_KeyboardMap["S"] = false; //S used to move player down
	m_KeyboardMap["A"] = false; //A used to move player left
	m_KeyboardMap["D"] = false; //D used to move player right
	m_MovementSpeed = 7;
	m_CanMoveUp = true;
	m_CanMoveDown = true;
	m_CanMoveLeft = true;
	m_CanMoveRight = true;
	m_LockedXPosition = 0;
	m_LockedYPosition = 0;
}

//!The deconstructor for the player class
Player::~Player()
{

}

//!Handles all the functions for the player that need to be called every update
void Player::EventHandler()
{
	//start the wait for event sequence
	al_wait_for_event(m_EventQueue, &m_AlEvent);

	//check player movement
	CheckMovement();

	//draw the player if the right time
	if(m_AlEvent.type = ALLEGRO_EVENT_TIMER)
	{
		DrawPlayer();
	}
}

//!Draws the player character to the screen
void Player::DrawPlayer()
{
	//draw the bound points
	al_draw_pixel(GetXNorthBoundPoint(), GetYNorthBoundPoint(), al_map_rgb(255, 0, 0));
	al_draw_pixel(GetXEastBoundPoint(), GetYEastBoundPoint(), al_map_rgb(255, 255, 255));
	al_draw_pixel(GetXSouthBoundPoint(), GetYSouthBoundPoint(), al_map_rgb(0, 0, 255));
	al_draw_pixel(GetXWestBoundPoint(), GetYWestBoundPoint(), al_map_rgb(0, 255, 0));
	
	//draw the character
	al_draw_filled_rectangle(m_XPosition, m_YPosition - 9, m_XPosition + 10, m_YPosition - 7, al_map_rgb(255, 0, 0));
	al_draw_filled_rectangle(m_XPosition, m_YPosition + 9, m_XPosition + 10, m_YPosition + 7, al_map_rgb(255, 0, 0));
	al_draw_filled_triangle(m_XPosition - 12, m_YPosition - 17, m_XPosition + 12, m_YPosition, m_XPosition - 12, m_YPosition + 17, al_map_rgb(0, 255, 0));
	al_draw_filled_rectangle(m_XPosition - 12, m_YPosition - 2, m_XPosition + 15, m_YPosition + 2, al_map_rgb(0, 0, 255));
}

//!Handles movement for the player character each update
void Player::CheckMovement()
{
	/*
	if(MainUtility.W_KEY_TRUE)
	{

	}

	if(MainUtility.S_KEY_TRUE)
	{

	}

	if(MainUtility.A_KEY_TRUE)
	{

	}

	if(MainUtility.D_KEY_TRUE)
	{

	}
	*/

	//if the right time call functions
	if(m_AlEvent.type == ALLEGRO_EVENT_TIMER)
	{
		if(m_KeyboardMap["W"])
		{
			if(m_CanMoveUp)
			{
				MoveUp();
			}
		}

		if(m_KeyboardMap["S"])
		{
			if(m_CanMoveDown)
			{
				MoveDown();
			}
		}

		if(m_KeyboardMap["A"])
		{
			if(m_CanMoveLeft)
			{
				MoveLeft();
			}
		}

		if(m_KeyboardMap["D"])
		{
			if(m_CanMoveRight)
			{
				MoveRight();
			}
		}
	}

	//if there is a key pressed down
	else if(m_AlEvent.type == ALLEGRO_EVENT_KEY_DOWN)
	{	
		switch(m_AlEvent.keyboard.keycode)
		{
		case ALLEGRO_KEY_W:
			m_KeyboardMap["W"] = true;
			break;

		case ALLEGRO_KEY_S:
			m_KeyboardMap["S"] = true;
			break;

		case ALLEGRO_KEY_A:
			m_KeyboardMap["A"] = true;
			break;

		case ALLEGRO_KEY_D:
			m_KeyboardMap["D"] = true;
			break;
		}
	}

	//if there is a key unpressed
	else if(m_AlEvent.type == ALLEGRO_EVENT_KEY_UP)
	{
		//test
		printf("W Pressed = %i", m_KeyboardMap["W"]);
		
		switch(m_AlEvent.keyboard.keycode)
		{
		case ALLEGRO_KEY_W:
			m_KeyboardMap["W"] = false;
			break;

		case ALLEGRO_KEY_S:
			m_KeyboardMap["S"] = false;
			break;

		case ALLEGRO_KEY_A:
			m_KeyboardMap["A"] = false;
			break;

		case ALLEGRO_KEY_D:
			m_KeyboardMap["D"] = false;
			break;
		}
	}
}

//!Moves the player negative in the y axis
void Player::MoveUp()
{
	if(GetYNorthBoundPoint() < 0)
	{
		m_LockedYPosition = m_YPosition;
		m_YPosition = m_LockedYPosition;
	}

	else
	{
		m_YPosition -= m_MovementSpeed;
	}
}

//!Moves the player positive in the y axis
void Player::MoveDown()
{
	if(GetYSouthBoundPoint() > m_ScreenHeight)
	{
		m_LockedYPosition = m_YPosition;
		m_YPosition = m_LockedYPosition;
	}

	else
	{
		m_YPosition += m_MovementSpeed;
	}
}

//!Move sthe player negative in the x axis
void Player::MoveLeft()
{
	if(GetXWestBoundPoint() < 0)
	{
		m_LockedXPosition = m_XPosition;
		m_XPosition = m_LockedXPosition;
	}

	else
	{
		m_XPosition -= m_MovementSpeed;
	}
}

//!Moves the player positive in the x axis
void Player::MoveRight()
{
	if(GetXEastBoundPoint() > m_ScreenWidth)
	{
		m_LockedXPosition = m_XPosition;
		m_XPosition = m_LockedXPosition;
	}

	else
	{
		m_XPosition += m_MovementSpeed;
	}
}

//!Gets and returns the player class tag
//Out - 
//		string - the player class tag
std::string Player::GetTag()
{
	return ClassTag;
}

//!Gets and returns the player X Bound
//Out - 
//		int - player x bound
int Player::GetXBound()
{
	return m_XBound;
}

//!Gets and returns the player Y Bound
//Out - 
//		int - player y bound
int Player::GetYBound()
{
	return m_YBound;
}

//!Gets and returns the North Bound Point x position
//Out - 
//		int - North Bound Point x position
int Player::GetXNorthBoundPoint()
{
	return m_XPosition;
}

//!Gets and returns the North Bound Point y position
//Out - 
//		int - North Bound Point y position
int Player::GetYNorthBoundPoint()
{
	return (m_YPosition - (m_YBound / 2));
}

//!Gets and returns the South Bound Point x position
//Out - 
//		int - South Bound Point x position
int Player::GetXSouthBoundPoint()
{
	return m_XPosition;
}

//!Gets and returns the South Bound Point y position
//Out - 
//		int - South Bound Point y position
int Player::GetYSouthBoundPoint()
{
	return (m_YPosition + (m_YBound / 2));
}

//!Gets and returns the East Bound Point x position
//Out - 
//		int - East Bound Point x position
int Player::GetXEastBoundPoint()
{
	return (m_XPosition + (m_XBound / 2));
}

//!Gets and returns the East Bound Point y position
//Out - 
//		int - East Bound Point y position
int Player::GetYEastBoundPoint()
{
	return m_YPosition;
}

//!Gets and returns the West Bound Point x position
//Out - 
//		int - West Bound Point x position
int Player::GetXWestBoundPoint()
{
	return (m_XPosition - (m_XBound / 2));
}

//!Gets and returns the West Bound Point y position
//Out - 
//		int - West Bound Point y position
int Player::GetYWestBoundPoint()
{
	return m_YPosition;
}

//!Gets and returns the X position of the player
//Out - 
//		int - the current x position of the player
int Player::GetXPosition()
{
	return m_XPosition;
}

//!Gets and returns the Y position of the player
//Out - 
//		int - the current y position of the player
int Player::GetYPosition()
{
	return m_YPosition;
}

//!Sets the x position of the player
//In - 
//		int NewXPosition - the new x position for the player
void Player::SetXPosition(int NewXPosition)
{
	m_XPosition = NewXPosition;
}

//!Sets the y position of the player
//In - 
//		int NewYPosition - the new y position for the player
void Player::SetYPosition(int NewYPosition)
{
	m_YPosition = NewYPosition;
}

