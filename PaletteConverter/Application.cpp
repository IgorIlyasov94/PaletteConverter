#include "Application.h"

Application::Application()
{

}

Application::~Application()
{

}

int Application::Run()
{
	try
	{
		std::cout << "Select target image file:" << std::endl;

		std::string imageFilePath;

		GetFilePathFromDialog(imageFilePath);

		std::cout << imageFilePath << std::endl << std::endl;
		std::cout << "Select image file with required palette:" << std::endl;

		std::string paletteFilePath;

		GetFilePathFromDialog(paletteFilePath);

		std::cout << paletteFilePath << std::endl << std::endl;

		ImagePaletteConverter paletteConverter(imageFilePath, paletteFilePath);

		float ditheringPower = 0.0f;

		std::cout << "Enter dithering power (0.0 - 1.0):" << std::endl;
		std::cin >> ditheringPower;
		std::cout << std::endl;

		paletteConverter.SetDithering(ditheringPower);

		paletteConverter.Convert();

		std::cout << "Done!" << std::endl << std::endl;
		
		std::string resultImagePath = imageFilePath;
		auto extensionPosition = resultImagePath.find_last_of('.');

		ThrowIfFailed(extensionPosition != std::string::npos, "Application::Run: Incorrect file extension!");

		resultImagePath.insert(extensionPosition, "Modified");

		paletteConverter.SaveResultToFile(resultImagePath);

		std::cout << "File saved:" << std::endl;

		std::cout << resultImagePath << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
