//Created by:		Jesse Herzenach
//Date updated:		10/05/15
//Last update by:	N/A
//Reason for update:N/A

#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>

#include "TerrainGenerator.h"
#include "ImageCells.h"

//This is the main function to call to generate the terrain
void TerrainGenerator::generateTerrain(Display &MainDisplay){
	//interpretMap(m_curFractal->generateFractal(m_curBiome));
	//m_curFractal->print();
	//m_spriteGenerator->generateTexture();
	buildNoiseMap();
	//testPrint();
	generateTextureMap();
	generateForests(MainDisplay);
	setStartPosition();
}

//A function to show the functionality of the ImageCells class in a clear manner.  Teaching by example :)
void TerrainGenerator::testPrint()
{
	ImageCells cells(25, 25, 25, 25); // it creates a ImageCells class having 4 rows and 3 cols, cell witdh = 70 cell height = 90 you can change these values according your needs

	Mat img = Mat(25, 25, CV_8UC3, Scalar(0, 0, 255)); // a test mat to use with cells.setCell important note is : img witdh&height must be same to cell witdh&height

	srand(time(NULL));

	for (int i = 0; i < cells.cols(); i++)
		for (int j = 0; j < cells.rows(); j++)
		{
			cells.setCell(i, j, img); // here you see how to use  setCell
			randu(img, 10 * i, 10 * j); // to show purpose changes img
			imshow("cells.image", cells.image);
			waitKey();
		}

	for (int i = 0; i < cells.cols(); i++)
		for (int j = 0; j < cells.rows(); j++)
		{
			imshow("cells", cells.getCell(i, j)); // here you see how to use  getCell
			waitKey();
		}
}

//currently commented out due to restrictions of the Allegro_Bitmap class
//Meant to generate A sprite sheet of "animated" water by applying blur effects to each frame
/*
void TerrainGenerator::generateAnimated(ImageCells &waterSpriteSheet, Mat img, int curRow)
{
for (int i = 1; i < numWaterFrames; i++)
{
medianBlur(img, img, 1 + (2 * i));
waterSpriteSheet.setCell(i, curRow, img);
}
}
*/

