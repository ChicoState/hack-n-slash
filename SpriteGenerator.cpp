#include <iostream>
#include "SpriteGenerator.h"
#include <time.h>

using namespace noise;

void SpriteGenerator::generateWaterTexture(int textureNum)
{
	srand(time(NULL));
	for (int i = 1; i <= textureNum; i++){
		float seedValue = rand() % 10000;
		// Primary jade texture.  The ridges from the ridged-multifractal module
		// produces the veins.
		//std::cout << "enter generate texture" << std::endl;
		module::RidgedMulti primaryJade;
		primaryJade.SetSeed(seedValue + 2);
		primaryJade.SetFrequency(2.0);
		primaryJade.SetLacunarity(2.20703125);
		primaryJade.SetOctaveCount(6);
		primaryJade.SetNoiseQuality(QUALITY_STD);

		// Base of the secondary jade texture.  The base texture uses concentric
		// cylinders aligned on the z axis, which will eventually be perturbed.
		module::Cylinders baseSecondaryJade;
		baseSecondaryJade.SetFrequency(2.0);

		// Rotate the base secondary jade texture so that the cylinders are not
		// aligned with any axis.  This produces more variation in the secondary
		// jade texture since the texture is parallel to the y-axis.
		module::RotatePoint rotatedBaseSecondaryJade;
		rotatedBaseSecondaryJade.SetSourceModule(0, baseSecondaryJade);
		rotatedBaseSecondaryJade.SetAngles(90.0, 25.0, 5.0);

		// Slightly perturb the secondary jade texture for more realism.
		module::Turbulence perturbedBaseSecondaryJade;
		perturbedBaseSecondaryJade.SetSourceModule(0, rotatedBaseSecondaryJade);
		perturbedBaseSecondaryJade.SetSeed(seedValue);
		perturbedBaseSecondaryJade.SetFrequency(4.0);
		perturbedBaseSecondaryJade.SetPower(1.0 / 4.0);
		perturbedBaseSecondaryJade.SetRoughness(4);

		// Scale the secondary jade texture so it contributes a small part to the
		// final jade texture.
		module::ScaleBias secondaryJade;
		secondaryJade.SetSourceModule(0, perturbedBaseSecondaryJade);
		secondaryJade.SetScale(0.25);
		secondaryJade.SetBias(0.0);

		// Add the two jade textures together.  These two textures were produced
		// using different combinations of coherent noise, so the final texture will
		// have a lot of variation.
		module::Add combinedJade;
		combinedJade.SetSourceModule(0, primaryJade);
		combinedJade.SetSourceModule(1, secondaryJade);

		// Finally, perturb the combined jade textures to produce the final jade
		// texture.  A low roughness produces nice veins.
		module::Turbulence finalJade;
		finalJade.SetSourceModule(0, combinedJade);
		finalJade.SetSeed(seedValue + 1);
		finalJade.SetFrequency(4.0);
		finalJade.SetPower(1.0 / 16.0);
		finalJade.SetRoughness(20);

		// Given the jade noise module, create a non-seamless texture map, a
		// seamless texture map, and a spherical texture map.
		/*CreatePlanarTexture(finalJade, false, TEXTURE_HEIGHT,
			"textureplane.bmp");*/
		std::string outputFile = "water";
		outputFile += std::to_string(i);
		outputFile += ".bmp";
		const char* output = outputFile.c_str();
		CreatePlanarTexture(finalJade, true, TEXTURE_HEIGHT,
			output);
		/*CreateSphericalTexture(finalJade, TEXTURE_HEIGHT,
			"texturesphere.bmp");*/
		//std::cout << "after print" << std::endl;
	}
}

void SpriteGenerator::generateGrassTexture(int textureNum){
	srand(time(NULL));
	for (int i = 1; i <= textureNum; i++){
		module::RidgedMulti mountainTerrain;
		module::Billow baseFlatTerrain;
		baseFlatTerrain.SetFrequency(2.0);

		module::ScaleBias flatTerrain;
		flatTerrain.SetSourceModule(0, baseFlatTerrain);
		flatTerrain.SetScale(0.125);
		flatTerrain.SetBias(-0.75);

		utils::NoiseMap heightMap;
		utils::NoiseMapBuilderPlane heightMapBuilder;

		heightMapBuilder.SetSourceModule(flatTerrain);
		heightMapBuilder.SetDestNoiseMap(heightMap);
		heightMapBuilder.SetDestSize(TEXTURE_HEIGHT, TEXTURE_HEIGHT);

		float seedValue = rand() % 10000;
		heightMapBuilder.SetBounds(seedValue, seedValue + 4.0f, seedValue - 1.0f, seedValue + 3.0f);
		//heightMapBuilder.SetBounds(6.0, 10.0, 1.0, 5.0);
		heightMapBuilder.Build();

		utils::RendererImage renderer;
		utils::Image image;
		renderer.SetSourceNoiseMap(heightMap);
		renderer.SetDestImage(image);
		renderer.ClearGradient();
		renderer.AddGradientPoint(-1.00, utils::Color(32, 160, 0, 255)); // grass
		renderer.AddGradientPoint(-0.25, utils::Color(224, 224, 0, 255)); // dirt
		renderer.AddGradientPoint(0.25, utils::Color(128, 128, 128, 255)); // rock
		renderer.AddGradientPoint(1.00, utils::Color(255, 255, 255, 255)); // snow
		renderer.EnableLight();
		renderer.SetLightContrast(3.0);
		renderer.SetLightBrightness(2.0);
		renderer.Render();

		utils::WriterBMP writer;
		writer.SetSourceImage(image);
		std::string outputFile = "grass";
		outputFile += std::to_string(i);
		outputFile += ".bmp";
		const char* output = outputFile.c_str();
		writer.SetDestFilename(output);
		writer.WriteDestFile();
	}

}

