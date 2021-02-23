#include "ConfigurationFileClass.hpp"


ConfigurationFileClass::ConfigurationFileClass(std::string string)
{
	this->_settingsFile = new std::fstream(string, std::ios_base::in);
}

ConfigurationFileClass::~ConfigurationFileClass()
{
	this->_settingsFile->close();
	delete _settingsFile;
}