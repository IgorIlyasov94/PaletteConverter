#include "BMPLoader.h"

void BMPLoader::Load(const std::string& filePath, std::vector<uint8_t>& rgbDataBuffer, int& width, int& height)
{
	std::ifstream textureFile{ filePath, std::ios::binary | std::ios::in };

	if (!textureFile.is_open())
		throw std::exception("LoadTextureBMP: File loading has failed!");

	textureFile >> std::noskipws;

	BMPHeader bmpHeader{};
	BMPInfoHeader bmpInfoHeader{};

	if (!textureFile.read(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPHeader)))
		throw std::exception("BMPLoader: Wrong file structure!");

	if (bmpHeader.bmpSignature[0] != 'B' || bmpHeader.bmpSignature[1] != 'M')
		throw std::exception("BMPLoader: Wrong file structure!");

	if (!textureFile.read(reinterpret_cast<char*>(&bmpInfoHeader), sizeof(BMPInfoHeader)))
		throw std::exception("BMPLoader: Wrong file structure!");

	auto rgbDataBufferSize = bmpHeader.bmpFileSize - bmpInfoHeader.bmpInfoHeaderSize;

	std::copy(std::istream_iterator<uint8_t>(textureFile), std::istream_iterator<uint8_t>(), std::back_inserter(rgbDataBuffer));

	width = bmpInfoHeader.width;
	height = bmpInfoHeader.height;
}

void BMPLoader::Save(const std::string& filePath, std::vector<uint8_t>& rgbDataBuffer, int width, int height)
{
	std::ofstream textureFile{ filePath, std::ios::binary | std::ios::out };

	auto paddingSize = 4 - (width * 3) % 4;
	auto rgbDataBufferSize = rgbDataBuffer.size();

	BMPHeader bmpHeader{};
	bmpHeader.bmpSignature[0] = 'B';
	bmpHeader.bmpSignature[1] = 'M';
	bmpHeader.bmpFileSize = BMP_HEADER_SIZE + rgbDataBufferSize + paddingSize * height;
	bmpHeader.dataOffset = BMP_HEADER_SIZE;

	textureFile.write(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPHeader));

	BMPInfoHeader bmpInfoHeader{};
	bmpInfoHeader.bmpInfoHeaderSize = sizeof(BMPInfoHeader);
	bmpInfoHeader.width = width;
	bmpInfoHeader.height = height;
	bmpInfoHeader.colorPlanesNumber = 1;
	bmpInfoHeader.colorDepth = 24;
	bmpInfoHeader.bmpRawDataSize = rgbDataBufferSize + paddingSize * height;
	bmpInfoHeader.horizontalResolution = 3750;
	bmpInfoHeader.verticalResolution = 3750;

	textureFile.write(reinterpret_cast<char*>(&bmpInfoHeader), sizeof(BMPInfoHeader));

	std::vector<uint8_t> rgbPaddedDataBuffer;

	AddPadding(rgbDataBuffer, rgbPaddedDataBuffer, width);

	textureFile.write(reinterpret_cast<char*>(&rgbPaddedDataBuffer[0]), rgbPaddedDataBuffer.size());
}

BMPLoader::BMPLoader()
{

}

BMPLoader::~BMPLoader()
{

}

void BMPLoader::AddPadding(std::vector<uint8_t>& rgbDataBuffer, std::vector<uint8_t>& rgbPaddedDataBuffer, int width)
{
	auto paddingSize = (4 - (width * 3) % 4) % 4;
	auto fullSize = width * 3 + paddingSize;

	auto beginIterator = conditional_iterator<std::vector<uint8_t>, NSkipper>(rgbDataBuffer.begin(), NSkipper(fullSize, paddingSize));
	auto endIterator = conditional_iterator<std::vector<uint8_t>, NSkipper>(rgbDataBuffer.end(), NSkipper(fullSize, paddingSize));

	std::copy(beginIterator, endIterator, variable_back_inserter(rgbPaddedDataBuffer, NSkipper(fullSize, paddingSize), 128));

	for (auto paddingByteId = 0; paddingByteId < paddingSize; paddingByteId++)
		rgbPaddedDataBuffer.push_back(0);
}
