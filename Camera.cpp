//Scott Backer

//Camera Class CPP: The camera class will handle all functions for moving the camera in-game

#include "Camera.h"

//!The constructor for the camera class
//In - 
//		ALLEGRO_EVENT_QUEUE* InputEventQueue - the overall game event queue input into the player class
Camera::Camera(ALLEGRO_EVENT_QUEUE* InputEventQueue) : m_EventQueue(InputEventQueue)
{
	//Set input variables to member variables

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
//		int PlayerWidth - The player width bound
//		int PlayerHeight - The player height bound
void Camera::EventHandler(ALLEGRO_EVENT& InputAlEvent, float PlayerXPosition, float PlayerYPosition)
{
	//make member event the same as the input event
	m_AlEvent = InputAlEvent;

	if(m_AlEvent.type = ALLEGRO_EVENT_TIMER)
	{
		//Update the position of the camera
		UpdatePosition(PlayerXPosition, PlayerYPosition);

		//Keep track of mouse coordinates
		m_MouseXCoordinate = m_AlEvent.mouse.x;
		m_MouseYCoordinate = m_AlEvent.mouse.y;

		//printf("%i", m_MouseXCoordinate);
		//printf("%i", m_MouseYCoordinate);
	}
}


//!Updates the position of the camera relative to the player position
//In - 
//		float PlayerXPosition - The current x position of the player
//		float PlayerYPosition - The current y position of the player
//		int PlayerWidth - The player width bound
//		int PlayerHeight - The player height bound	
void Camera::UpdatePosition(float PlayerXPosition, float PlayerYPosition)
{
	//Update the posiotion of the camera relative to the player position and bound
	//m_XPosition = -(1280 / 2) + (PlayerXPosition + (PlayerWidth / 2));
	//m_YPosition = -(720 / 2) + (PlayerYPosition + (PlayerHeight / 2));
	m_XPosition = -(1280 / 2) + (PlayerXPosition);
	m_YPosition = -(720 / 2) + (PlayerYPosition);

	//if the x position is less than 0 keep it there
	if(m_XPosition < 0)
	{
		m_XPosition = 0;
	}

	//if the y position is less than 0 keep it there
	if(m_YPosition < 0)
	{
		m_YPosition = 0;
	}

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

float Camera::GetMouseXWorldCoordinate()
{
	float TempMouseXCoordinate = m_MouseXCoordinate;
	float TempMouseYCoordinate = m_MouseYCoordinate;

	al_transform_coordinates(&CameraTransform, &TempMouseXCoordinate, &TempMouseYCoordinate);

	return TempMouseXCoordinate;
}

float Camera::GetMouseYWorldCoordinate()
{
	float TempMouseXCoordinate = m_AlEvent.mouse.x;
	float TempMouseYCoordinate = m_AlEvent.mouse.y;

	al_transform_coordinates(&CameraTransform, &TempMouseXCoordinate, &TempMouseYCoordinate);

	return TempMouseYCoordinate;
}