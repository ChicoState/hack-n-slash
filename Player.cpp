//Scott Backer

//Player Class CPP: Player class will handle all systems having to do with the character that the player controls. 

#include "Player.h"

//!The constructor for the player class
//In - 
//		ALLEGRO_EVENT_QUEUE* InputEventQueue - the overall game event queue input into the player class
Player::Player(ALLEGRO_EVENT_QUEUE* InputEventQueue)
	: m_EventQueue(InputEventQueue),
		m_PlayerTile(0, 0, 88, 88, true, true, false, true, 6)
{
	//register the event sources to the event queue
	al_register_event_source(m_EventQueue, al_get_keyboard_event_source());
	al_init_user_event_source(&m_PositionEventSource);
	al_register_event_source(m_EventQueue, &m_PositionEventSource);

	//load fonts for player
	font28 = al_load_font("GOUDOS.TTF", 28, 0);
	font16 = al_load_font("GOUDOS.TTF", 16, 0);

	//initialize the member variables
	m_CurrentGameScale = 1;
	m_ExperienceMultiplier = 35;
	m_BaseMaxHealth = 100;
	m_MaxHealthIncrement = 20;
	m_SpeedPowerUpSpeed = 12;
	m_StrengthPowerupMultiplier = 2;
	m_BaseMovementSpeed = 7;
	m_PowerupTimerLength = 2400;

	ClassTag = "Player";
	m_XBound = 88;
	m_YBound = 88;
	m_SpriteBoundOffset = 22;
	m_XPosition = 0;
	m_YPosition = 0;
	m_CurrentDirection = Direction(North);
	m_PreviousLockedDirection = Direction(North);
	m_MaxHealth = m_BaseMaxHealth;
	m_CurrentHealth = m_MaxHealth;
	m_Level = 1;
	m_Experience = 0;
	m_KeyboardMap["W"] = false; //W used to move player up
	m_KeyboardMap["S"] = false; //S used to move player down
	m_KeyboardMap["A"] = false; //A used to move player left
	m_KeyboardMap["D"] = false; //D used to move player right
	m_MovementSpeed = m_BaseMovementSpeed;
	m_MouseMoving = false;
	m_CurrentMouseMoveXPosition = 0;
	m_CurrentMouseMoveYPosition = 0;
	m_CanMoveUp = true;
	m_CanMoveDown = true;
	m_CanMoveLeft = true;
	m_CanMoveRight = true;
	m_IsColliding = false;
	m_IsCollidingBoundOne = false;
	m_IsCollidingBoundTwo = false;
	m_LockedXPosition = 0;
	m_LockedYPosition = 0;
	m_CanAttack = true;
	m_IsDead = false;
	m_NewRangedWeaponActivation = false;

	m_SpeedPowerUp = false;
	m_SpeedPowerUpTimer = 0;
	m_StrengthPowerUp = false;
	m_StrengthPowerUpTimer = 0;

	m_DrawExperienceUp = false;
	m_DrawExperienceUpTimer = 0;
	m_DrawLevelUp = false;
	m_DrawLevelUpTimer = 0;

	//Initiate weapons
	SwordWeapon* TempSwordWeapon = new SwordWeapon(m_EventQueue, m_AlEvent);
	BowWeapon* TempBowWeapon = new BowWeapon(m_EventQueue, m_AlEvent);
	m_Inventory.AddWeapon(TempSwordWeapon);
	m_Inventory.AddWeapon(TempBowWeapon);
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
//		float InputMouseXWorldPosition - the translated mouse x position into world space
//		float InputMouseYWorldPosition - the translated mouse y position into world space
void Player::EventHandler(ALLEGRO_EVENT& InputAlEvent, float InputMouseXWorldPosition, float InputMouseYWorldPosition)
{
	//make member event the same as the input event
	m_AlEvent = InputAlEvent;

	if(m_AlEvent.type == ALLEGRO_EVENT_TIMER)
	{
		//check active weapon
		m_ActiveWeapon->EventHandler();
	}

	//Check custom events

	if(m_AlEvent.type == AI_KILLED_EVENT)
	{
		GiveExperience();
		m_ScoreCalculator.AddEnemyKilledScore();
	}

	if(m_AlEvent.type == BOSS_KILLED_EVENT)
	{
		GiveExperience();
		GiveExperience();
		GiveExperience();
		GiveExperience();
		GiveExperience();
		m_ScoreCalculator.AddBossKilledScore();
	}

	//check for pickup event

	if (InputAlEvent.type == PICKUP_EVENT)
	{
		//get pickup event type
		PICKUP_TYPES Type = (PICKUP_TYPES)InputAlEvent.user.data1;

		//check for pickup event type

		if (Type == FOOD)
		{
			HealPlayer();
		}

		if (Type == SPEED)
		{
			m_SpeedPowerUp = true;
			m_SpeedPowerUpTimer = 0;
			m_MovementSpeed = m_SpeedPowerUpSpeed;
		}

		if (Type == STR)
		{
			m_StrengthPowerUp = true;
			m_StrengthPowerUpTimer = 0;
		}
	}
	//check timers for power ups

	if(m_SpeedPowerUp)
	{
		m_SpeedPowerUpTimer++;

		if(m_SpeedPowerUpTimer >= m_PowerupTimerLength)
		{
			m_SpeedPowerUp = false;
			m_SpeedPowerUpTimer = 0;
			m_MovementSpeed = m_BaseMovementSpeed;
		}
	}

	if(m_StrengthPowerUp)
	{
		m_StrengthPowerUpTimer++;

		if(m_StrengthPowerUpTimer >= m_PowerupTimerLength)
		{
			m_StrengthPowerUp = false;
			m_StrengthPowerUpTimer = 0;
		}
	}

	//check player movement
	CheckMovement(InputMouseXWorldPosition, InputMouseYWorldPosition);
	m_IsCollidingBoundOne = false;
	m_IsCollidingBoundTwo = false;
}

//!Draws the player character to the screen
void Player::DrawPlayer()
{
	//draw the player sprite and change depending on weapon
	if(m_ActiveWeapon->IsRangedWeapon())
	{
		//if its a new ranged weapon activation
		if(m_NewRangedWeaponActivation)
		{
			m_PlayerTile.Draw((m_XPosition - m_XBound / 2), (m_YPosition - m_YBound / 2), false, m_ActiveWeapon->IsActive(), true);
			m_NewRangedWeaponActivation = false;
		}

		else
		{
			m_PlayerTile.Draw((m_XPosition - m_XBound / 2), (m_YPosition - m_YBound / 2), false, m_ActiveWeapon->IsActive(), false);
		}
	}

	else
	{
		m_PlayerTile.Draw((m_XPosition - m_XBound / 2), (m_YPosition - m_YBound / 2), m_ActiveWeapon->IsActive(), false, false);
	}

	//check weapon
	if(m_CurrentDirection == Direction(North))
	{
		m_ActiveWeapon->Draw(GetXNorthBoundPoint(), GetYNorthBoundPoint(), 0, -1);
	}

	if(m_CurrentDirection == Direction(South))
	{
		m_ActiveWeapon->Draw(GetXSouthBoundPoint(), GetYSouthBoundPoint(), 0, 1);
	}

	if(m_CurrentDirection == Direction(East))
	{
		m_ActiveWeapon->Draw(GetXEastBoundPoint(), GetYEastBoundPoint(), 1, 0);
	}

	if(m_CurrentDirection == Direction(West))
	{
		m_ActiveWeapon->Draw(GetXWestBoundPoint(), GetYWestBoundPoint(), -1, 0);
	}

	//draw experience up
	if(m_DrawExperienceUp)
	{
		m_DrawExperienceUpTimer++;

		if(m_DrawExperienceUpTimer < 45)
		{
			std::string ExperienceNumber = std::to_string((m_ExperienceMultiplier * m_CurrentGameScale));
			std::string FullExperienceText = "XP+ ";
			FullExperienceText.append(ExperienceNumber);
			char const *ExperienceChar = FullExperienceText.c_str();
			al_draw_text(font16, al_map_rgb(241, 92, 34), m_XPosition, m_YPosition - 50 - (m_DrawExperienceUpTimer), ALLEGRO_ALIGN_CENTER, ExperienceChar);
		}

		else
		{
			m_DrawExperienceUp = false;
			m_DrawExperienceUpTimer = 0;
		}
	}

	//draw level up
	if(m_DrawLevelUp)
	{
		m_DrawLevelUpTimer++;

		if(m_DrawLevelUpTimer < 45)
		{
			std::string FullLevelUpText = "LEVEL UP";
			char const *LevelUpChar = FullLevelUpText.c_str();
			al_draw_text(font28, al_map_rgb(0, 86, 46), m_XPosition, m_YPosition - 50 - (m_DrawLevelUpTimer), ALLEGRO_ALIGN_CENTER, LevelUpChar);
		}

		else
		{
			m_DrawLevelUp = false;
			m_DrawLevelUpTimer = 0;
		}
	}

	//draw strangth powerup color change
	if(m_StrengthPowerUp)
	{
		al_draw_filled_circle(m_XPosition, m_YPosition, 40, al_map_rgba(50, 50, 0, 30));
	}


	/*
	//draw the hit bound points
	al_draw_pixel(GetXNorthBoundPoint(), GetYNorthBoundPoint(), al_map_rgb(255, 0, 0));
	al_draw_pixel(GetXEastBoundPoint(), GetYEastBoundPoint(), al_map_rgb(255, 255, 255));
	al_draw_pixel(GetXSouthBoundPoint(), GetYSouthBoundPoint(), al_map_rgb(0, 0, 255));
	al_draw_pixel(GetXWestBoundPoint(), GetYWestBoundPoint(), al_map_rgb(0, 255, 0));

	al_draw_pixel(GetNorthEastXBoundPoint(), GetNorthEastYBoundPoint(), al_map_rgb(255, 0, 0));
	al_draw_pixel(GetSouthEastXBoundPoint(), GetSouthEastYBoundPoint(), al_map_rgb(255, 255, 255));
	al_draw_pixel(GetSouthWestXBoundPoint(), GetSouthWestYBoundPoint(), al_map_rgb(0, 0, 255));
	al_draw_pixel(GetNorthWestXBoundPoint(), GetNorthWestYBoundPoint(), al_map_rgb(0, 255, 0));
	*/

	//draw player hit box
	//al_draw_rectangle(GetHitBoxXBoundOne(), GetHitBoxYBoundOne(), GetHitBoxXBoundTwo(), GetHitBoxYBoundTwo(), al_map_rgb(255, 250, 0), 1);
	

	//draw the weapon hit box
	//al_draw_rectangle(GetWeaponHitBoxXBoundOne(), GetWeaponHitBoxYBoundOne(), GetWeaponHitBoxXBoundTwo(), GetWeaponHitBoxYBoundTwo(), al_map_rgb(0, 0, 0), 10);
}

//!Handles movement for the player character each update
//In - 
//		float InputMouseXWorldPosition - the converted mouse x position into world position
//		float InputMouseYWorldPosition - the converted mouse y position into world position
void Player::CheckMovement(float InputMouseXWorldPosition, float InputMouseYWorldPosition)
{
	Weapon* TempReturnedWeapon = NULL; //used when retrieving a new weapon from the inventory

	//reset the keyboard moving bool so that the mouse movement can occur on this frame if called on
	m_KeyboardMoving = false;

	//reset player movement
	m_CanMoveUp = true;
	m_CanMoveDown = true;
	m_CanMoveLeft = true;
	m_CanMoveRight = true;

	
	//check and update player collision
	CheckCollision();

	/*
	//Mouse Movement
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

			//set mouse moving to true
			m_MouseMoving = true;
		}
	}
	*/

	//if the right time call functions
	if(m_AlEvent.type == ALLEGRO_EVENT_TIMER)
	{
		//Depending on the keyboard move direction pressed call its function

		if(m_KeyboardMap["A"])
		{
			if(m_CanMoveLeft)
			{
				//set moevement is occuring and set moevement direction
				m_KeyboardMoving = true;
				m_CurrentDirection = Direction(West);
				m_PlayerTile.Set_CurRow(3, false);
				MoveLeft();
			}
		}

		else if(m_KeyboardMap["D"])
		{
			if(m_CanMoveRight)
			{
				//set moevement is occuring and set moevement direction
				m_KeyboardMoving = true;
				m_CurrentDirection = Direction(East);
				m_PlayerTile.Set_CurRow(2, false);
				MoveRight();
			}
		}

		else if(m_KeyboardMap["W"])
		{
			if(m_CanMoveUp)
			{
				//set moevement is occuring and set moevement direction
				m_KeyboardMoving = true;
				m_CurrentDirection = Direction(North);
				m_PlayerTile.Set_CurRow(1, false);
				MoveUp();
			}
		}

		else if(m_KeyboardMap["S"])
		{
			if(m_CanMoveDown)
			{
				//set moevement is occuring and set moevement direction
				m_KeyboardMoving = true;
				m_CurrentDirection = Direction(South);
				m_PlayerTile.Set_CurRow(0, false);
				MoveDown();
			}
		}

		else
		{
			m_KeyboardMoving = false;
		}

		//if there is no movement 
		if(!m_KeyboardMoving && !m_ActiveWeapon->IsActive())
		{
			if(m_CurrentDirection == Direction(North))
			{
				m_PlayerTile.Set_CurRow(1, true);
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
				m_PlayerTile.Set_CurRow(3, true);
			}
		}

		/*
		//check to see if there was keyboard movement this frame before calling mouse movement
		if(m_KeyboardMoving)
		{
			m_MouseMoving = false;
		}
		CheckMouseMovement();
		*/
	}

	//if there is a key pressed down
	if(m_AlEvent.type == ALLEGRO_EVENT_KEY_DOWN)
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

			//Attack with current weapon
		case ALLEGRO_KEY_SPACE:
			if(m_CanAttack)
			{
				m_ActiveWeapon->Attack();
				m_CanAttack = false;

				if(m_ActiveWeapon->IsRangedWeapon())
				{
					m_NewRangedWeaponActivation = true;
				}
			}
			break;

			//cycle through weapon inventory
		case ALLEGRO_KEY_E:
			TempReturnedWeapon = m_Inventory.GetNextCycledWeapon();
			if(TempReturnedWeapon != NULL)
			{
				m_ActiveWeapon = TempReturnedWeapon;
			}
			break;

			//get inventory slot 1 weapon
		case ALLEGRO_KEY_1:
			TempReturnedWeapon = m_Inventory.GetWeaponFromSlot(1);
			if(TempReturnedWeapon != NULL)
			{
				m_ActiveWeapon = TempReturnedWeapon;
			}
			break;

			//get inventory slot 2 weapon
		case ALLEGRO_KEY_2:
			TempReturnedWeapon = m_Inventory.GetWeaponFromSlot(2);
			if(TempReturnedWeapon != NULL)
			{
				m_ActiveWeapon = TempReturnedWeapon;
			}
			break;
		}
	}

	//if there is a key unpressed
	if(m_AlEvent.type == ALLEGRO_EVENT_KEY_UP)
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

		case ALLEGRO_KEY_SPACE:
				m_CanAttack = true;
			break;
		}
	}

	//if the player is moving update the sprite
	if(m_KeyboardMoving || m_MouseMoving)
	{
		//update sprite
		m_PlayerTile.Event_Handler();
		
		//emit the event source of the player position
		m_AlEvent.user.type = CUSTOM_EVENT_ID(PLAYERPOSITION_EVENT);
		m_AlEvent.user.data1 =  (intptr_t)m_XPosition;
		m_AlEvent.user.data2 =  (intptr_t)m_YPosition;
		al_emit_user_event(&m_PositionEventSource, &m_AlEvent, NULL);
	}

	//if the weapon is active continue updating sprite
	else if(m_ActiveWeapon->IsActive())
	{
		//update sprite
		m_PlayerTile.Event_Handler();
	}
}

