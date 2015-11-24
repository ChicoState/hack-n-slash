//Scott Backer

//Player Class H: Player class will handle all systems having to do with the character that the player controls. 

#ifndef PLAYER_H
#define PLAYER_H

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

#include "PlayerTile.h"
#include "Camera.h"
#include "Inventory.h"
#include "Weapon.h"
<<<<<<< HEAD

#include "SwordWeapon.h"
#include "LongSwordWeapon.h"
=======
#include "Projectile.h"
#include "FoodPickup.h"

#include "SwordWeapon.h"
#include "BowWeapon.h"
>>>>>>> Scott

#include <string>
#include <map>

//#include "Utility.h"

class Player
{
public:

<<<<<<< HEAD
	Player(ALLEGRO_BITMAP *Image, int InputScreenWidth, int InputScreenHeight, ALLEGRO_EVENT_QUEUE* InputEventQueue); //Constructor for the player class
=======
	Player(ALLEGRO_EVENT_QUEUE* InputEventQueue); //Constructor for the player class
>>>>>>> Scott

	~Player(); //Deconstructor for the player class

	void EventHandler(ALLEGRO_EVENT& InputAlEvent, float InputMouseXWorldPosition, float InputMouseYWorldPosition); //Handles all the functions for the player that need to be called every update
<<<<<<< HEAD
	void DrawPlayer(); //Draws the player character to the screen
	void MovementColliding(); //Tells the player that they have collided with something in thier current moving direction
=======
	void ScaleGameUp(int InputScaleLevel); //Scales the game up to the input level
	void DrawPlayer(); //Draws the player character to the screen
	void NoMovementCollidingBoundOne(); //Tells the player that they are not colliding with something in their current moving direction
	void NoMovementCollidingBoundTwo(); //Tells the player that they are not colliding with something in their current moving direction
	void MovementCollidingBoundOne(); //Tells the player that their bound one has collided with something in their current moving direction
	void MovementCollidingBoundTwo(); //Tells the player that their bound two has collided with something in their current moving direction
	void GiveExperience(); //Give sthe player experience depending on the current scale of the game
	void AddPlayerLevel(); //Adds a player level to the player
	void DealDamage(int InputDamage); //Deal damage to th eplayer's health
	void HealPlayer(); //Heals player depending on current scale
	void ResetPlayer(); //Resets the player for a fresh game

	bool IsDead(); //Returns whether or not the player is dead
>>>>>>> Scott

