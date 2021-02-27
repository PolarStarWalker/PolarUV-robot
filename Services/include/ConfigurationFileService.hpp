#pragma once
#include <fstream>

class ConfigurationFileService
{
private:
	std::fstream* _settingsFile;

public:

	ConfigurationFileService(std::string string);
	

	~ConfigurationFileService();

};

