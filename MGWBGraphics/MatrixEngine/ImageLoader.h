#pragma once
#include "GLTexture.h"
#include <string>
namespace MatrixEngine
{
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};
}

