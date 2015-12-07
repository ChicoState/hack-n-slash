//Created by:	Ryan Nolan-Hieb

#ifndef __TERRAIN_LAYER__
#define __TERRAIN_LAYER__

#include <vector>


#include "TerrainTile.h"
#include "Display.h"


class TerrainLayer
{
private:
	std::vector<std::vector<TerrainTile>> m_Layer; //All of the tiles that make up a layer

	std::vector<TerrainTile*> m_AnimatedTiles; //A collection of the animated tiles within the layer
	std::vector<TerrainTile*> m_TriggerTiles; //A collection of the tiles that have triggers within the layer

	ALLEGRO_BITMAP *m_LayerImage; //An image of the layer will be created at runtime. So only one image needs to be drawn instead of a individual tiles.
	Display *m_MainDisplay; //Pointer to the primart display

	bool m_PrePlayerDraw; //determines if the layer should be drawn before or after the player
	int m_LayerWidth; //Pixel width of the layer
	int m_LayerHeight; //Pixel height of the layer

public:
	TerrainLayer() : m_LayerImage(NULL), m_LayerWidth(0), m_LayerHeight(0)
	{}
	TerrainLayer(std::vector<std::vector<TerrainTile>> Layer, bool PrePlayerDraw = true) : m_Layer(Layer), m_PrePlayerDraw(PrePlayerDraw), m_LayerImage(NULL)
	{
		m_LayerWidth = m_Layer[0][0].Get_TileWidth() * m_Layer.size();
		m_LayerHeight = m_Layer[0][0].Get_TileHeight() * m_Layer[0].size();
	}
	~TerrainLayer()
	{
		if (m_LayerImage)
		{
			al_destroy_bitmap(m_LayerImage);
		}
	}

	//Basic Getters and Setters

	int Get_LayerWidth(){ return m_LayerWidth; } //return layer width in pixels
	int Get_LayerHeight(){ return m_LayerHeight; } //return layer height in pixels
	int Get_TileSize(){ return m_Layer[0][0].Get_TileWidth(); } //returns the size of one tile
	int Get_PrePlayerDraw(){ return m_PrePlayerDraw; } //returns if the layer should be drawn before or after the player

	TerrainTile& Get_Tile(Vec2i Pos) { return m_Layer[Pos.x()][Pos.y()]; } //returns a reference to the tile at the given index position 
	void Set_Tile(Vec2i Pos, TerrainTile Tile) { m_Layer[Pos.x()][Pos.y()] = Tile; } //Set the tile type of a tile at the given index position

	std::vector<std::vector<TerrainTile>>& Get_VectorLayer() { return m_Layer; } //return the layer in vector form

	//Core Functions

	void Event_Handler(ALLEGRO_EVENT&); //Handles all the events
	void Draw(); //Draws the layer
	void CreateBitmap(Display*); //Creates a bitmap images of all the non-animated tiles in the layer and draws using that.
								 //I figure that must be a lot faster than drawing a bunch of individual tiles
};

#endif