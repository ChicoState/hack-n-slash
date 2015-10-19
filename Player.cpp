//Scott Backer

//Player Class CPP: Player class will handle all systems having to do with the character that the player controls. 

#include "Player.h"

//!The constructor for the player class
//In - 
//		int InputScreenWidth - the input screen width dimension of the game
//		int INputScreenHeight - the input screen height dimension of the game
//		ALLEGRO_EVENT_QUEUE* InputEventQueue - the overall game event queue input into the player class
Player::Player(ALLEGRO_BITMAP *Image, int InputScreenWidth, int InputScreenHeight, ALLEGRO_EVENT_QUEUE* InputEventQueue) : m_EventQueue(InputEventQueue),
		m_PlayerTile(Image, m_ScreenWidth / 2, m_ScreenHeight / 2, 48, 64, true, true, false, true, 6)
{
	//Set input variables to member variables
	m_ScreenWidth = InputScreenWidth;
	m_ScreenHeight = InputScreenHeight;

	//register the event sources to the event queue
	al_register_event_source(m_EventQueue, al_get_keyboard_event_source());

	//initialize the member variables
	ClassTag = "Player";
	m_XBound = 48;
	m_YBound = 64;
	m_XPosition = m_ScreenWidth / 2;
	m_YPosition = m_ScreenHeight / 2;
	m_CurrentDirection = Direction(North);
	m_KeyboardMap["W"] = false; //W used to move player up
	m_KeyboardMap["S"] = false; //S used to move player down
	m_KeyboardMap["A"] = false; //A used to move player left
	m_KeyboardMap["D"] = false; //D used to move player right
	m_MovementSpeed = 7;
	m_MouseMoving = false;
	m_CurrentMouseMoveXPosition = 0;
	m_CurrentMouseMoveYPosition = 0;
	m_CanMoveUp = true;
	m_CanMoveDown = true;
	m_CanMoveLeft = true;
	m_CanMoveRight = true;
	m_IsColliding = false;
	m_LockedXPosition = 0;
	m_LockedYPosition = 0;

	//Initiate weapons
	SwordWeapon* TempSwordWeapon = new SwordWeapon(m_AlEvent);
	LongSwordWeapon* TempLongSwordWeapon = new LongSwordWeapon(m_AlEvent);
	m_Inventory.AddWeapon(TempSwordWeapon);
	m_Inventory.AddWeapon(TempLongSwordWeapon);
	m_ActiveWeapon = m_Inventory.GetWeaponFromSlot(1);
}

//!The deconstructor for the player class
Player::~Player()
{
	delete m_ActiveWeapon;
}

//!Handles all the functions for the player that need to be called every update
//In - 
//		ALLEGRO_EVENT& InputAlEvent - The global event handler for the game
void Player::EventHandler(ALLEGRO_EVENT& InputAlEvent, float InputMouseXWorldPosition, float InputMouseYWorldPosition)
{
	//make member event the same as the input event
	m_AlEvent = InputAlEvent;

	//check player movement
	CheckMovement(InputMouseXWorldPosition, InputMouseYWorldPosition);

	/*
	//check weapon
	if(m_CurrentDirection == Direction(North))
	{
		//m_Sword.EventHandler(GetXNorthBoundPoint(), GetYNorthBoundPoint(), 0, -1);
		m_ActiveWeapon->EventHandler(GetXNorthBoundPoint(), GetYNorthBoundPoint(), 0, -1);
	}

	if(m_CurrentDirection == Direction(South))
	{
		//m_Sword.EventHandler(GetXSouthBoundPoint(), GetYSouthBoundPoint(), 0, 1);
		m_ActiveWeapon->EventHandler(GetXSouthBoundPoint(), GetYSouthBoundPoint(), 0, 1);
	}

	if(m_CurrentDirection == Direction(East))
	{
		//m_Sword.EventHandler(GetXEastBoundPoint(), GetYEastBoundPoint(), 1, 0);
		m_ActiveWeapon->EventHandler(GetXEastBoundPoint(), GetYEastBoundPoint(), 1, 0);
	}

	if(m_CurrentDirection == Direction(West))
	{
		//m_Sword.EventHandler(GetXWestBoundPoint(), GetYWestBoundPoint(), -1, 0);
		m_ActiveWeapon->EventHandler(GetXWestBoundPoint(), GetYWestBoundPoint(), -1, 0);
	}
	*/
	
	if(m_AlEvent.type = ALLEGRO_EVENT_TIMER)
	{
		m_IsColliding = false;

		/*
		//update sprite
		m_PlayerTile.Event_Handler(m_AlEvent);

		//draw the player sprite
		m_PlayerTile.Draw(m_XPosition, m_YPosition);
		DrawPlayer();
		*/
	}
	
}

