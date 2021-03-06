//Scott Backer

//Camera Class CPP: The camera class will handle all functions for moving the camera in-game

#include <stdlib.h>

#include "Camera.h"

//!The constructor for the camera class
//In - 
//		ALLEGRO_EVENT_QUEUE* InputEventQueue - the overall game event queue input into the player class
//		int InputScreenWidth - the game screen width
//		int InputScreenHeight - the game screen height
Camera::Camera(ALLEGRO_EVENT_QUEUE* InputEventQueue, int InputScreenWidth, int InputScreenHeight) : m_EventQueue(InputEventQueue)
{
	//initialize member variables
	m_ScreenWidth = InputScreenWidth;
	m_ScreenHeight = InputScreenHeight;

	m_MouseXCoordinate = 0;
	m_MouseYCoordinate = 0;

	m_XPosition = 0;
	m_YPosition = 0;
}

//!Handles all the functions for the camera that need to be called every update
//In - 
//		ALLEGRO_EVENT& InputAlEvent - The global event handler for the game
//		float PlayerXPosition - The current x position of the player
//		float PlayerYPosition - The current y position of the player
void Camera::EventHandler(ALLEGRO_EVENT& InputAlEvent, float PlayerXPosition, float PlayerYPosition)
{
	//make member event the same as the input event
	m_AlEvent = InputAlEvent;

	//Update the position of the camera
	UpdatePosition(PlayerXPosition, PlayerYPosition);

	//Keep track of mouse coordinates
	m_MouseXCoordinate = m_AlEvent.mouse.x;
	m_MouseYCoordinate = m_AlEvent.mouse.y;
}


//!Updates the position of the camera relative to the player position
//In - 
//		float PlayerXPosition - The current x position of the player
//		float PlayerYPosition - The current y position of the player
void Camera::UpdatePosition(float PlayerXPosition, float PlayerYPosition)
{
	//Update the posiotion of the camera relative to the player position and bound
	m_XPosition = -(m_ScreenWidth / 2) + (PlayerXPosition);
	m_YPosition = -(m_ScreenHeight / 2) + (PlayerYPosition);

	//Update the transform and translation of the camera
	UpdateTransform();
}

//!Updates the transform and translation of the camera
void Camera::UpdateTransform()
{
	//Update the transform and translation of the camera
	al_identity_transform(&CameraTransform);
	al_translate_transform(&CameraTransform, -(m_XPosition), -(m_YPosition));
	al_use_transform(&CameraTransform);
}

//Resets the transform/translate position of the camera to regular screen space
void Camera::ResetTranslate()
{
	//reset the transform and translation of the camera
	al_identity_transform(&CameraTransform);
	al_use_transform(&CameraTransform);
}

//!Gets and returns the mouse x world coordinate translated from the screen coordinate
float Camera::GetMouseXWorldCoordinate()
{
	//get the current mouse screen positions
	float TempMouseXCoordinate = m_MouseXCoordinate;
	float TempMouseYCoordinate = m_MouseYCoordinate;

	//transform mouse screen coordinate sinto world coordinates
	al_transform_coordinates(&CameraTransform, &TempMouseXCoordinate, &TempMouseYCoordinate);

	return TempMouseXCoordinate;
}

//!Gets and returns the mouse y world coordinate translated from the screen coordinate
float Camera::GetMouseYWorldCoordinate()
{
	//get the current mouse screen positions
	float TempMouseXCoordinate = m_AlEvent.mouse.x;
	float TempMouseYCoordinate = m_AlEvent.mouse.y;

	//transform mouse screen coordinate sinto world coordinates
	al_transform_coordinates(&CameraTransform, &TempMouseXCoordinate, &TempMouseYCoordinate);

	return TempMouseYCoordinate;
}