//This is the main function that will generate the landscape bitmap
void TerrainGenerator::generateTextureMap()
{
	int size = 64; //129 129
	ImageCells cells(m_mapSize, m_mapSize, size, size); // it creates a ImageCells class having 4 rows and 3 cols, cell witdh = 70 cell height = 90 you can change these values according your needs
	ImageCells waterSpriteSheet(5, numWaterFrames * 2, size, size);
	Mat imgSize = Mat(size, size, CV_8UC3, Scalar(0, 0, 255)); // a test mat to use with cells.setCell important note is : img witdh&height must be same to cell witdh&height
	Mat img;
	int textureNum = 5;
	m_spriteGenerator->generateWaterTexture(textureNum);
	m_spriteGenerator->generateGrassTexture(textureNum);
	m_spriteGenerator->generateDirtTexture(textureNum);
	srand(time(NULL));
	std::vector<AVec2i> waterTiles; //Vector to keep track of water tiles (only collidable tiles in the base texture map)
	int curRow = 0;
	for (int i = 0; i < m_mapSize; i++)
	{
		for (int j = 0; j < m_mapSize; j++)
		{
			AVec2i Pos(i, j);

			//If tile is water
			if (m_map->Get_Layer(0)->Get_Tile(Pos).Get_TileType() == Water)
			{
				int texNum = rand() % textureNum + 1;
				std::string input = "water";
				input += std::to_string(texNum);
				input += ".bmp";
				img = imread(input, CV_LOAD_IMAGE_COLOR);
				if (!img.empty())
				{
					cv::resize(img, img, imgSize.size());

					//Image to be added to the base texture image that is loaded
					Mat imgAdd = Mat(size, size, CV_8UC3, Scalar(0, (1.0f - fabs(m_heightMap.GetValue(i, j))) * 255.0f, 0));

					//cv::addWeighted(clone, 0.8, imgAdd, 0.2, clone);

					//Adds the raw Mat img with only RGB to the base texture
					cv::addWeighted(img, 0.5, imgAdd, 0.5, 0, img);
					waterTiles.push_back(Pos);
					
					//Another example of water sprite animated generation through the use of filters
					//Commented out due to limitations
					//generateAnimated(waterSpriteSheet, img, curRow);
					/*
					if (curRow < 5){
						waterSpriteSheet.setCell(0, curRow, img);
						waterSpriteSheet.setCell(9, curRow, img);
					for (int i = 1; i < numWaterFrames; i++)
					{
						int erosion_size = 0;
						int erosion_type = MORPH_RECT;
						Mat element = getStructuringElement(erosion_type,
							Size(i * erosion_size + 1, i * erosion_size + 1),
							Point(erosion_size, erosion_size));
						medianBlur(img, img, 1 + (2 * i));
						erode(img, img, element);
						
						waterSpriteSheet.setCell(i, curRow, img);
						waterSpriteSheet.setCell(((numWaterFrames * 2 - 1)) - i, curRow, img);
					}
					curRow++;
					}
					*/
				}
				//Set the image cell to the output image
				cells.setCell(i, j, img);
			}
			//If tile is grass
			else if (m_map->Get_Layer(0)->Get_Tile(Pos).Get_TileType() == Grass)
			{
				int texNum = rand() % textureNum + 1;
				std::string input = "grass";
				input += std::to_string(texNum);
				input += ".bmp";
				img = imread(input, CV_LOAD_IMAGE_COLOR);
				if (!img.empty())
				{
					cv::resize(img, img, imgSize.size());
					Mat imgAdd = Mat(size, size, CV_8UC3, Scalar(0, 0, m_heightMap.GetValue(i, j) * 255.0f));
					//cv::addWeighted(clone, 0.8, imgAdd, 0.2, clone);
					cv::addWeighted(img, 0.8, imgAdd, 0.2, 0, img);
				}
				cells.setCell(i, j, img);
			}

			//If tile is dirt
			else if (m_map->Get_Layer(0)->Get_Tile(Pos).Get_TileType() == Dirt)
			{
				int texNum = rand() % textureNum + 1;
				std::string input = "dirt";
				input += std::to_string(texNum);
				input += ".bmp";

				img = imread(input, CV_LOAD_IMAGE_COLOR);
				if (!img.empty())
				{
					cv::resize(img, img, imgSize.size());
					Mat imgAdd = Mat(size, size, CV_8UC3, Scalar(0, m_heightMap.GetValue(i, j) * 255.0f, m_heightMap.GetValue(i, j) * 255.0f));
					//cv::addWeighted(clone, 0.8, imgAdd, 0.2, clone);
					cv::addWeighted(img, 0.8, imgAdd, 0.2, 0, img);
				}
				cells.setCell(i, j, img);
			}

			// here you see how to use  setCell
			//randu(img, 30 * i, 160 * j); // to show purpose changes img
			//imshow("cells.image", cells.image);


			/*for (int i = 0; i < cells.cols(); i++)
			for (int j = 0; j < cells.rows(); j++)
			{
			imshow("cells", cells.getCell(i, j)); // here you see how to use  getCell
			waitKey();
			}*/


		}
	}
	vector<int> compression_params;
	compression_params.push_back(0);
	compression_params.push_back(0);
	//Writes the complete terrain bitmap image from ImageCells to a jpg
	imwrite("terrain1.jpg", cells.image, compression_params);
	//Sets the terrain image of layer 0 to the outputted jpg
	m_map->Get_Layer(0)->SetTerrainImage();
	

	//imwrite("watersprites.jpg", waterSpriteSheet.image, compression_params);
}