//!Draws the player character to the screen
void Player::DrawPlayer()
{
	//update sprite
	m_PlayerTile.Event_Handler(m_AlEvent);

	//draw the player sprite
	m_PlayerTile.Draw((m_XPosition - m_XBound / 2), (m_YPosition - m_YBound / 2));

	//check weapon
	if(m_CurrentDirection == Direction(North))
	{
		//m_Sword.EventHandler(GetXNorthBoundPoint(), GetYNorthBoundPoint(), 0, -1);
		m_ActiveWeapon->EventHandler(GetXNorthBoundPoint(), GetYNorthBoundPoint(), 0, -1);
	}

	if(m_CurrentDirection == Direction(South))
	{
		//m_Sword.EventHandler(GetXSouthBoundPoint(), GetYSouthBoundPoint(), 0, 1);
		m_ActiveWeapon->EventHandler(GetXSouthBoundPoint(), GetYSouthBoundPoint(), 0, 1);
	}

	if(m_CurrentDirection == Direction(East))
	{
		//m_Sword.EventHandler(GetXEastBoundPoint(), GetYEastBoundPoint(), 1, 0);
		m_ActiveWeapon->EventHandler(GetXEastBoundPoint(), GetYEastBoundPoint(), 1, 0);
	}

	if(m_CurrentDirection == Direction(West))
	{
		//m_Sword.EventHandler(GetXWestBoundPoint(), GetYWestBoundPoint(), -1, 0);
		m_ActiveWeapon->EventHandler(GetXWestBoundPoint(), GetYWestBoundPoint(), -1, 0);
	}

	//draw the bound points
	al_draw_pixel(GetXNorthBoundPoint(), GetYNorthBoundPoint(), al_map_rgb(255, 0, 0));
	al_draw_pixel(GetXEastBoundPoint(), GetYEastBoundPoint(), al_map_rgb(255, 255, 255));
	al_draw_pixel(GetXSouthBoundPoint(), GetYSouthBoundPoint(), al_map_rgb(0, 0, 255));
	al_draw_pixel(GetXWestBoundPoint(), GetYWestBoundPoint(), al_map_rgb(0, 255, 0));
}

