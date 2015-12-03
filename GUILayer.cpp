//Scott Backer

//GUILayer.CPP - Draws the GUI Layer of the game to the screen

#include "GUILayer.h"

//!Constructor for the GUI Layer
GUILayer::GUILayer(int InputScreenWidth, int InputScreenHeight)
{
	//set member variables
	m_ScreenWidth = InputScreenWidth;
	m_ScreenHeight = InputScreenHeight;
	
	//initialze member variables
	PlayerInfoImage = al_load_bitmap("Player_Info.png");
	TimerInfoImage = al_load_bitmap("Timer_Info.png");
	font96 = al_load_font("GOUDOS.TTF", 72, 0);
	font28 = al_load_font("GOUDOS.TTF", 28, 0);
}

//!Draws the player information to the gui layer
/*!
\param		InputCamera - the current camera of the game to reset translate
\param		INputPlayerLevel - the input player level to draw
\param		InputPlayerHealth - th einput player health to draw
*/
void GUILayer::DrawPlayerInformation(Camera& InputCamera, int InputPlayerLevel, int InputPlayerHealth)
{
	//reset camera translate
	InputCamera.ResetTranslate();

	//draw info box
	//al_draw_rectangle(0, (m_ScreenHeight - 75), (m_ScreenWidth - 1750), m_ScreenHeight, al_map_rgb(0, 0, 0), 100);
	al_draw_bitmap(PlayerInfoImage, 0, (m_ScreenHeight - 150), NULL);

	//draw level
	std::string LevelNumber = std::to_string(InputPlayerLevel);
	std::string FullLevelText = "Level: ";
	FullLevelText.append(LevelNumber);
	char const *LevelChar = FullLevelText.c_str();
	al_draw_text(font28, al_map_rgb(150, 255, 0), (m_ScreenWidth / 17), (m_ScreenHeight - (m_ScreenHeight / 13)), ALLEGRO_ALIGN_CENTER, LevelChar);

	//draw health
	std::string HealthNumber = std::to_string(InputPlayerHealth);
	std::string FullHealthText = "Health: ";
	FullHealthText.append(HealthNumber);
	char const *HealthChar = FullHealthText.c_str();
	al_draw_text(font28, al_map_rgb(150, 255, 0), (m_ScreenWidth / 17), (m_ScreenHeight - (m_ScreenHeight / 9)), ALLEGRO_ALIGN_CENTER, HealthChar);
}

//Draws the score information of the player
/*!
\param		InputCamera - the current camera of the game to reset translate
\param		InputScore - the input score to draw
*/
void GUILayer::DrawScoreInformation(Camera& InputCamera, int InputScore)
{
	//reset camera translate
	InputCamera.ResetTranslate();

	//draw score
	std::string ScoreNumber = std::to_string(InputScore);
	std::string FullScoreText = "Score: ";
	FullScoreText.append(ScoreNumber);
	char const *ScoreChar = FullScoreText.c_str();
	al_draw_text(font28, al_map_rgb(150, 255, 0), (m_ScreenWidth / 17), (m_ScreenHeight - (m_ScreenHeight / 24)), ALLEGRO_ALIGN_CENTER, ScoreChar);
}

//Draws the game time information of the player
/*!
\param		InputCamera - the current camera of the game to reset translate
\param		InputTimer - Timer to calculate game time in seconds
*/
void GUILayer::DrawGameTimeInformation(Camera& InputCamera, const ALLEGRO_TIMER* InputTimer)
{
	//reset camera translate
	InputCamera.ResetTranslate();

	//load time info image
	al_draw_bitmap(TimerInfoImage, (m_ScreenWidth - 215), 0, NULL);

	//draw time info
	std::string TimeNumber = std::to_string((al_get_timer_count(InputTimer)) / 60);
	std::string FullTimeText = "Time: ";
	FullTimeText.append(TimeNumber);
	char const *TimeChar = FullTimeText.c_str();
	al_draw_text(font28, al_map_rgb(150, 255, 0), (m_ScreenWidth - 110), 35, ALLEGRO_ALIGN_CENTER, TimeChar);
}

//Draws the final score information of the player on the end screen
/*!
\param		InputFinalScore - the input score to draw
*/
void GUILayer::DrawFinalScoreScreen(int InputFinalScore)
{
	//draw final score
	std::string ScoreNumber = std::to_string(InputFinalScore);
	std::string FullScoreText = "Final Score: ";
	FullScoreText.append(ScoreNumber);
	char const *ScoreChar = FullScoreText.c_str();
	al_draw_text(font96, al_map_rgb(14, 33, 93), (m_ScreenWidth - 380), 550, ALLEGRO_ALIGN_CENTER, ScoreChar);
}