//Builds the noise map for the terrain
//There is a lot going on here, so it is best to refer to libnoise documentation to understand
//http://libnoise.sourceforge.net/tutorials/index.html
void TerrainGenerator::buildNoiseMap(){
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(m_perlinModule);
	heightMapBuilder.SetDestNoiseMap(m_heightMap);
	heightMapBuilder.SetDestSize(m_mapSize, m_mapSize);
	srand(time(NULL));
	float seedValue = rand() % 10000;
	heightMapBuilder.SetBounds(seedValue, seedValue + 4.0f, seedValue - 1.0f, seedValue + 3.0f);

	heightMapBuilder.Build();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(m_heightMap);
	renderer.SetDestImage(image);
	renderer.ClearGradient();
	renderer.AddGradientPoint(-1.0000, utils::Color(0, 0, 128, 255)); // deeps
	renderer.AddGradientPoint(-0.2500, utils::Color(0, 0, 255, 255)); // shallow
	renderer.AddGradientPoint(0.0000, utils::Color(0, 128, 255, 255)); // shore
	renderer.AddGradientPoint(0.0625, utils::Color(240, 240, 64, 255)); // sand
	renderer.AddGradientPoint(0.1250, utils::Color(32, 160, 0, 255)); // grass
	renderer.AddGradientPoint(0.3750, utils::Color(224, 224, 0, 255)); // dirt
	renderer.AddGradientPoint(0.7500, utils::Color(128, 128, 128, 255)); // rock
	renderer.AddGradientPoint(1.0000, utils::Color(0, 0, 0, 255)); // snow
	renderer.Render();

	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("tutorial.bmp");
	writer.WriteDestFile();
	interpretMap();
}

void TerrainGenerator::draw(){
	for (int i = 0; i < m_mapSize; i++){
		for (int j = 0; j < m_mapSize; j++){
			//m_map[i][j].Draw();
		}
	}

	//m_curFractal->drawGrayscale();
}

//generates data necessary to begin placement of forests and details
void TerrainGenerator::generateForests(Display &MainDisplay)
{
	utils::NoiseMap perlinTrees;
	noise::module::Perlin m_perlinTreeModule;
	m_perlinTreeModule.SetOctaveCount(1);
	m_perlinTreeModule.SetFrequency(8);
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(m_perlinTreeModule);
	heightMapBuilder.SetDestNoiseMap(perlinTrees);
	heightMapBuilder.SetDestSize(m_mapSize, m_mapSize);
	srand(time(NULL));
	float seedValue = rand() % 10000;
	heightMapBuilder.SetBounds(seedValue, seedValue + 4.0f, seedValue - 1.0f, seedValue + 3.0f);

	heightMapBuilder.Build();
	analyzeForests(perlinTrees, MainDisplay);
}

