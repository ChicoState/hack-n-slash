//Created by:		Jesse Herzenach
//Date updated:		10/05/15
//Last update by:	N/A
//Reason for update:N/A

#ifndef __TERRAIN_GENERATOR__
#define __TERRAIN_GENERATOR__

#include "Vec2.h"
#include "SpriteGenerator.h"
#include <noise.h>
#include <vector>
#include <time.h>

#include "noiseutils.h"
#include "allegro5\allegro.h"
#include "allegro5\allegro_primitives.h"
#include "TerrainTile.h"
#include "TerrainLayer.h"
#include "TerrainTile.h"
#include "TerrainMap.h"
#include "Player.h"

#include <opencv2\opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/stitching.hpp>

//Biome class is not currently in use, but leaving here for potential future use with the Fractal class
class Biome{
public:
	enum biomeType{
		FOREST,
		TUNDRA,
		DESERT
	};
	Biome(int level){
		level = level % 3 + 1;
		switch (level){
		case 1:
			m_type = biomeType::FOREST;
		case 2:
			m_type = biomeType::DESERT;
		case 3:
			m_type = biomeType::TUNDRA;
		}
	};
	//~Biome();
	int getType(){
		switch (m_type){
		case biomeType::FOREST:
			return 1;
		case biomeType::DESERT:
			return 2;
		case biomeType::TUNDRA:
			return 3;
		}
	}
private:
	biomeType m_type;

};
//Fractal class is not currently in use, but leaving here for potential future use, as the algorithms coded have various uses
class Fractal{
public:
	Fractal(){
		roughness = 0.6f; //default value of roughness for testing
	};
	void drawGrayscale();

	//~Fractal();
	float** generateFractal(Biome*); //generate fractal calls setheightconstants and then runs the diamond square algorithm to produce the height map
	void setInitialConditions(Biome*);
	void initializeHeightmap();
	void diamond(int size, int X, int Y, float range);
	void square(int size, int X, int Y, float range);
	void calculateAverage();
	float getAverage(){ return average; }
	float getLowest(){ return lowest; }
	float getHighest(){ return highest; }
	void print();
private:
	int m_size = 129; //width and height of the heightmap to be generated
	float *m_heightMap[129];
	float roughness;
	float highest;
	float lowest;
	float average;
};

//This is the main class that is currently being used for terrain generation
class TerrainGenerator{
public:
	TerrainGenerator(ALLEGRO_EVENT_QUEUE *EventQueue, Player *MainPlayer) : m_EventQueue(EventQueue), m_MainPlayer(MainPlayer)
	{
		//m_curBiome = new Biome(1);
		//m_curFractal = new Fractal();
		m_mapSize = 64;
		m_tileSize = 64;
		numWaterTiles = 0;
		numWaterFrames = 5;

		al_init_user_event_source(&m_EnterDungeonEvent);
		al_register_event_source(m_EventQueue, &m_EnterDungeonEvent);
	};
	~TerrainGenerator(){ delete m_map; };

	void generateTerrain(Display &MainDisplay); //Call this from main to generate terrain
	void buildNoiseMap(); //Builds noise map for base terrain generation
	void interpretMap(); //Interprets noise map for base terrain generation
	void generateForests(Display &MainDisplay); //Creates noise map to be interpreted for detail placement
	void analyzeForests(utils::NoiseMap perlinTrees, Display &MainDisplay); //Interprets the detail noise map generated in generateForests
	void draw(); //Draws the fractal (currently not in use)
	void generateTextureMap(); //Main call to generate base textures and create bitmap of terrain
	void testPrint(); //Function to show how the ImageCells class works by example
	void Event_Handler(ALLEGRO_EVENT &EV); //Handles terrain events
	void Draw(bool PrePlayerDraw); //Draws the terrain
	void overlayImage(const cv::Mat &background, const cv::Mat &foreground,
		cv::Mat &output, cv::Point2i location); //Use to overlay an image with another
	void setStartPosition(); //Sets player start position to highest point on terrain

	TerrainMap* Get_Map(){ return m_map; }  //Returns the TerrainMap
	AVec2f GetStartPosition() { return AVec2f(m_StartPosition.x() * m_tileSize, m_StartPosition.y() * m_tileSize); } //Returns the current start position

	void placeTrees(int i, int j, std::vector<std::vector<TerrainTile>> &Layer, std::vector<TerrainLayer> &Layers, ALLEGRO_BITMAP *DecorativeTiles); //Places the details of terrain
	//void generateAnimated(ImageCells &waterSpriteSheet, Mat img, int curRow); //commented out due to Rect conflict with allegro, need workaround
	void setWaterTiles(std::vector<AVec2i> waterTiles); //Not currently in use, to be used when animated water is implemented

private:
	ALLEGRO_EVENT_QUEUE *m_EventQueue;
	ALLEGRO_EVENT_SOURCE m_EnterDungeonEvent;

	ALLEGRO_BITMAP *m_DecorativeTiles; //Forest tile sprite sheet
	ALLEGRO_BITMAP *m_WaterTiles;	//Animated water tile sprite sheet

	utils::NoiseMap m_heightMap; //Base noise map
	noise::module::Perlin m_perlinModule;  //Base perlin module for noise map generation
	TerrainMap *m_map;  //Terrain map which contains all layers, tiles, etc.
	//TerrainTile m_entryTile;
	int m_mapSize;  //Size of map (grid)
	int m_tileSize; //Size of tiles
	int numWaterTiles;  //Water tile count (set after generation)
	int numWaterFrames;  //Num of frames for water tiles to be animated

	AVec2i m_StartPosition; //Start position of player
	Player *m_MainPlayer; //Reference to player object
	SpriteGenerator *m_spriteGenerator; //Sprite generator used for base terrain tile generation

	Biome *m_curBiome; //Current biome (not in use)
	Fractal *m_curFractal; //Curent fractal (not in use)

};

#endif