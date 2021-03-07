#include "SettingsFileService.hpp"


SettingsFileService::~SettingsFileService()
{

}

FileText SettingsFileService::ReadFile()
{
	FileText fileText;

	std::fstream file(this->_fileName.c_str(), std::ios_base::in);

	file.seekg(0, file.end);
	fileText.FileLength = file.tellg();
	file.seekg(0, file.beg);

	fileText.FileText = new char[fileText.FileLength];
	file.read(fileText.FileText, fileText.FileLength);

	file.close();

	return fileText;
}


SettingsStruct* SettingsFileService::CreateSettingsStruct(FileText fileText) {
	SettingsStruct* settingsStruct = new SettingsStruct;

	std::cout << fileText.FileText;

	return settingsStruct;
}

SettingsStruct* SettingsFileService::GetSettings() {
	return CreateSettingsStruct(ReadFile());
}