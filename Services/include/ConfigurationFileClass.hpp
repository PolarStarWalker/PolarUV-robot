#pragma once
#include <fstream>

class ConfigurationFileClass
{
private:
	std::fstream* _settingsFile;

public:

	ConfigurationFileClass(std::string string);
	

	~ConfigurationFileClass();

};

