//Created by:		Jesse Herzenach
//Date updated:		10/05/15
//Last update by:	N/A
//Reason for update:N/A

#include <iostream>
#include <iomanip>
#include <fstream>

#include "TerrainGenerator.h"

void TerrainGenerator::generateTerrain(){
	interpretMap(m_curFractal->generateFractal(m_curBiome));
	//m_curFractal->print();
}

void TerrainGenerator::draw(){
	for (int i = 0; i < 129; i++){
		for (int j = 0; j < 129; j++){
			m_curMap[i][j].Draw();
		}
	}
}

void TerrainGenerator::interpretMap(float *heightMap[]){
	int TileSize = 4;
	int *interpretedData[129];
	m_curMap.resize(129, std::vector<TerrainTile>(129, TerrainTile(0, 0, Grass, 0, 0)));
	for (int i = 0; i < 129; i++){
		interpretedData[i] = new int[129];
	}
	std::ofstream mapFile;
	mapFile.open("terrainMap.txt");
	for (int i = 0; i < 129; i++){
		for (int j = 0; j < 129; j++){
			if (heightMap[i][j] >= 40.0f){
				interpretedData[i][j] = 3;
				m_curMap[i][j] = TerrainTile(i * TileSize, j * TileSize, Grass, TileSize, TileSize, true);
			}
			else if (heightMap[i][j] <= 34.0f){
				interpretedData[i][j] = 1;
				m_curMap[i][j] = TerrainTile(i * TileSize, j * TileSize, Water, TileSize, TileSize, true);
			}
			else{
				interpretedData[i][j] = 2;
				m_curMap[i][j] = TerrainTile(i * TileSize, j * TileSize, Dirt, TileSize, TileSize, true);
			}
			mapFile << interpretedData[i][j];
		}
		mapFile << std::endl;
	}
	mapFile.close();

}

float** Fractal::generateFractal(Biome *curBiome){
	initializeHeightmap();
	setInitialConditions(curBiome);
	diamond(m_size - 1, 0, 0, 200);	//calling diamond will then recursively call square->diamond until the entire height map is generated
	print(); //prints raw height map to text file
	return m_heightMap;
}

void Fractal::setInitialConditions(Biome *curBiome){
	//this will contain the initial conditions for each of the primary biomes
	//I will add a conditional to declare additional constants other than the four corner points
	m_heightMap[0][0] = 20.0f;
	m_heightMap[0][m_size - 1] = 20.0f;
	m_heightMap[m_size - 1][0] = 20.0f;
	m_heightMap[m_size - 1][m_size - 1] = 20.0f;
	int X = 0;
	int Y = 0;
	

}

void Fractal::initializeHeightmap(){
	for (int i = 0; i < m_size; i++){
		m_heightMap[i] = new float[m_size];
	}
	for (int i = 0; i < m_size; i++){
		for (int j = 0; j < m_size; j++){
			m_heightMap[i][j] = 0.0f;
		}
	}

}

void Fractal::diamond(int size, int X, int Y, float range){
	if (size <= 1){
		return;
	}
	srand(time(NULL));

	m_heightMap[Y + size / 2][X + size / 2] = (m_heightMap[Y][X] + m_heightMap[Y + size][X] + m_heightMap[Y][X + size] + m_heightMap[Y + size][X + size]) / 4 + (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / range)));
	square(size, X, Y, range);

}

void Fractal::square(int size, int X, int Y, float range){
	//top
	if (size <= 1){
		return;
	}
	if ((Y - 1) < 0){
		m_heightMap[Y][X + size / 2] = (m_heightMap[Y][X] + m_heightMap[Y][X + size] + m_heightMap[Y + size / 2][X + size / 2]) / 3;
	}
	else{
		m_heightMap[Y][X + size / 2] = (m_heightMap[Y][X] + m_heightMap[Y - 1][X + size / 2] + m_heightMap[Y][X + size] + m_heightMap[Y + size / 2][X + size / 2]) / 4;
	}
	//right
	if ((X + size + 1) > m_size - 1){
		m_heightMap[Y + size / 2][X + size] = (m_heightMap[Y][X + size] + m_heightMap[Y + size][X + size] + m_heightMap[Y + size / 2][X + size / 2]) / 3;
	}
	else{
		m_heightMap[Y + size / 2][X + size] = (m_heightMap[Y][X + size] + m_heightMap[Y + size / 2][X + size + 1] + m_heightMap[Y + size][X + size] + m_heightMap[Y + size / 2][X + size / 2]) / 4;
	}
	//bottom
	if ((Y + size + 1) > m_size - 1){
		m_heightMap[Y + size][X + size / 2] = (m_heightMap[Y + size / 2][X + size / 2] + m_heightMap[Y + size][X + size] + m_heightMap[Y + size][X]) / 3;
	}
	else{
		m_heightMap[Y + size][X + size / 2] = (m_heightMap[Y + size / 2][X + size / 2] + m_heightMap[Y + size + 1][X + size / 2] + m_heightMap[Y + size][X + size] + m_heightMap[Y + size][X]) / 4;
	}
	//left
	if ((X - 1) < 0){
		m_heightMap[Y + size / 2][X] = (m_heightMap[Y][X] + m_heightMap[Y + size / 2][X + size / 2] + m_heightMap[Y + size][X]) / 3;
	}
	else{
		m_heightMap[Y + size / 2][X] = (m_heightMap[Y][X] + m_heightMap[Y + size / 2][X + size / 2] + m_heightMap[Y + size][X] + m_heightMap[Y + size / 2][X - 1]) / 4;
	}
	range *= pow(2, -roughness);
	diamond(size / 2, X, Y, range);

	diamond(size / 2, X + size / 2, Y, range);

	diamond(size / 2, X, Y + size / 2, range);

	diamond(size / 2, X + size / 2, Y + size / 2, range);

}

void Fractal::print(){
	std::ofstream mapFile;
	mapFile.open("rawHeight.txt");
	for (int i = 0; i < m_size; i++){
		for (int j = 0; j < m_size; j++){
			mapFile << std::setprecision(2) << std::fixed << m_heightMap[i][j] << " ";
		}
		mapFile << std::endl;
	}
}