	std::string GetTag(); //Gets and returns the player class tag
	int GetXBound(); //Gets and returns the X Bound of the player
	int GetYBound(); //Gets and returns the Y Bound of the player
	int GetXNorthBoundPoint(); //Gets and returns the North Bound Point x position
	int GetYNorthBoundPoint(); //Gets and returns the North Bound Point y position
	int GetXSouthBoundPoint(); //Gets and returns the South Bound Point x position
	int GetYSouthBoundPoint(); //Gets and returns the South Bound Point y position
	int GetXEastBoundPoint(); //Gets and returns the East Bound Point x position
	int GetYEastBoundPoint(); //Gets and returns the East Bound Point y position
	int GetXWestBoundPoint(); //Gets and returns the West Bound Point x position
	int GetYWestBoundPoint(); //Gets and returns the West Bound Point y position
<<<<<<< HEAD
	float GetXPosition(); //Gets and returns the X position of the player
	float GetYPosition(); //Gets and returns the Y position of the player
	float GetCurrentMovingXPosition(); //Gets and returns the current X bound position of the player relative to the direction the player is moving
	float GetCurrentMovingYPosition(); //Gets and returns the current y bound position of the player relative to the direction the player is moving


=======
	int GetNorthEastXBoundPoint(); //Gtes and returns the north east bound point x position
	int GetNorthEastYBoundPoint(); //Gtes and returns the north east bound point y position
	int GetSouthEastXBoundPoint(); //Gtes and returns the south east bound point x position
	int GetSouthEastYBoundPoint(); //Gtes and returns the south east bound point y position
	int GetSouthWestXBoundPoint(); //Gtes and returns the south west bound point x position
	int GetSouthWestYBoundPoint(); //Gtes and returns the south west bound point y position
	int GetNorthWestXBoundPoint(); //Gtes and returns the north west bound point x position
	int GetNorthWestYBoundPoint(); //Gtes and returns the north west bound point y position
	int GetXPosition(); //Gets and returns the X position of the player
	int GetYPosition(); //Gets and returns the Y position of the player
	int GetHitBoxXBoundOne(); //!Gets and returns the player's first hitbox x bound (will always be top left)
	int GetHitBoxYBoundOne(); //!Gets and returns the player's first hitbox x bound (will always be top left)
	int GetHitBoxXBoundTwo(); //!Gets and returns the player's second hitbox x bound (will always be bottom right)
	int GetHitBoxYBoundTwo(); //!Gets and returns the player's second hitbox x bound (will always be bottom right)
	int GetCollisionXBoundOne(); //Gets and returns the first current X collision bound position of the player relative to the direction the player is moving will always be the north or east most bound point
	int GetCollisionYBoundOne(); //Gets and returns the first current Y collision bound position of the player relative to the direction the player is moving will always be the north or east most bound point
	int GetCollisionXBoundTwo(); //Gets and returns the second current X collision bound position of the player relative to the direction the player is moving will always be the south or west most bound point
	int GetCollisionYBoundTwo(); //Gets and returns the second current Y collision bound position of the player relative to the direction the player is moving will always be the south or west most bound point
	int GetWeaponHitBoxXBoundOne(); //!Gets and returns the current weapon's first active hit box x bound
	int GetWeaponHitBoxYBoundOne(); //!Gets and returns the current weapon's first active hit box y bound
	int GetWeaponHitBoxXBoundTwo(); //!Gets and returns the current weapon's second active hit box x bound
	int GetWeaponHitBoxYBoundTwo(); //!Gets and returns the current weapon's second active hit box y bound
	Projectile* GetWeaponProjectile(); //!Gets and returns the current weapon's projectile if it is a ranged weapon
	int GetCurrentHealth(); //Gets and returns the player's current health
	float GetWeaponDamage(); //!Gets and returns the current weapon's damage
>>>>>>> Scott
	void SetXPosition(float NewXPosition); //Sets the x position of the player
	void SetYPosition(float NewYPosition); //Sets the y position of the player

private:

<<<<<<< HEAD
=======
	//Player class enum for direction and which way the player is moving and facing
>>>>>>> Scott
	enum Direction
	{
		North,
		South,
		East,
<<<<<<< HEAD
		West
	};

	
	void CheckMovement(float InputMouseXWorldPosition, float InputMouseYWorldPosition); //Handles movement for the player character each update
=======
		West,
		None
	};

	void CheckMovement(float InputMouseXWorldPosition, float InputMouseYWorldPosition); //Handles movement for the player character each update
	void CheckCollision(); //!Checks and updates the collision of the player to allow movement directions
>>>>>>> Scott
	void CheckMouseMovement(); //Checks each frame to see if the player needs to move from a mouse click and update sthe position
	void MoveUp(); //Moves the player negative in the y axis
	void MoveDown(); //Moves the player positive in the y axis
	void MoveLeft(); //Move sthe player negative in the x axis
	void MoveRight(); //Moves the player positive in the x axis

	ALLEGRO_EVENT_QUEUE *m_EventQueue; //The event queue for the player class
	ALLEGRO_EVENT m_AlEvent; //the event variable for the player class
	ALLEGRO_FONT *font28; //font for player health
	ALLEGRO_FONT *font16; //font for experience and level up draw
	ALLEGRO_EVENT_SOURCE m_PositionEventSource; //event source for emitting player position

<<<<<<< HEAD
	int m_ScreenWidth; //the screen width dimension of the game
	int m_ScreenHeight; //the screen height dimension of the game
	PlayerTile m_PlayerTile;
	Camera *m_Camera;
	Inventory m_Inventory;
	Weapon* m_ActiveWeapon;
