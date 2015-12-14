#ifndef __SPRITE_GENERATOR__
#define __SPRITE_GENERATOR__

#include <noise.h>

#include "noiseutils.h"

using namespace noise;

class SpriteGenerator{
public:
	SpriteGenerator();
	// Height of the texture.
	static const int TEXTURE_HEIGHT = 64;

	void generateWaterTexture(int textureNum);
	void generateGrassTexture(int textureNum);
	void generateDirtTexture(int textureNum);
	// Creates the color gradients for the texture.
	void CreateTextureColor(utils::RendererImage& renderer);

	// Given a noise module, this function renders a flat square texture map and
	// writes it to a Windows bitmap (*.bmp) file.  Because the texture map is
	// square, its width is equal to its height.  The texture map can be seamless
	// (tileable) or non-seamless.
	void CreatePlanarTexture(const module::Module& noiseModule, bool seamless,
		int height, const char* filename);

	// Given a noise module, this function renders a spherical texture map and
	// writes it to a Windows bitmap (*.bmp) file.  The texture map's width is
	// double its height.
	void CreateSphericalTexture(const module::Module& noiseModule, int height,
		const char* filename);

	// Given a noise map, this function renders a texture map and writes it to a
	// Windows bitmap (*.bmp) file.
	void RenderTexture(const utils::NoiseMap& noiseMap, const char* filename);

};

#endif