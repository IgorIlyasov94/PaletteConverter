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

	for (size_t imagePixelId = 0; imagePixelId < targetImageData.size() / 3; imagePixelId++)
	{
		int imagePixelRId = imagePixelId * 3;
		int imagePixelGId = imagePixelRId + 1;
		int imagePixelBId = imagePixelGId + 1;

		int imagePixelR = targetImageData[imagePixelRId];
		int imagePixelG = targetImageData[imagePixelGId];
		int imagePixelB = targetImageData[imagePixelBId];

		float minRgbDistance = 256.0f;
		size_t closestPaletteColorId = 0;

		for (size_t paletteColorId = 0; paletteColorId < paletteData.size() / 3; paletteColorId++)
		{
			size_t paletteColorRId = paletteColorId * 3;
			size_t paletteColorGId = paletteColorRId + 1;
			size_t paletteColorBId = paletteColorGId + 1;

			int paletteColorR = paletteData[paletteColorRId];
			int paletteColorG = paletteData[paletteColorGId];
			int paletteColorB = paletteData[paletteColorBId];

			int rDistance = imagePixelR - paletteColorR;
			int gDistance = imagePixelG - paletteColorG;
			int bDistance = imagePixelB - paletteColorB;

			float rgbDistance = std::sqrt(static_cast<float>(rDistance * rDistance + gDistance * gDistance + bDistance * bDistance));
			rgbDistance *= 1.0f + (Random(randomEngine) - 0.5f) * ditheringPower;

			if (minRgbDistance > rgbDistance)
			{
				minRgbDistance = rgbDistance;

				closestPaletteColorId = paletteColorId;
			}
		}

		size_t paletteColorRId = closestPaletteColorId * 3;
		size_t paletteColorGId = paletteColorRId + 1;
		size_t paletteColorBId = paletteColorGId + 1;

		targetImageData[imagePixelRId] = paletteData[paletteColorRId];
		targetImageData[imagePixelGId] = paletteData[paletteColorGId];
		targetImageData[imagePixelBId] = paletteData[paletteColorBId];
	}
}

void ImagePaletteConverter::SaveResultToFile(const std::string& imageFilePath)
{
	BMPLoader::Save(imageFilePath, targetImageData, imageWidth, imageHeight);
}