//This function takes in a second noise map that is compared to the base map's
//if certain conditions are met from comparing the two, certain objects are placed
void TerrainGenerator::analyzeForests(utils::NoiseMap perlinTrees, Display &MainDisplay){
	std::vector<std::vector<TerrainTile>> Layer;
	int TileSize = 64;
	Layer.resize(m_mapSize, std::vector<TerrainTile>(m_mapSize, TerrainTile(NULL, 0, 0, Blank, TileSize, TileSize)));
	std::vector<std::vector<TerrainTile>> DLayer;
	DLayer.resize(m_mapSize, std::vector<TerrainTile>(m_mapSize, TerrainTile(NULL, 0, 0, Blank, TileSize, TileSize)));
	TerrainLayer *DetailLayer = new TerrainLayer(DLayer, false);
	std::vector<TerrainLayer> Layers;
	Layers.push_back(*DetailLayer);
	ALLEGRO_BITMAP *DecorativeTiles = al_load_bitmap("ForestSprites.png");
	//int size = 64;
	//ImageCells cells(m_mapSize, m_mapSize, size, size); // it creates a ImageCells class having 4 rows and 3 cols, cell witdh = 70 cell height = 90 you can change these values according your needs

	//Mat imgSize = Mat(size, size, CV_8UC4, Scalar(0, 0, 0, 0)); // a test mat to use with cells.setCell important note is : img witdh&height must be same to cell witdh&height
	//Mat img;
	for (int i = 1; i < m_mapSize - 1; i++){
		for (int j = 1; j < m_mapSize - 1; j++){
			//if grassy
			if (m_heightMap.GetValue(i, j) >= 0.1f)
			{
				if (perlinTrees.GetValue(i, j) > 0.94f && perlinTrees.GetValue(i, j) <= 0.96f)
				{
					Layer[i][j] = TerrainTile(DecorativeTiles, i * TileSize, j * TileSize, DungeonEntrance, TileSize, TileSize, false, 9, 6, true, TR_ENTERDUNGEON, m_EventQueue);
				}
				else if (perlinTrees.GetValue(i, j) >= 0.3f && perlinTrees.GetValue(i, j) <= 0.9f)
				{
					placeTrees(i, j, Layer, Layers, DecorativeTiles);
				}
			}
			else if (m_heightMap.GetValue(i, j) > -0.4f)
			{
				if (perlinTrees.GetValue(i, j) > 0.0f && perlinTrees.GetValue(i, j) <= 0.009f)
				{
					Layer[i][j] = TerrainTile(DecorativeTiles, i * TileSize, j * TileSize, DungeonEntrance, TileSize, TileSize, false, 9, 6, true, TR_ENTERDUNGEON, m_EventQueue);
				}
				else if (perlinTrees.GetValue(i, j) > -0.4f && perlinTrees.GetValue(i, j) <= -0.37f)
				{
					//std::string input = "Tree.png";
					//img = imread(input, CV_LOAD_IMAGE_UNCHANGED);
					//m_map[i][j].setRGB(0.0f, 255.0f, 255.0f);
					//AddTree
					//cv::resize(img, img, imgSize.size());
					int numRow;
					int numCol = Random(12, 16);
					if (numCol == 12 || numCol == 13){
						numRow = Random(8, 10);
					}
					else if (numCol == 14 || numCol == 15)
					{
						numRow = 9;
					}
					Layer[i][j] = TerrainTile(DecorativeTiles, i * TileSize, j * TileSize, Wall, TileSize, TileSize, false, numRow, numCol);
					//cells.setCell(i, j, img);
				}
				else if (perlinTrees.GetValue(i, j) > -0.2f && perlinTrees.GetValue(i, j) <= -0.18f)
				{
					int numRow;
					int numCol = Random(0, 4);
					if (numCol == 0)
					{
						numRow = Random(14, 16);
					}
					else if (numCol == 1)
					{
						numCol = 3;
						numRow = Random(14, 16);
					}
					else
					{
						numCol = Random(12, 14);
						numRow = 7;
					}
					Layer[i][j] = TerrainTile(DecorativeTiles, i * TileSize, j * TileSize, Wall, TileSize, TileSize, true, numRow, numCol);
				}
			}
		}
	}
	/*
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(0);
	imwrite("TerrainDecorations.png", cells.image, compression_params);



	cv::Mat foreground = imread("TerrainDecorations.png", CV_LOAD_IMAGE_UNCHANGED);
	cv::Mat background = imread("output.jpg", CV_LOAD_IMAGE_COLOR);
	cv::Mat result;

	overlayImage(background, foreground, result, cv::Point(0, 0));
	imwrite("output.png", result, compression_params);

	//m_map->Get_Layer(0)->SetImage(0);
	*/
	int numRow = 15;
	int numCol = 0;
	for (int i = 0; i < m_mapSize; i++){
		Layer[i][0] = TerrainTile(DecorativeTiles, i * TileSize, 0 * TileSize, Rock, TileSize, TileSize, true, numRow, numCol);
		Layer[0][i] = TerrainTile(DecorativeTiles, 0 * TileSize, i * TileSize, Rock, TileSize, TileSize, true, numRow, numCol);
		Layer[m_mapSize - 1][i] = TerrainTile(DecorativeTiles, (m_mapSize - 1) * TileSize, i * TileSize, Rock, TileSize, TileSize, true, numRow, numCol);
		Layer[i][m_mapSize - 1] = TerrainTile(DecorativeTiles, i * TileSize, (m_mapSize - 1) * TileSize, Rock, TileSize, TileSize, true, numRow, numCol);
	}

	TerrainLayer *Temp = new TerrainLayer(Layer);
	Temp->CreateBitmap(&MainDisplay);

	m_map->AddLayer(Temp, 1);
	for (int i = 0; i < Layers.size(); i++){
		Temp = new TerrainLayer(Layers[i]);
		Temp->CreateBitmap(&MainDisplay);
		m_map->AddLayer(Temp, 0);
	}
}

