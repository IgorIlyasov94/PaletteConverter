#include "ImagePaletteConverter.h"

ImagePaletteConverter::ImagePaletteConverter(const std::string& imageFilePath, const std::string& paletteImageFilePath)
	: imageWidth(0), imageHeight(0), paletteWidth(0), paletteHeight(0), ditheringPower(0.0f)
{
	BMPLoader::Load(imageFilePath, targetImageData, imageWidth, imageHeight);
	BMPLoader::Load(paletteImageFilePath, paletteData, paletteWidth, paletteHeight);
}

ImagePaletteConverter::~ImagePaletteConverter()
{

}

void ImagePaletteConverter::SetDithering(float power)
{
	ditheringPower = power;
}

void ImagePaletteConverter::Convert()
{
	std::random_device randomDevice;

	std::default_random_engine randomEngine(randomDevice());

	size_t imageDataSize = targetImageData.size() / 3;
	size_t blockSize = imageDataSize / THREADS_NUMBER;
	size_t blockExcess = imageDataSize % THREADS_NUMBER;

	std::vector<std::thread> threads;
	threads.reserve(THREADS_NUMBER + blockExcess > 0 ? 1 : 0);

	for (size_t threadId = 0; threadId < THREADS_NUMBER; threadId++)
	{
		size_t imageDataId = threadId * blockSize;
		threads.push_back(std::thread(&ImagePaletteConverter::ThreadFunction, this, ThreadFunctionData{ randomEngine, targetImageData, paletteData, imageDataId, imageDataId + blockSize, ditheringPower }));
	}

	if (blockExcess > 0)
		threads.push_back(std::thread(&ImagePaletteConverter::ThreadFunction, this, ThreadFunctionData{ randomEngine, targetImageData, paletteData, imageDataSize - blockExcess, imageDataSize, ditheringPower }));

	for (auto& thread : threads)
		thread.join();
}

void ImagePaletteConverter::SaveResultToFile(const std::string& imageFilePath)
{
	BMPLoader::Save(imageFilePath, targetImageData, imageWidth, imageHeight);
}

void ImagePaletteConverter::ThreadFunction(ThreadFunctionData data)
{
	for (size_t imagePixelId = data.pixelIdStart; imagePixelId < data.pixelIdEnd; imagePixelId++)
	{
		int imagePixelRId = imagePixelId * 3;
		int imagePixelGId = imagePixelRId + 1;
		int imagePixelBId = imagePixelGId + 1;

		int imagePixelR = data.targetImageData[imagePixelRId];
		int imagePixelG = data.targetImageData[imagePixelGId];
		int imagePixelB = data.targetImageData[imagePixelBId];

		float minRgbDistance = 256.0f;
		size_t closestPaletteColorId = 0;

		for (size_t paletteColorId = 0; paletteColorId < data.paletteData.size() / 3; paletteColorId++)
		{
			size_t paletteColorRId = paletteColorId * 3;
			size_t paletteColorGId = paletteColorRId + 1;
			size_t paletteColorBId = paletteColorGId + 1;

			int paletteColorR = data.paletteData[paletteColorRId];
			int paletteColorG = data.paletteData[paletteColorGId];
			int paletteColorB = data.paletteData[paletteColorBId];

			int rDistance = imagePixelR - paletteColorR;
			int gDistance = imagePixelG - paletteColorG;
			int bDistance = imagePixelB - paletteColorB;

			float rgbDistance = std::sqrt(static_cast<float>(rDistance * rDistance + gDistance * gDistance + bDistance * bDistance));
			rgbDistance *= 1.0f + (Random(data.randomEngine) - 0.5f) * data.ditherPower;

			if (minRgbDistance > rgbDistance)
			{
				minRgbDistance = rgbDistance;

				closestPaletteColorId = paletteColorId;
			}
		}

		size_t paletteColorRId = closestPaletteColorId * 3;
		size_t paletteColorGId = paletteColorRId + 1;
		size_t paletteColorBId = paletteColorGId + 1;

		data.targetImageData[imagePixelRId] = data.paletteData[paletteColorRId];
		data.targetImageData[imagePixelGId] = data.paletteData[paletteColorGId];
		data.targetImageData[imagePixelBId] = data.paletteData[paletteColorBId];
	}
}
