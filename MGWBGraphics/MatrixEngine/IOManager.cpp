#include "IOManager.h"
#include <fstream>
#include "Errors.h"
namespace MatrixEngine
{
	bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
	{
		std::ifstream file(filePath, std::ios::binary);
		if (file.fail())
		{
			perror(filePath.c_str());
			fatalError("File " + filePath + " could not be loaded.");
			return false;
		}

		// seek to the end of the file
		file.seekg(0, std::ios::end);
		// get the filesize in bytes
		int fileSize = file.tellg();
		// go back to the beginning of the file
		file.seekg(0, std::ios::beg);
		// reduce the file size by the possible header bytes
		fileSize -= file.tellg();

		buffer.resize(fileSize);

		file.read((char*)&(buffer[0]), fileSize);
		file.close();

		return true;
	}
}