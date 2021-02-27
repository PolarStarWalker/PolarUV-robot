#pragma once
#include <fstream>

class SettingsFileService
{
private:
	std::fstream* _settingsFile;

public:

	SettingsFileService(std::string string);
	

	~SettingsFileService();

};

