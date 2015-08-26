#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

namespace MatrixEngine
{
	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(::std::string texturePath)
	{
		auto mit = _textureMap.find(texturePath);

		if (mit == _textureMap.end())
		{
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);
			_textureMap.insert(make_pair(texturePath, newTexture));
			std::cout << "Loaded texture.\n";
			return newTexture;
		}
		//std::cout << "Used cached texture.\n";
		return mit->second;
	}
}