//Call to set the players start position, currently sets to highest point on the terrain
void TerrainGenerator::setStartPosition()
{
	float highestPoint = -100.0f;
	int x, y;
	for (int i = 1; i < m_mapSize - 1; i++){
		for (int j = 1; j < m_mapSize - 1; j++){
			if (m_heightMap.GetValue(i, j) >= highestPoint){
				highestPoint = m_heightMap.GetValue(i, j);
				x = i;
				y = j;
			}
		}
	}
	m_StartPosition = AVec2i(x, y);
}

//Function to be used to allow overlaying a image with another
void TerrainGenerator::overlayImage(const cv::Mat &background, const cv::Mat &foreground,
	cv::Mat &output, cv::Point2i location)
{
	background.copyTo(output);


	// start at the row indicated by location, or at row 0 if location.y is negative.
	for (int y = std::max(location.y, 0); y < background.rows; ++y)
	{
		int fY = y - location.y; // because of the translation

		// we are done of we have processed all rows of the foreground image.
		if (fY >= foreground.rows)
			break;

		// start at the column indicated by location, 

		// or at column 0 if location.x is negative.
		for (int x = std::max(location.x, 0); x < background.cols; ++x)
		{
			int fX = x - location.x; // because of the translation.

			// we are done with this row if the column is outside of the foreground image.
			if (fX >= foreground.cols)
				break;

			// determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
			double opacity =
				((double)foreground.data[fY * foreground.step + fX * foreground.channels() + 3])

				/ 255.;


			// and now combine the background and foreground pixel, using the opacity, 

			// but only if opacity > 0.
			for (int c = 0; opacity > 0 && c < output.channels(); ++c)
			{
				unsigned char foregroundPx =
					foreground.data[fY * foreground.step + fX * foreground.channels() + c];
				unsigned char backgroundPx =
					background.data[y * background.step + x * background.channels() + c];
				output.data[y*output.step + output.channels()*x + c] =
					backgroundPx * (1. - opacity) + foregroundPx * opacity;
			}
		}
	}
}
void TerrainGenerator::setWaterTiles(std::vector<AVec2i> waterTiles)
{
	//Layer[i][j] = TerrainTile(DecorativeTiles, i * TileSize, j * TileSize, Tree, TileSize, TileSize, true, numRow, numCol);

	int TileSize = 64;
	m_WaterTiles = al_load_bitmap("watersprites.jpg");
	for (int i = 0; i < waterTiles.size(); i++)
	{
		TerrainTile temp = TerrainTile(m_WaterTiles, waterTiles[i].x() * TileSize, waterTiles[i].y() * TileSize, Water, TileSize, TileSize, true, 0, 0, false, TR_NONE, NULL, 1001, true, true, true, 1);
		m_map->Get_Layer(0)->Set_Tile(waterTiles[i], temp);
	}
	//this will iterate through each terrain tile and set to the proper coordinates on the sprite sheet created in generate texture map

}