//!Checks and updates the collision of the player to allow movement directions
void Player::CheckCollision()
{
	//if the player has not changed direction keep the same locked movement
	if(m_CurrentDirection == m_PreviousLockedDirection)
	{
		if(m_CurrentDirection == Direction(North))
		{
			m_CanMoveUp = false;
		}

		if(m_CurrentDirection == Direction(South))
		{
			m_CanMoveDown = false;
		}

		if(m_CurrentDirection == Direction(East))
		{
			m_CanMoveRight = false;
		}

		if(m_CurrentDirection == Direction(West))
		{
			m_CanMoveLeft = false;
		}
	}

	//if both collision points are colliding
	else if(m_IsCollidingBoundOne && m_IsCollidingBoundTwo)
	{
		//set previous direction moved
		m_PreviousLockedDirection = m_CurrentDirection;

		//push player back to their previous position
		m_XPosition	= m_PreviousXPosition;
		m_YPosition	= m_PreviousYPosition;

		//lock movement direction depending on hit bounds colliding

		if(m_CurrentDirection == Direction(North))
		{
			m_CanMoveUp = false;
		}

		if(m_CurrentDirection == Direction(South))
		{
			m_CanMoveDown = false;
		}

		if(m_CurrentDirection == Direction(East))
		{
			m_CanMoveRight = false;
		}

		if(m_CurrentDirection == Direction(West))
		{
			m_CanMoveLeft = false;
		}
	}

	//if the first collision point is colliding
	else if(m_IsCollidingBoundOne && !m_IsCollidingBoundTwo)
	{		
		//set previous direction moved
		m_PreviousLockedDirection = m_CurrentDirection;

		//lock movement direction depending on hit bounds colliding

		if(m_CurrentDirection == Direction(North) && m_KeyboardMap["D"] == true)
		{
			m_XPosition	= m_PreviousXPosition;
			m_CanMoveRight = false;
			m_CanMoveUp = false;
		}

		else if(m_CurrentDirection == Direction(North))
		{
			m_XPosition	= m_PreviousXPosition;
			m_CanMoveRight = false;
			m_CanMoveUp = false;
		}

		if(m_CurrentDirection == Direction(South) && m_KeyboardMap["D"] == true)
		{
			m_XPosition	= m_PreviousXPosition;
			m_CanMoveRight = false;
			m_CanMoveUp = false;
		}

		else if(m_CurrentDirection == Direction(South))
		{
			m_XPosition	= m_PreviousXPosition;
			m_CanMoveRight = false;
			m_CanMoveDown = false;
		}

		if(m_CurrentDirection == Direction(East))
		{
			m_YPosition	= m_PreviousYPosition;
			m_CanMoveUp = false;
			m_CanMoveRight = false;
		}

		if(m_CurrentDirection == Direction(West))
		{
			m_YPosition	= m_PreviousYPosition;
			m_CanMoveUp = false;
			m_CanMoveLeft = false;
		}
	}

	//if the second collision point is colliding
	else if(!m_IsCollidingBoundOne && m_IsCollidingBoundTwo)
	{
		//set previous direction moved
		m_PreviousLockedDirection = m_CurrentDirection;

		//lock movement direction depending on hit bounds colliding

		if(m_CurrentDirection == Direction(North) && m_KeyboardMap["A"] == true)
		{
			m_XPosition	= m_PreviousXPosition;
			m_CanMoveLeft = false;
		}

		else if(m_CurrentDirection == Direction(North))
		{
			m_XPosition	= m_PreviousXPosition;
			m_CanMoveLeft = false;
			m_CanMoveUp = false;
		}

		if(m_CurrentDirection == Direction(South) && m_KeyboardMap["A"] == true)
		{
			m_XPosition	= m_PreviousXPosition;
			m_CanMoveLeft = false;
		}

		else if(m_CurrentDirection == Direction(South))
		{
			m_XPosition	= m_PreviousXPosition;
			m_CanMoveLeft = false;
			m_CanMoveDown = false;
		}

		if(m_CurrentDirection == Direction(East))
		{
			m_YPosition	= m_PreviousYPosition;
			m_CanMoveDown = false;
			m_CanMoveRight = false;
		}

		if(m_CurrentDirection == Direction(West))
		{
			m_YPosition	= m_PreviousYPosition;
			m_CanMoveDown = false;
			m_CanMoveLeft = false;
		}
	}

	//else set direction to default
	else
	{
		m_PreviousLockedDirection = Direction(None);
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
	m_PreviousYPosition = m_YPosition;
	m_YPosition -= m_MovementSpeed;
}

//!Moves the player positive in the y axis
void Player::MoveDown()
{
	//move the player in its specified direction
	m_PreviousYPosition = m_YPosition;
	m_YPosition += m_MovementSpeed;
}

//!Move sthe player negative in the x axis
void Player::MoveLeft()
{
	//move the player in its specified direction
	m_PreviousXPosition = m_XPosition;
	m_XPosition -= m_MovementSpeed;
}

//!Moves the player positive in the x axis
void Player::MoveRight()
{
	//move the player in its specified direction
	m_PreviousXPosition = m_XPosition;
	m_XPosition += m_MovementSpeed;
}

//Tells the player that their bound one has collided with something in their current moving direction
void Player::MovementCollidingBoundOne()
{
	m_IsCollidingBoundOne = true;
}

//Tells the player that their bound one has collided with something in their current moving direction
void Player::MovementCollidingBoundTwo()
{
	m_IsCollidingBoundTwo = true;
}

//Scales the game up to the input level
//In - 
//		int InputScaleLevel - the input scale level for the game
void Player::ScaleGameUp(int InputScaleLevel)
{
	m_CurrentGameScale = InputScaleLevel;
}

//Give sthe player experience depending on the current scale of the game
void Player::GiveExperience()
{
	//give experience to the player scaled to the current game scale
	m_Experience += (m_ExperienceMultiplier * m_CurrentGameScale);

	//calculate the current levels max experience
	//if the current experience is above the max experience level up the player
	if(m_Experience >= ((m_Level * m_ExperienceMultiplier) * 8))
	{
		//carry over any extra experience
		m_Experience = m_Experience - (m_Level * m_ExperienceMultiplier);
		//level up player
		AddPlayerLevel();
	}

	//draw the level up pop up
	m_DrawExperienceUpTimer = 0;
	m_DrawExperienceUp = true;
}

//Adds a player level to the player
void Player::AddPlayerLevel()
{
	//increment all needed game design variables
	m_Level++;
	m_MaxHealth += m_MaxHealthIncrement;
	m_CurrentHealth = m_MaxHealth;
	m_Inventory.IncreaseRangedWeaponsAttackTime();
	m_ScoreCalculator.AddLevelUpScore();

	//turn on draw level up
	m_DrawLevelUpTimer = 0;
	m_DrawLevelUp = true;
}

//Deal damage to the eplayer's health
//In - 
//		int InputDamage - the input damage to deal to the player
void Player::DealDamage(int InputDamage)
{
	//subject health from input damage
	m_CurrentHealth -= InputDamage;

	//if health goes below 0 the player is dead
	if(m_CurrentHealth < 0)
	{
		m_CurrentHealth = 0;
		m_IsDead = true;
	}
}

//Heals player depending on current scale
void Player::HealPlayer()
{
	//heal player acccording to scale of game
	m_CurrentHealth += (5 * m_CurrentGameScale);

	//if healed health is above max health make it max health
	if(m_CurrentHealth > m_MaxHealth)
	{
		m_CurrentHealth = m_MaxHealth;
	}
}

//Resets the player for a fresh game
void Player::ResetPlayer()
{
	//reset all member variables
	m_IsDead = false;
	m_CurrentGameScale = 1;
	m_DrawExperienceUpTimer = 0;
	m_DrawLevelUpTimer = 0;
	m_SpeedPowerUp = false;
	m_SpeedPowerUpTimer = 0;
	m_MovementSpeed = m_BaseMovementSpeed;
	m_StrengthPowerUp = false;
	m_StrengthPowerUpTimer = 0;

	//reset game design aspects
	m_Level = 1;
	m_Experience = 0;
	m_MaxHealth = m_BaseMaxHealth;
	m_CurrentHealth = m_MaxHealth;
	m_ScoreCalculator.ResetScore();
}

//Returns whether or not the player is dead
bool Player::IsDead()
{
	//Return true if the playe ris dead else false
	if(m_IsDead)
	{
		return true;
	}

	else
	{
		return false;
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

//!Gets and returns the North East Bound Point x position
//Out - 
//		int - North East Bound Point x position
int Player::GetNorthEastXBoundPoint()
{
	return (GetXNorthBoundPoint() + (m_XBound / 2));
}

//!Gets and returns the North East Bound Point y position
//Out - 
//		int - North East Bound Point y position
int Player::GetNorthEastYBoundPoint()
{
	return (GetYNorthBoundPoint());
}

//!Gets and returns the North West Bound Point x position
//Out - 
//		int - North West Bound Point x position
int Player::GetNorthWestXBoundPoint()
{
	return (GetXNorthBoundPoint() - (m_XBound / 2));
}

//!Gets and returns the North West Bound Point y position
//Out - 
//		int - North West Bound Point y position
int Player::GetNorthWestYBoundPoint()
{
	return (GetYNorthBoundPoint());
}

//!Gets and returns the South East Bound Point x position
//Out - 
//		int - South East Bound Point x position
int Player::GetSouthEastXBoundPoint()
{
	return (GetXSouthBoundPoint() + (m_XBound / 2));
}

//!Gets and returns the South East Bound Point y position
//Out - 
//		int - South East Bound Point y position
int Player::GetSouthEastYBoundPoint()
{
	return (GetYSouthBoundPoint());
}

//!Gets and returns the South West Bound Point x position
//Out - 
//		int - South West Bound Point x position
int Player::GetSouthWestXBoundPoint()
{
	return (GetXSouthBoundPoint() - (m_XBound / 2));
}

//!Gets and returns the South West Bound Point y position
//Out - 
//		int - South West Bound Point y position
int Player::GetSouthWestYBoundPoint()
{
	return (GetYSouthBoundPoint());
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

//!Gets and returns the player's first hitbox x bound (will always be top left)
//Out - 
//		int - first x bound of the player hitbox
int Player::GetHitBoxXBoundOne()
{
	return (m_XPosition - (m_XBound / 2));
}

//!Gets and returns the player's first hitbox y bound (will always be top left)
//Out - 
//		int - first y bound of the player hitbox
int Player::GetHitBoxYBoundOne()
{
	return (m_YPosition - (m_YBound / 2));
}

//!Gets and returns the player's second hitbox x bound (will always be bottom right)
//Out - 
//		int - second x bound of the player hitbox
int Player::GetHitBoxXBoundTwo()
{
	return (m_XPosition + (m_XBound / 2));
}

//!Gets and returns the player's second hitbox y bound (will always be bottom right)
//Out - 
//		int - second y bound of the player hitbox
int Player::GetHitBoxYBoundTwo()
{
	return (m_YPosition + (m_YBound / 2));
}

//!Gets and returns the first current X collision bound position of the player relative to the direction the player is moving will always be the north or east most bound point
//Out - 
//		int - the first current X collision bound of the player relative to their moving direction
int Player::GetCollisionXBoundOne()
{
	//check moving direction and return the bound for that specific direction

	if(m_CurrentDirection == Direction(North))
	{
		return GetNorthEastXBoundPoint() - m_SpriteBoundOffset;
	}

	else if(m_CurrentDirection == Direction(South))
	{
		return GetSouthEastXBoundPoint() - m_SpriteBoundOffset;
	}

	else if(m_CurrentDirection == Direction(East))
	{
		return GetNorthEastXBoundPoint() - m_SpriteBoundOffset;
	}

	else if(m_CurrentDirection == Direction(West))
	{
		return GetNorthWestXBoundPoint() + m_SpriteBoundOffset;
	}

	else
	{
		return NULL;
	}
}

//!Gets and returns the first current Y collision bound position of the player relative to the direction the player is moving will always be the north or east most bound point
//Out - 
//		int - the first current Y collision bound of the player relative to their moving direction
int Player::GetCollisionYBoundOne()
{
	//check moving direction and return the bound for that specific direction

	if(m_CurrentDirection == Direction(North))
	{
		return GetNorthEastYBoundPoint() + m_SpriteBoundOffset;
	}

	else if(m_CurrentDirection == Direction(South))
	{
		return GetSouthEastYBoundPoint() - m_SpriteBoundOffset;
	}

	else if(m_CurrentDirection == Direction(East))
	{
		return GetNorthEastYBoundPoint() + m_SpriteBoundOffset;
	}

	else if(m_CurrentDirection == Direction(West))
	{
		return GetNorthWestYBoundPoint() + m_SpriteBoundOffset;
	}

	else
	{
		return NULL;
	}
}

//!Gets and returns the second current X collision bound position of the player relative to the direction the player is moving will always be the south or west most bound point
//Out - 
//		int - the first current X collision bound of the player relative to their moving direction
int Player::GetCollisionXBoundTwo()
{
	//check moving direction and return the bound for that specific direction

	if(m_CurrentDirection == Direction(North))
	{
		return GetNorthWestXBoundPoint() + m_SpriteBoundOffset;
	}

	else if(m_CurrentDirection == Direction(South))
	{
		return GetSouthWestXBoundPoint() + m_SpriteBoundOffset;
	}

	else if(m_CurrentDirection == Direction(East))
	{
		return GetSouthEastXBoundPoint() - m_SpriteBoundOffset;
	}

	else if(m_CurrentDirection == Direction(West))
	{
		return GetSouthWestXBoundPoint() + m_SpriteBoundOffset;
	}

	else
	{
		return NULL;
	}
}

//!Gets and returns the second current Y collision bound position of the player relative to the direction the player is moving will always be the south or west most bound point
//Out - 
//		int - the first current Y collision bound of the player relative to their moving direction
int Player::GetCollisionYBoundTwo()
{
	//check moving direction and return the bound for that specific direction

	if(m_CurrentDirection == Direction(North))
	{
		return GetNorthWestYBoundPoint() + m_SpriteBoundOffset;
	}

	else if(m_CurrentDirection == Direction(South))
	{
		return GetSouthWestYBoundPoint() - m_SpriteBoundOffset;
	}

	else if(m_CurrentDirection == Direction(East))
	{
		return GetSouthEastYBoundPoint() - m_SpriteBoundOffset;
	}

	else if(m_CurrentDirection == Direction(West))
	{
		return GetSouthWestYBoundPoint() - m_SpriteBoundOffset;
	}

	else
	{
		return NULL;
	}
}

//!Gets and returns the current weapon's first active hit box x bound
//Out - 
//		int - x bound of the current weapon's active hit box
int Player::GetWeaponHitBoxXBoundOne()
{
	return m_ActiveWeapon->GetHitBoxXBoundOne();
}

//!Gets and returns the current weapon's first active hit box y bound
//Out - 
//		int - y bound of the current weapon's active hit box
int Player::GetWeaponHitBoxYBoundOne()
{
	return m_ActiveWeapon->GetHitBoxYBoundOne();
}

//!Gets and returns the current weapon's second active hit box x bound
//Out - 
//		int - x bound of the current weapon's active hit box
int Player::GetWeaponHitBoxXBoundTwo()
{
	return m_ActiveWeapon->GetHitBoxXBoundTwo();
}

//!Gets and returns the current weapon's second active hit box y bound
//Out - 
//		int - y bound of the current weapon's active hit box
int Player::GetWeaponHitBoxYBoundTwo()
{
	return m_ActiveWeapon->GetHitBoxYBoundTwo();
}

//!Gets and returns the current weapon's projectile if it is a ranged weapon
//Out - 
//		Projectile* - the current projectile of the current weapon
Projectile* Player::GetWeaponProjectile()
{
	return m_ActiveWeapon->GetProjectile();
}

//Gets and returns the player's current level
//Out - 
//		float - the player's current level
int Player::GetCurrentLevel()
{
	return m_Level;
}

//Gets and returns the player's current health
//Out - 
//		float - the player's current health
int Player::GetCurrentHealth()
{
	return m_CurrentHealth;
}

//!Gets and returns the current weapon's damage
//Out - 
//		float - the damage of the current weapon
float Player::GetWeaponDamage()
{
	//if there is a power up on calculate the new damage

	if(m_StrengthPowerUp)
	{
		return (((m_ActiveWeapon->GetDamage() + m_Level) + (m_Level * 2)) * m_StrengthPowerupMultiplier);
	}

	else
	{
		return ((m_ActiveWeapon->GetDamage() + m_Level) + (m_Level * 2));
	}
}

//!Gets and returns the current player score
//Out
//		int - the final calculated player score
int Player::GetCurrentScore()
{
	return m_ScoreCalculator.GetCurrentPlayerScore();
}

//!Gets and returns the final player score calculated with the timer
//In - 
//		const ALLEGRO_TIMER* InputTimer - the input timer of the game
//Out
//		int - the final calculated player score
int Player::GetFinalTimedScore(const ALLEGRO_TIMER* InputTimer)
{
	return m_ScoreCalculator.CalculateTimedScore(InputTimer, m_CurrentGameScale);
}

//!Sets the x position of the player
//In - 
//		int NewXPosition - the new x position for the player
void Player::SetXPosition(float NewXPosition)
{
	m_XPosition = NewXPosition;
	m_PreviousXPosition = NewXPosition;
}

//!Sets the y position of the player
//In - 
//		int NewYPosition - the new y position for the player
void Player::SetYPosition(float NewYPosition)
{
	m_YPosition = NewYPosition;
	m_PreviousYPosition = NewYPosition;
}

