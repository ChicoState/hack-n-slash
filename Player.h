//Scott Backer

//Player Class H: Player class will handle all systems having to do with the character that the player controls. 

#include <allegro5\allegro.h>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>

#include <string>
#include <map>

//#include "Utility.h"

class Player
{
public:

	Player(int InputScreenWidth, int InputScreenHeight, ALLEGRO_EVENT_QUEUE* InputEventQueue); //Constructor for the player class

	~Player(); //Deconstructor for the player class

	void EventHandler(); //Handles all the functions for the player that need to be called every update
	
	std::string GetTag(); //Gets and returns the player class tag
	int GetXBound(); //Gets and returns the X Bound of the player
	int GetYBound(); //Gets and returns the Y Bound of the player
	int GetXNorthBoundPoint(); //!Gets and returns the North Bound Point x position
	int GetYNorthBoundPoint(); //!Gets and returns the North Bound Point y position
	int GetXSouthBoundPoint(); //!Gets and returns the South Bound Point x position
	int GetYSouthBoundPoint(); //!Gets and returns the South Bound Point y position
	int GetXEastBoundPoint(); //!Gets and returns the East Bound Point x position
	int GetYEastBoundPoint(); //!Gets and returns the East Bound Point y position
	int GetXWestBoundPoint(); //!Gets and returns the West Bound Point x position
	int GetYWestBoundPoint(); //!Gets and returns the West Bound Point y position
	int GetXPosition(); //Gets and returns the X position of the player
	int GetYPosition(); //Gets and returns the Y position of the player

	void SetXPosition(int NewXPosition); //Sets the x position of the player
	void SetYPosition(int NewYPosition); //Sets the y position of the player

private:

	void DrawPlayer(); //Draws the player character to the screen
	void CheckMovement(); //Handles movement for the player character each update
	void MoveUp(); //Moves the player negative in the y axis
	void MoveDown(); //Moves the player positive in the y axis
	void MoveLeft(); //Move sthe player negative in the x axis
	void MoveRight(); //Moves the player positive in the x axis

	ALLEGRO_EVENT_QUEUE *m_EventQueue; //The event queue for the player class
	ALLEGRO_EVENT m_AlEvent; //the event variable for the player class

	int m_ScreenWidth; //the screen width dimension of the game
	int m_ScreenHeight; //the screen height dimension of the game

	std::string ClassTag; //tag for the player class
	int m_XBound; //the x bound for the player
	int m_YBound; //the y bound for the player
	int m_XPosition; //the current x position of the player
	int m_YPosition; //the current y position of the player
	std::map <std::string, bool> m_KeyboardMap; //map of the keyboard keys and their bools for the player
	int m_MovementSpeed; //the speed at which the player moves
	bool m_CanMoveUp; //true if the player can move up else false
	bool m_CanMoveDown; //true if the player can move down else false
	bool m_CanMoveLeft; //true if the player can move left else false
	bool m_CanMoveRight; //true if the player can move right else false
	int m_LockedXPosition; //x position to lock the player to when their bounds go off screen
	int m_LockedYPosition; //y position to lock the player to when their bounds go off screen
};