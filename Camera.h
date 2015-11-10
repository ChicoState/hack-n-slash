//Scott Backer

//Camera Class H: The camera class will handle all functions for moving the camera in-game

#ifndef CAMERA_H
#define CAMERA_H

#include <allegro5\allegro.h>

#include <string>

class Camera
{

public:
	Camera(ALLEGRO_EVENT_QUEUE* InputEventQueue); //Constructor for the camera class
	//Handles all the functions for the camera that need to be called every update
	void EventHandler(ALLEGRO_EVENT& InputAlEvent, float PlayerXPosition, float PlayerYPosition); 
	float GetCameraXPosition(); //Gets and returns the cameras x position
	float GetCameraYPosition(); //Gets and returns the cameras y position

	float GetMouseXWorldCoordinate(); //!Gets and returns the mouse x world coordinate translated from the screen coordinate
	float GetMouseYWorldCoordinate(); //!Gets and returns the mouse y world coordinate translated from the screen coordinate

private:
	void UpdatePosition(float playerXPosition, float playerYPosition); //Updates the position of the camera relative to the player position
	void UpdateTransform(); //Updates the transform and translation of the camera

	ALLEGRO_EVENT_QUEUE *m_EventQueue; //The event queue for the player class
	ALLEGRO_EVENT m_AlEvent; //the event variable for the player class
	ALLEGRO_TRANSFORM CameraTransform; //The transform for the camera

	float m_MouseXCoordinate; //mouse coordinate x on screen
	float m_MouseYCoordinate; //mouse coordinate y on screen

	float m_XPosition; //x position of the camera
	float m_YPosition; //y position of the camera
};

#endif