//Initial interpretation and setting of tile types based on the heightmap values generated in buildNoiseMap
void TerrainGenerator::interpretMap(){
	std::vector<std::vector<TerrainTile> > Layer;
	float waterCutoff = -0.4f;
	int TileSize = 64;
	Layer.resize(m_mapSize, std::vector<TerrainTile>(m_mapSize, TerrainTile(NULL, 0, 0, Grass, TileSize, TileSize)));
	int *interpretedData[64];
	//m_map.resize(m_mapSize, std::vector<TerrainTile>(m_mapSize, TerrainTile(empty, 0, 0, Grass, 0, 0)));
	for (int i = 0; i < m_mapSize; i++)
	{
		interpretedData[i] = new int[m_mapSize];
	}
	std::ofstream mapFile;
	mapFile.open("terrainMap.txt");
	for (int i = 1; i < m_mapSize - 1; i++){
		for (int j = 1; j < m_mapSize - 1; j++){
			if (m_heightMap.GetValue(i, j) <= waterCutoff)
			{
				interpretedData[i][j] = 1;
				Layer[i][j] = TerrainTile(NULL, i * TileSize, j * TileSize, Water, TileSize, TileSize, true);
				numWaterTiles++;
			}
			else if (m_heightMap.GetValue(i, j) >= 0.1f){
				/*int random = rand() % 40;
				if (random == 4){
				interpretedData[i][j] = 2;
				m_map[i][j] = TerrainTile(i * TileSize, j * TileSize, Dirt, TileSize, TileSize, false);
				m_map[i][j].setRGB(0.0f, 0.0f, 0.0f);
				continue;
				}
				else{*/
				interpretedData[i][j] = 3;
				Layer[i][j] = TerrainTile(NULL, i * TileSize, j * TileSize, Grass, TileSize, TileSize, false);
				//}
			}

			else{
				interpretedData[i][j] = 2;
				Layer[i][j] = TerrainTile(NULL, i * TileSize, j * TileSize, Dirt, TileSize, TileSize, false);

			}
			mapFile << interpretedData[i][j];
		}
		mapFile << std::endl;
	}

	mapFile.close();
	TerrainLayer *Temp = new TerrainLayer(Layer);
	m_map = new TerrainMap(m_EventQueue, Temp, m_MainPlayer);

}
//Call to draw grayscale map of fractal
void Fractal::drawGrayscale()
{

	for (int i = 0; i < 64; i++){
		for (int j = 0; j < 64; j++){
			float height = fmod(m_heightMap[i][j] * 4, 255);
			al_draw_filled_rectangle(i * 4, j * 4, (i * 4) + 4, (j * 4) + 4, al_map_rgb(255 - height, 255 - height, 255 - height));
		}
	}

}

//Generates a fractal
float** Fractal::generateFractal(Biome *curBiome){
	initializeHeightmap();
	setInitialConditions(curBiome);
	diamond(m_size - 1, 0, 0, 100.0f);	//calling diamond will then recursively call square->diamond until the entire height map is generated
	calculateAverage();
	print(); //prints raw height map to text file
	return m_heightMap;
}

//Sets initial conditions for fractal (tweak for different results)
void Fractal::setInitialConditions(Biome *curBiome){
	//this will contain the initial conditions for each of the primary biomes
	//I will add a conditional to declare additional constants other than the four corner points
	m_heightMap[0][0] = 20.0f;
	m_heightMap[0][m_size - 1] = 20.0f;
	m_heightMap[m_size - 1][0] = 20.0f;
	m_heightMap[m_size - 1][m_size - 1] = 20.0f;
	int X = 0;
	int Y = 0;
	highest = 20.0f;
	lowest = 20.0f;
	average = 0.0f;
}

//Initializes array for fractal heightmap
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

