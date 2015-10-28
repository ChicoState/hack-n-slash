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

#include "SwordWeapon.h"
//#include "LongSwordWeapon.h"

#include <string>
#include <map>

//#include "Utility.h"

class Player
{
public:

	Player(ALLEGRO_BITMAP *SpriteImage, ALLEGRO_BITMAP *SwordImage, int InputScreenWidth, int InputScreenHeight, ALLEGRO_EVENT_QUEUE* InputEventQueue); //Constructor for the player class

	~Player(); //Deconstructor for the player class

	void EventHandler(ALLEGRO_EVENT& InputAlEvent, float InputMouseXWorldPosition, float InputMouseYWorldPosition); //Handles all the functions for the player that need to be called every update
	void DrawPlayer(); //Draws the player character to the screen
	void MovementCollidingBoundOne(); //Tells the player that their bound one has collided with something in their current moving direction
	void MovementCollidingBoundTwo(); //Tells the player that their bound two has collided with something in their current moving direction

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
	int GetNorthEastXBoundPoint(); //Gtes and returns the north east bound point x position
	int GetNorthEastYBoundPoint(); //Gtes and returns the north east bound point y position
	int GetSouthEastXBoundPoint(); //Gtes and returns the south east bound point x position
	int GetSouthEastYBoundPoint(); //Gtes and returns the south east bound point y position
	int GetSouthWestXBoundPoint(); //Gtes and returns the south west bound point x position
	int GetSouthWestYBoundPoint(); //Gtes and returns the south west bound point y position
	int GetNorthWestXBoundPoint(); //Gtes and returns the north west bound point x position
	int GetNorthWestYBoundPoint(); //Gtes and returns the north west bound point y position
	float GetXPosition(); //Gets and returns the X position of the player
	float GetYPosition(); //Gets and returns the Y position of the player
	int GetCollisionXBoundOne(); //Gets and returns the first current X collision bound position of the player relative to the direction the player is moving will always be the north or east most bound point
	int GetCollisionYBoundOne(); //Gets and returns the first current Y collision bound position of the player relative to the direction the player is moving will always be the north or east most bound point
	int GetCollisionXBoundTwo(); //Gets and returns the second current X collision bound position of the player relative to the direction the player is moving will always be the south or west most bound point
	int GetCollisionYBoundTwo(); //Gets and returns the second current Y collision bound position of the player relative to the direction the player is moving will always be the south or west most bound point
	void SetXPosition(float NewXPosition); //Sets the x position of the player
	void SetYPosition(float NewYPosition); //Sets the y position of the player

private:

	//Player class enum for direction and which way the player is moving and facing
	enum Direction
	{
		North,
		South,
		East,
		West
	};

	void CheckMovement(float InputMouseXWorldPosition, float InputMouseYWorldPosition); //Handles movement for the player character each update
	void CheckCollision(); //!Checks and updates the collision of the player to allow movement directions
	void CheckMouseMovement(); //Checks each frame to see if the player needs to move from a mouse click and update sthe position
	void MoveUp(); //Moves the player negative in the y axis
	void MoveDown(); //Moves the player positive in the y axis
	void MoveLeft(); //Move sthe player negative in the x axis
	void MoveRight(); //Moves the player positive in the x axis

	ALLEGRO_EVENT_QUEUE *m_EventQueue; //The event queue for the player class
	ALLEGRO_EVENT m_AlEvent; //the event variable for the player class

	int m_ScreenWidth; //the screen width dimension of the game
	int m_ScreenHeight; //the screen height dimension of the game
	PlayerTile m_PlayerTile; //member player tile instance
	Camera *m_Camera; //member camera instance
	Inventory m_Inventory; //Player inventory
	Weapon* m_ActiveWeapon; //Player current active weapon holder

	std::string ClassTag; //tag for the player class
	int m_XBound; //the x bound for the player
	int m_YBound; //the y bound for the player
	float m_XPosition; //the current x position of the player
	float m_YPosition; //the current y position of the player
	Direction m_CurrentDirection;
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
	bool m_IsCollidingBoundOne; //true if the ploayer is colliding on movement bound one else false
	bool m_IsCollidingBoundTwo; //true if the ploayer is colliding on movement bound two else false
	float m_LockedXPosition; //x position to lock the player to when their bounds go off screen
	float m_LockedYPosition; //y position to lock the player to when their bounds go off screen
};

#endif