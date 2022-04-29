#pragma once

#include "BMPLoader.h"
#include <thread>



class ImagePaletteConverter
{
public:
	ImagePaletteConverter(const std::string& imageFilePath, const std::string& paletteImageFilePath);
	~ImagePaletteConverter();

	void SetDithering(float power);
	void Convert();
	void SaveResultToFile(const std::string& imageFilePath);

private:
	ImagePaletteConverter() = delete;

	using ThreadFunctionData = struct
	{
		std::default_random_engine randomEngine;
		std::vector<uint8_t>& targetImageData;
		const std::vector<uint8_t>& paletteData;
		size_t pixelIdStart;
		size_t pixelIdEnd;
		float ditherPower;
	};

	void ThreadFunction(ThreadFunctionData threadFunctionData);

	std::vector<uint8_t> targetImageData;
	std::vector<uint8_t> paletteData;

	int imageWidth;
	int imageHeight;
	int paletteWidth;
	int paletteHeight;

	float ditheringPower;

	const size_t THREADS_NUMBER = std::thread::hardware_concurrency();
};