void SpriteGenerator::generateDirtTexture(int textureNum){
	srand(time(NULL));
	for (int i = 1; i <= textureNum; i++){
		module::RidgedMulti mountainTerrain;
		module::Billow baseFlatTerrain;
		baseFlatTerrain.SetFrequency(2.0);

		module::ScaleBias flatTerrain;
		flatTerrain.SetSourceModule(0, baseFlatTerrain);
		flatTerrain.SetScale(0.125);
		flatTerrain.SetBias(-0.75);

		utils::NoiseMap heightMap;
		utils::NoiseMapBuilderPlane heightMapBuilder;

		heightMapBuilder.SetSourceModule(flatTerrain);
		heightMapBuilder.SetDestNoiseMap(heightMap);
		heightMapBuilder.SetDestSize(TEXTURE_HEIGHT, TEXTURE_HEIGHT);

		float seedValue = rand() % 10000;
		heightMapBuilder.SetBounds(seedValue, seedValue + 4.0f, seedValue - 1.0f, seedValue + 3.0f);
		//heightMapBuilder.SetBounds(6.0, 10.0, 1.0, 5.0);
		heightMapBuilder.Build();

		utils::RendererImage renderer;
		utils::Image image;
		renderer.SetSourceNoiseMap(heightMap);
		renderer.SetDestImage(image);
		renderer.ClearGradient();
		renderer.AddGradientPoint(-1.00, utils::Color(145, 115, 0, 255)); // grass
		renderer.AddGradientPoint(-0.25, utils::Color(179, 157, 0, 255)); // dirt
		renderer.AddGradientPoint(0.25, utils::Color(179, 157, 0, 255)); // rock
		renderer.AddGradientPoint(1.00, utils::Color(202, 187, 0, 255)); // snow
		renderer.EnableLight();
		renderer.SetLightContrast(3.0);
		renderer.SetLightBrightness(2.0);
		renderer.Render();

		utils::WriterBMP writer;
		writer.SetSourceImage(image);
		std::string outputFile = "dirt";
		outputFile += std::to_string(i);
		outputFile += ".bmp";
		const char* output = outputFile.c_str();
		writer.SetDestFilename(output);
		writer.WriteDestFile();
	}
}

void SpriteGenerator::CreateTextureColor(utils::RendererImage& renderer)
{
	// Create a nice jade palette.
	//std::cout << "create texture color enter" << std::endl;
	renderer.ClearGradient();
	renderer.AddGradientPoint(-1.000, utils::Color(10, 160, 255, 255));
	renderer.AddGradientPoint(0.000, utils::Color(0, 170, 255, 255));
	renderer.AddGradientPoint(0.250, utils::Color(0, 130, 255, 255));
	renderer.AddGradientPoint(0.375, utils::Color(0, 100, 255, 255));
	renderer.AddGradientPoint(1.000, utils::Color(0, 120, 255, 255));

	//std::cout << "end of texture color create" << std::endl;
}

void SpriteGenerator::CreatePlanarTexture(const module::Module& noiseModule, bool seamless,
	int height, const char* filename)
{
	// Map the output values from the noise module onto a plane.  This will
	// create a two-dimensional noise map which can be rendered as a flat
	// texture map.
	//std::cout << "create planar texture enter" << std::endl;
	utils::NoiseMapBuilderPlane plane;
	utils::NoiseMap noiseMap;
	plane.SetBounds(-1.0, 1.0, -1.0, 1.0);
	plane.SetDestSize(height, height);
	plane.SetSourceModule(noiseModule);
	plane.SetDestNoiseMap(noiseMap);
	plane.EnableSeamless(seamless);
	plane.Build();

	RenderTexture(noiseMap, filename);
}

void SpriteGenerator::CreateSphericalTexture(const module::Module& noiseModule, int height,
	const char* filename)
{
	// Map the output values from the noise module onto a sphere.  This will
	// create a two-dimensional noise map which can be rendered as a spherical
	// texture map.
	utils::NoiseMapBuilderSphere sphere;
	utils::NoiseMap noiseMap;
	sphere.SetBounds(-90.0, 90.0, -180.0, 180.0); // degrees
	sphere.SetDestSize(height * 2, height);
	sphere.SetSourceModule(noiseModule);
	sphere.SetDestNoiseMap(noiseMap);
	sphere.Build();

	RenderTexture(noiseMap, filename);
}

void SpriteGenerator::RenderTexture(const utils::NoiseMap& noiseMap, const char* filename)
{
	// Create the color gradients for the texture.
	utils::RendererImage textureRenderer;
	CreateTextureColor(textureRenderer);

	// Set up us the texture renderer and pass the noise map to it.
	utils::Image destTexture;
	textureRenderer.SetSourceNoiseMap(noiseMap);
	textureRenderer.SetDestImage(destTexture);
	textureRenderer.EnableLight(false);

	// Render the texture.
	textureRenderer.Render();

	// Write the texture as a Windows bitmap file (*.bmp).
	utils::WriterBMP textureWriter;
	textureWriter.SetSourceImage(destTexture);
	textureWriter.SetDestFilename(filename);
	textureWriter.WriteDestFile();
}