//Diamond of the diamond square algorithm
//https://en.wikipedia.org/wiki/Diamond-square_algorithm
void Fractal::diamond(int size, int X, int Y, float range){
	if (size <= 1){
		return;
	}
	srand(time(NULL));
	//srand(2);
	m_heightMap[Y + size / 2][X + size / 2] = (m_heightMap[Y][X] + m_heightMap[Y + size][X] + m_heightMap[Y][X + size] + m_heightMap[Y + size][X + size]) / 4 + (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / range)));
	square(size, X, Y, range);

}

//Square of the diamond square algorithm
//https://en.wikipedia.org/wiki/Diamond-square_algorithm
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

//Finds the average height of the fractal's heightmap
void Fractal::calculateAverage()
{
	average = 0;
	for (int i = 0; i < m_size; i++){
		for (int j = 0; j < m_size; j++){
			if (m_heightMap[i][j] > highest){
				highest = m_heightMap[i][j];
			}
			else if (m_heightMap[i][j] < lowest){
				lowest = m_heightMap[i][j];
			}
			average += m_heightMap[i][j];
		}
	}
	average /= (m_size * m_size);
}

//Prints fractal's values to a text file
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

//Event handler for Terrain events
void TerrainGenerator::Event_Handler(ALLEGRO_EVENT &EV)
{
	m_map->Event_Handler(EV);
	if (EV.type == TERRAINTILE_TRIGGER_EVENT)
	{
		if ((TRIGGER)EV.user.data1 == TR_ENTERDUNGEON)
		{
			EV.user.type = CUSTOM_EVENT_ID(ENTER_DUNGEON);
			//m_entryTile = m_map->Get_Layer(0)->Get_Tile(AVec2i(EV.user.data1, EV.user.data2));
			//m_StartPosition = AVec2i(EV.user.data1, EV.user.data2);
			al_emit_user_event(&m_EnterDungeonEvent, &EV, NULL);

		}
	}
}

//Call to draw terrain
void TerrainGenerator::Draw(bool PrePlayerDraw)
{


	m_map->Draw(PrePlayerDraw);

	//al_draw_filled_circle(m_BossPortal.x() * cm_TileSize, m_BossPortal.y() * cm_TileSize, 5, al_map_rgb(255, 255, 255));
	//al_draw_filled_circle(m_BossPortal.x() * cm_TileSize, m_BossPortal.y() * cm_TileSize, 5, al_map_rgb(255, 0, 0));
}

