//

#include "Player.h"

Player::Player(int InputScreenWidth, int InputScreenHeight, ALLEGRO_EVENT_QUEUE* InputEventQueue) : m_EventQueue(InputEventQueue)
{
	m_ScreenWidth = InputScreenWidth;
	m_ScreenHeight = InputScreenHeight;

	al_register_event_source(m_EventQueue, al_get_keyboard_event_source());

	m_PositionX = m_ScreenWidth / 2;
	m_PositionY = m_ScreenHeight / 2;
	m_KeyboardMap["W"] = false;
	m_KeyboardMap["S"] = false;
	m_KeyboardMap["A"] = false;
	m_KeyboardMap["D"] = false;
	m_MovementSpeed = 7;
}

Player::~Player()
{

}

void Player::EventHandler()
{
	al_wait_for_event(m_EventQueue, &m_AlEvent);

	CheckMovement();

	if(m_AlEvent.type = ALLEGRO_EVENT_TIMER)
	{
		DrawPlayer();
	}
}

void Player::DrawPlayer()
{
	al_draw_filled_rectangle(m_PositionX, m_PositionY - 9, m_PositionX + 10, m_PositionY - 7, al_map_rgb(255, 0, 0));
	al_draw_filled_rectangle(m_PositionX, m_PositionY + 9, m_PositionX + 10, m_PositionY + 7, al_map_rgb(255, 0, 0));
	al_draw_filled_triangle(m_PositionX - 12, m_PositionY - 17, m_PositionX + 12, m_PositionY, m_PositionX - 12, m_PositionY + 17, al_map_rgb(0, 255, 0));
	al_draw_filled_rectangle(m_PositionX - 12, m_PositionY - 2, m_PositionX + 15, m_PositionY + 2, al_map_rgb(0, 0, 255));
}

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

	if(m_AlEvent.type == ALLEGRO_EVENT_TIMER)
	{
		if(m_KeyboardMap["W"])
		{
			MoveUp();
		}

		if(m_KeyboardMap["S"])
		{
			MoveDown();
		}

		if(m_KeyboardMap["A"])
		{
			MoveLeft();
		}

		if(m_KeyboardMap["D"])
		{
			MoveRight();
		}
	}

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

	else if(m_AlEvent.type == ALLEGRO_EVENT_KEY_UP)
	{
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

void Player::MoveUp()
{
	m_PositionY -= m_MovementSpeed;

	if(m_PositionY < 0)
	{
		m_PositionY = 0;
	}
}

void Player::MoveDown()
{
	m_PositionY += m_MovementSpeed;

	if(m_PositionY > m_ScreenHeight)
	{
		m_PositionY = m_ScreenHeight;
	}
}

void Player::MoveLeft()
{
	m_PositionX -= m_MovementSpeed;

	if(m_PositionX < 0)
	{
		m_PositionX = 0;
	}
}

void Player::MoveRight()
{
	m_PositionX += m_MovementSpeed;

	if(m_PositionX > m_ScreenWidth)
	{
		m_PositionX = m_ScreenWidth;
	}
}

int Player::GetXPosition()
{
	return m_PositionX;
}

int Player::GetYPosition()
{
	return m_PositionY;
}

