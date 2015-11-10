//Created by:		Jesse Herzenach
//Date updated:		10/05/15
//Last update by:	N/A
//Reason for update:N/A

#ifndef __TERRAIN_GENERATOR__
#define __TERRAIN_GENERATOR__

#include "Vec2.h"
#include <vector>

#include "allegro5\allegro.h"
#include "allegro5\allegro_primitives.h"

#include "TerrainTile.h"

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
		roughness = 0.8f; //default value of roughness for testing
	};
	void drawGrayscale();

	//~Fractal();
	float** generateFractal(Biome*); //generate fractal calls setheightconstants and then runs the diamond square algorithm to produce the height map
	void setInitialConditions(Biome*);
	void initializeHeightmap();
	void diamond(int size, int X, int Y, float range);
	void square(int size, int X, int Y, float range);
	void print();
private:
	int m_size = 129; //width and height of the heightmap to be generated
	float *m_heightMap[129];
	float roughness;
};

class TerrainGenerator{
public:
	TerrainGenerator(int level)
	{
		m_curBiome = new Biome(level);
		m_curFractal = new Fractal();
	};
	//~TerrainGenerator();
	void generateTerrain();
	void interpretMap(float*[]);
	void draw();
private:
	std::vector<std::vector<TerrainTile>> m_curMap;
	Biome *m_curBiome;
	Fractal *m_curFractal;
};

#endif