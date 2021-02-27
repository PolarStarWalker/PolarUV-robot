#include "ConfigurationFileService.hpp"


ConfigurationFileService::ConfigurationFileService(std::string string)
{
	this->_settingsFile = new std::fstream(string, std::ios_base::in);
}

ConfigurationFileService::~ConfigurationFileService()
{
	this->_settingsFile->close();
	delete _settingsFile;
}