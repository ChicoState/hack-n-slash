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

//Biome class only contains information needed for the generation of the fractal height map
//as well as information for how to place "noise" within the environment

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

class TerrainGenerator{
public:
	TerrainGenerator(ALLEGRO_EVENT_QUEUE *EventQueue, Player *MainPlayer) : m_EventQueue(EventQueue), m_MainPlayer(MainPlayer)
	{
		//m_curBiome = new Biome(1);
		//m_curFractal = new Fractal();
		m_mapSize = 64;
		m_tileSize = 64;
		time(&start);
		numWaterTiles = 0;
		numWaterFrames = 5;

		al_init_user_event_source(&m_EnterDungeonEvent);
		al_register_event_source(m_EventQueue, &m_EnterDungeonEvent);
	};
	~TerrainGenerator(){ delete m_map; };

	void generateTerrain(Display &MainDisplay);
	void buildNoiseMap();
	void interpretMap();
	void generateForests(Display &MainDisplay);
	void analyzeForests(utils::NoiseMap perlinTrees, Display &MainDisplay);
	void draw();
	void generateTextureMap();
	void testPrint();
	void Event_Handler(ALLEGRO_EVENT &EV);
	void Draw(bool PrePlayerDraw);
	void overlayImage(const cv::Mat &background, const cv::Mat &foreground,
		cv::Mat &output, cv::Point2i location);
	void setStartPosition();

	TerrainMap* Get_Map(){ return m_map; }
	AVec2f GetStartPosition() { return AVec2f(m_StartPosition.x() * m_tileSize, m_StartPosition.y() * m_tileSize); }

	void placeTrees(int i, int j, std::vector<std::vector<TerrainTile>> &Layer, std::vector<TerrainLayer> &Layers, ALLEGRO_BITMAP *DecorativeTiles);
	//void generateAnimated(ImageCells &waterSpriteSheet, Mat img, int curRow); //commented out due to Rect conflict with allegro, need workaround
	void setWaterTiles(std::vector<AVec2i> waterTiles);

private:
	ALLEGRO_EVENT_QUEUE *m_EventQueue;
	ALLEGRO_EVENT_SOURCE m_EnterDungeonEvent;

	ALLEGRO_BITMAP *m_DecorativeTiles;
	ALLEGRO_BITMAP *m_WaterTiles;

	utils::NoiseMap m_heightMap;
	noise::module::Perlin m_perlinModule;
	TerrainMap *m_map;
	//TerrainTile m_entryTile;
	int m_mapSize;
	int m_tileSize;
	int numWaterTiles;
	int numWaterFrames;

	double timeSinceLastUpdate;
	time_t start, end;

	AVec2i m_StartPosition;
	Player *m_MainPlayer;

	Biome *m_curBiome;
	Fractal *m_curFractal;
	SpriteGenerator *m_spriteGenerator;

};

#endif