=======
	int m_CurrentGameScale; //the current scale level of the game
	int m_ExperienceMultiplier; //the multiplier to calculate player experience gained
	int m_MaxHealthIncrement; //the amount of health to increment the max health on level up
	PlayerTile m_PlayerTile; //member player tile instance
	Camera *m_Camera; //member camera instance
	Inventory m_Inventory; //Player inventory
	Weapon* m_ActiveWeapon; //Player current active weapon holder
>>>>>>> Scott

	std::string ClassTag; //tag for the player class
	int m_XBound; //the x bound for the player
	int m_YBound; //the y bound for the player
<<<<<<< HEAD
	float m_XPosition; //the current x position of the player
	float m_YPosition; //the current y position of the player
	Direction m_CurrentDirection;
=======
	int m_XPosition; //the current x position of the player
	int m_YPosition; //the current y position of the player
	float m_PreviousXPosition; //the previous x position of the player
	float m_PreviousYPosition; //the previous y position of the player
	Direction m_CurrentDirection; //The current moving direction of the player
	Direction m_PreviousLockedDirection; //The last direction locked from colliding
	int m_BaseMaxHealth; //the base max health for the player
	int m_MaxHealth; //The max health of the player
	int m_CurrentHealth; //the current health of the player
	int m_Level; //the current player level
	int m_Experience; //the current player experience
	int m_BaseMovementSpeed; //the default base movement speed
>>>>>>> Scott
	int m_MovementSpeed; //the speed at which the player moves
	bool m_MouseMoving; //true if the player is moving by a mouse click and the position has not been reached else false
	int m_CurrentMouseMoveXPosition; //the last x position the player clicked to move to
	int m_CurrentMouseMoveYPosition; //the last y position the player clicked to move to
	bool m_KeyboardMoving; //true if the player is moving by the keyboard else false
	std::map <std::string, bool> m_KeyboardMap; //map of the keyboard keys and their bools for the player
	bool m_CanMoveUp; //true if the player can move up else false
	bool m_CanMoveDown; //true if the player can move down else false
	bool m_CanMoveLeft; //true if the player can move left else false
	bool m_CanMoveRight; //true if the player can move right else false
<<<<<<< HEAD
	bool m_IsColliding; //true if the ploayer is colliding else false
	float m_LockedXPosition; //x position to lock the player to when their bounds go off screen
	float m_LockedYPosition; //y position to lock the player to when their bounds go off screen
=======
	bool m_IsColliding; //true if the player is colliding in their movement else false
	bool m_IsCollidingBoundOne; //true if the ploayer is colliding on movement bound one else false
	bool m_IsCollidingBoundTwo; //true if the ploayer is colliding on movement bound two else false
	float m_LockedXPosition; //x position to lock the player to when their bounds go off screen
	float m_LockedYPosition; //y position to lock the player to when their bounds go off screen
	bool m_CanAttack; //tells the player whether or not they can attack
	bool m_IsDead; //true if the player is dead else false

	int m_PowerupTimerLength; //the length of time for power ups
	int m_SpeedPowerUpSpeed; //the speed the player moves with the speed power up
	bool m_SpeedPowerUp; //true if the player has the speed power up
	int m_SpeedPowerUpTimer; //timer for the speed power up
	int m_StrengthPowerupMultiplier; //the multipler to use to power up the player weapon damage
	bool m_StrengthPowerUp; //true if the player has the srength powerup boost
	int m_StrengthPowerUpTimer; //timer for the strength power up
	
	bool m_DrawExperienceUp; //true if experience marker should be drawn else false
	int m_DrawExperienceUpTimer; //the amount of time the draw experience up should be displayed
	bool m_DrawLevelUp; //true if the level up marker should be drawn else false
	int m_DrawLevelUpTimer; //the amount of time the draw level up should be displayed
>>>>>>> Scott
};

#endif