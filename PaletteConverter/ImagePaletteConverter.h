#pragma once

#include "BMPLoader.h"

class ImagePaletteConverter
{
public:
	ImagePaletteConverter(const std::string& imageFilePath, const std::string& paletteImageFilePath);
	~ImagePaletteConverter();

	void Convert();
	void SaveResultToFile(const std::string& imageFilePath);

private:
	ImagePaletteConverter() = delete;

	std::vector<uint8_t> targetImageData;
	std::vector<uint8_t> paletteData;

	int imageWidth;
	int imageHeight;
	int paletteWidth;
	int paletteHeight;
};