//Places trees and details for grass tiles, called from within the analyzeForests function
void TerrainGenerator::placeTrees(int i, int j, std::vector<std::vector<TerrainTile>> &Layer, std::vector<TerrainLayer> &Layers, ALLEGRO_BITMAP *DecorativeTiles)
{
	//std::string input = "Tree.png";
	//img = imread(input, CV_LOAD_IMAGE_UNCHANGED);
	//m_map[i][j].setRGB(0.0f, 255.0f, 255.0f);
	//AddTree
	//cv::resize(img, img, imgSize.size());
	int TileSize = 64;
	int tree = Random(0, 10);
	int width, height, numRow, numCol;
	if (tree == 0)
	{
		int randomTree = Random(0, 4);
		if (randomTree < 4)
		{

			if (randomTree == 0)
			{
				numCol = 2;
				numRow = 5;
				width = 3;
				height = 6;
			}
			else if (randomTree == 1)
			{
				numCol = 6;
				numRow = 11;
				width = 3;
				height = 6;
			}
			else if (randomTree == 2)
			{
				numCol = 6;
				numRow = 2;
				width = 2;
				height = 3;
			}
			else if (randomTree == 3)
			{
				numCol = 6;
				numRow = 5;
				width = 2;
				height = 3;
			}
			for (int row = 0; row < height; row++){
				for (int col = 0; col < width; col++){
					if (j - row >= 0){
						if (row == 0 && col == 0){
							Layer[i][j] = TerrainTile(DecorativeTiles, i * TileSize, j * TileSize, Tree, TileSize, TileSize, true, numCol, numRow);
						}
						else{
							if (i - col >= 0)
							{
								if (Layers[0].Get_Tile(AVec2i(i - col, j - row)).Get_TileType() != Blank){
									for (int n = 0; n < Layers.size(); n++)
									{
										if (Layers[n].Get_Tile(AVec2i(i - col, j - row)).Get_TileType() == Blank)
										{
											Layers[n].Set_Tile(AVec2i(i - col, j - row), TerrainTile(DecorativeTiles, (i - col) * TileSize, (j - row) * TileSize, Tree, TileSize, TileSize, false, numCol - col, numRow - row));
											break;
										}
										else if (n == Layers.size() - 1)
										{
											std::vector<std::vector<TerrainTile>> TempLayer;
											TempLayer.resize(m_mapSize, std::vector<TerrainTile>(m_mapSize, TerrainTile(NULL, 0, 0, Blank, TileSize, TileSize)));
											TerrainLayer *TempLayerP = new TerrainLayer(TempLayer, false);
											TempLayerP->Set_Tile(AVec2i(i - col, j - row), TerrainTile(DecorativeTiles, (i - col) * TileSize, (j - row) * TileSize, Tree, TileSize, TileSize, false, numCol - col, numRow - row));
											Layers.push_back(*TempLayerP);
											break;
										}
									}

								}
								else{
									Layers[0].Set_Tile(AVec2i(i - col, j - row), TerrainTile(DecorativeTiles, (i - col) * TileSize, (j - row) * TileSize, Tree, TileSize, TileSize, false, numCol - col, numRow - row));
								}
							}
							if (i + col < m_mapSize && col != 0)
							{
								if (Layers[0].Get_Tile(AVec2i(i + col, j - row)).Get_TileType() != Blank){
									for (int n = 0; n < Layers.size(); n++)
									{
										if (Layers[n].Get_Tile(AVec2i(i + col, j - row)).Get_TileType() == Blank)
										{
											Layers[n].Set_Tile(AVec2i(i + col, j - row), TerrainTile(DecorativeTiles, (i + col) * TileSize, (j - row) * TileSize, Tree, TileSize, TileSize, false, numCol + col, numRow - row));
											break;
										}
										else if (n == Layers.size() - 1)
										{
											std::vector<std::vector<TerrainTile>> TempLayer;
											TempLayer.resize(m_mapSize, std::vector<TerrainTile>(m_mapSize, TerrainTile(NULL, 0, 0, Blank, TileSize, TileSize)));
											TerrainLayer *TempLayerP = new TerrainLayer(TempLayer, false);
											TempLayerP->Set_Tile(AVec2i(i - col, j - row), TerrainTile(DecorativeTiles, (i + col) * TileSize, (j - row) * TileSize, Tree, TileSize, TileSize, false, numCol + col, numRow - row));
											Layers.push_back(*TempLayerP);
											break;
										}
									}

								}
								else{
									Layers[0].Set_Tile(AVec2i(i + col, j - row), TerrainTile(DecorativeTiles, (i + col) * TileSize, (j - row) * TileSize, Tree, TileSize, TileSize, false, numCol + col, numRow - row));
								}
							}
						}
					}
				}
			}
		}
	}
		else
		{
			int rock = Random(0, 20);
			if (rock == 1)
			{
				int numRow = Random(14, 16);
				int numCol = 0;
				Layer[i][j] = TerrainTile(DecorativeTiles, i * TileSize, j * TileSize, Rock, TileSize, TileSize, true, numRow, numCol);
			}
			else
			{
				int numRow = Random(10, 14);
				int numCol = Random(0, 4);
				Layer[i][j] = TerrainTile(DecorativeTiles, i * TileSize, j * TileSize, Shrub, TileSize, TileSize, false, numRow, numCol);
			}
		}
	
	//cells.setCell(i, j, img);
}