//!Handles movement for the player character each update
void Player::CheckMovement(float InputMouseXWorldPosition, float InputMouseYWorldPosition)
{
	Weapon* TempReturnedWeapon = NULL; //used when retrieving a new weapon from the inventory

	//reset the keyboard moving bool so that the mouse movement can occur on this frame if called on
	m_KeyboardMoving = false;

	//check to see if the player can move a direction they were previously colliding
	if(!m_IsColliding || m_YPosition != m_LockedYPosition)
	{
		m_CanMoveUp = true;
		m_CanMoveDown = true;
	}

	if(!m_IsColliding || m_XPosition != m_LockedXPosition)
	{
		m_CanMoveLeft = true;
		m_CanMoveRight = true;
	}

	//if a mouse button was pressed
	if(m_AlEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
	{
		//if the left mous ebutton was clicked
		if(m_AlEvent.mouse.button & 1)
		{
			//grab the current mouse x and y position and set mouse moving to true
			//m_CurrentMouseMoveXPosition = m_AlEvent.mouse.x;
			//m_CurrentMouseMoveYPosition = m_AlEvent.mouse.y;
			m_CurrentMouseMoveXPosition = InputMouseXWorldPosition;
			m_CurrentMouseMoveYPosition = InputMouseYWorldPosition;

			m_MouseMoving = true;
		}
	}

	//if the right time call functions
	else if(m_AlEvent.type == ALLEGRO_EVENT_TIMER)
	{
		//Depending on the keyboard move direction pressed call its function

		if(m_KeyboardMap["W"])
		{
			m_KeyboardMoving = true;
			if(m_CanMoveUp)
			{
				m_PlayerTile.Set_CurRow(3, false);

				m_CurrentDirection = Direction(North);
				MoveUp();
			}
		}

		else if(m_KeyboardMap["S"])
		{
			m_KeyboardMoving = true;
			if(m_CanMoveDown)
			{
				m_PlayerTile.Set_CurRow(0, false);

				m_CurrentDirection = Direction(South);
				MoveDown();
			}
		}

		else if(m_KeyboardMap["A"])
		{
			m_KeyboardMoving = true;
			if(m_CanMoveLeft)
			{
				m_PlayerTile.Set_CurRow(1, false);

				m_CurrentDirection = Direction(West);
				MoveLeft();
			}
		}

		else if(m_KeyboardMap["D"])
		{
			m_KeyboardMoving = true;
			if(m_CanMoveRight)
			{
				m_PlayerTile.Set_CurRow(2, false);

				m_CurrentDirection = Direction(East);
				MoveRight();
			}
		}

		else
		{
			if(m_CurrentDirection == Direction(North))
			{
				m_PlayerTile.Set_CurRow(3, true);
			}

			else if(m_CurrentDirection == Direction(South))
			{
				m_PlayerTile.Set_CurRow(0, true);
			}

			else if(m_CurrentDirection == Direction(East))
			{
				m_PlayerTile.Set_CurRow(2, true);
			}

			else if(m_CurrentDirection == Direction(West))
			{
				m_PlayerTile.Set_CurRow(1, true);
			}
		}

		//check to see if there was keyboard movement this frame before calling mouse movement
		if(m_KeyboardMoving)
		{
			m_MouseMoving = false;
		}

		CheckMouseMovement();
	}

	//if there is a key pressed down
	else if(m_AlEvent.type == ALLEGRO_EVENT_KEY_DOWN)
	{	
		//Depending on the keyboard move direction pressed call its function
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

		case ALLEGRO_KEY_SPACE:
			m_ActiveWeapon->Attack();
			break;

		case ALLEGRO_KEY_E:
			TempReturnedWeapon = m_Inventory.GetNextCycledWeapon();
			if(TempReturnedWeapon != NULL)
			{
				m_ActiveWeapon = TempReturnedWeapon;
			}
			break;

		case ALLEGRO_KEY_1:
			TempReturnedWeapon = m_Inventory.GetWeaponFromSlot(1);
			if(TempReturnedWeapon != NULL)
			{
				m_ActiveWeapon = TempReturnedWeapon;
			}
			break;

		case ALLEGRO_KEY_2:
			TempReturnedWeapon = m_Inventory.GetWeaponFromSlot(2);
			if(TempReturnedWeapon != NULL)
			{
				m_ActiveWeapon = TempReturnedWeapon;
			}
			break;

		case ALLEGRO_KEY_3:
			TempReturnedWeapon = m_Inventory.GetWeaponFromSlot(3);
			if(TempReturnedWeapon != NULL)
			{
				m_ActiveWeapon = TempReturnedWeapon;
			}
			break;
		}
	}

	//if there is a key unpressed
	else if(m_AlEvent.type == ALLEGRO_EVENT_KEY_UP)
	{
		//Depending on the keyboard move direction pressed call its function
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

//!Checks each frame to see if the player needs to move from a mouse click and update sthe position
void Player::CheckMouseMovement()
{
	//if there is movement that needs to occur because of a mouse click
	if(m_MouseMoving)
	{
		//if the player position has not reached the correct x position
		if(m_XPosition != m_CurrentMouseMoveXPosition)
		{
			//depending on the direction that player needs to move call its function

			if(m_XPosition > m_CurrentMouseMoveXPosition)
			{
				MoveLeft();
			}

			if(m_XPosition < m_CurrentMouseMoveXPosition)
			{
				MoveRight();
			}
		}

		//if the player position has not reached the correct y position
		if(m_YPosition != m_CurrentMouseMoveYPosition)
		{
			//depending on the direction that player needs to move call its function

			if(m_YPosition > m_CurrentMouseMoveYPosition)
			{
				MoveUp();
			}

			if(m_YPosition < m_CurrentMouseMoveYPosition)
			{
				MoveDown();
			}
		}

		//if the player has reached its destination stop the mouse movement
		else
		{
			m_MouseMoving = false;
		}
	}
}

//!Moves the player negative in the y axis
void Player::MoveUp()
{
	//move the player in its specified direction
	//m_YPosition -= m_MovementSpeed;

	//if the player is about to go off screen lock its position
	if(GetYNorthBoundPoint() < 0)
	{
		m_LockedYPosition = m_YPosition;
		m_YPosition = m_LockedYPosition;
	}

	//else move the player in its specified direction
	else
	{
		m_YPosition -= m_MovementSpeed;
	}
}

//!Moves the player positive in the y axis
void Player::MoveDown()
{
	//move the player in its specified direction
	m_YPosition += m_MovementSpeed;

	/*
	//if the player is about to go off screen lock its position
	if(GetYSouthBoundPoint() > m_ScreenHeight)
	{
		m_LockedYPosition = m_YPosition;
		m_YPosition = m_LockedYPosition;
	}

	//else move the player in its specified direction
	else
	{
		m_YPosition += m_MovementSpeed;
	}
	*/
}

//!Move sthe player negative in the x axis
void Player::MoveLeft()
{
	//move the player in its specified direction
	//m_XPosition -= m_MovementSpeed;

	//if the player is about to go off screen lock its position
	if(GetXWestBoundPoint() < 0)
	{
		m_LockedXPosition = m_XPosition;
		m_XPosition = m_LockedXPosition;
	}

	//else move the player in its specified direction
	else
	{
		m_XPosition -= m_MovementSpeed;
	}
}

//!Moves the player positive in the x axis
void Player::MoveRight()
{
	//move the player in its specified direction
	m_XPosition += m_MovementSpeed;

	/*
	//if the player is about to go off screen lock its position
	if(GetXEastBoundPoint() > m_ScreenWidth)
	{
		m_LockedXPosition = m_XPosition;
		m_XPosition = m_LockedXPosition;
	}

	//else move the player in its specified direction
	else
	{
		m_XPosition += m_MovementSpeed;
	}
	*/
}

//!Tells the player that they have collided with something in thier current moving direction
void Player::MovementColliding()
{
	//check moving direction and stop the current movement direction

	m_IsColliding = true;

	if(m_CurrentDirection == Direction(North))
	{
		m_LockedYPosition = m_YPosition;
		m_CanMoveUp = false;
	}

	if(m_CurrentDirection == Direction(South))
	{
		m_LockedYPosition = m_YPosition;
		m_CanMoveDown = false;
	}

	if(m_CurrentDirection == Direction(East))
	{
		m_LockedXPosition = m_XPosition;
		m_CanMoveRight = false;
	}

	if(m_CurrentDirection == Direction(West))
	{
		m_LockedXPosition = m_XPosition;
		m_CanMoveLeft = false;
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
float Player::GetXPosition()
{
	return m_XPosition;
}

//!Gets and returns the Y position of the player
//Out - 
//		int - the current y position of the player
float Player::GetYPosition()
{
	return m_YPosition;
}

//!Gets and returns the current X bound position of the player relative to the direction the player is moving
//Out - 
//		int - the current x bound of the player relative to their moving direction
float Player::GetCurrentMovingXPosition()
{
	//check moving direction and return the bound for that specific direction

	if(m_CurrentDirection == Direction(North))
	{
		return GetXNorthBoundPoint();
	}

	else if(m_CurrentDirection == Direction(South))
	{
		return GetXSouthBoundPoint();
	}

	else if(m_CurrentDirection == Direction(East))
	{
		return GetXEastBoundPoint();
	}

	else if(m_CurrentDirection == Direction(West))
	{
		return GetXWestBoundPoint();
	}

	else
	{
		return NULL;
	}
}

//!Gets and returns the current Y bound position of the player relative to the direction the player is moving
//Out - 
//		int - the current y bound of the player relative to their moving direction
float Player::GetCurrentMovingYPosition()
{
	//check moving direction and return the bound for that specific direction

	if(m_CurrentDirection == Direction(North))
	{
		return GetYNorthBoundPoint();
	}

	else if(m_CurrentDirection == Direction(South))
	{
		return GetYSouthBoundPoint();
	}

	else if(m_CurrentDirection == Direction(East))
	{
		return GetYEastBoundPoint();
	}

	else if(m_CurrentDirection == Direction(West))
	{
		return GetYWestBoundPoint();
	}

	else
	{
		return NULL;
	}
}

//!Sets the x position of the player
//In - 
//		int NewXPosition - the new x position for the player
void Player::SetXPosition(float NewXPosition)
{
	m_XPosition = NewXPosition;
}

//!Sets the y position of the player
//In - 
//		int NewYPosition - the new y position for the player
void Player::SetYPosition(float NewYPosition)
{
	m_YPosition = NewYPosition;
}

