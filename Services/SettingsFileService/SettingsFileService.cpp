#include "SettingsFileService.hpp"


SettingsFileService::SettingsFileService(std::string string)
{
	this->_settingsFile = new std::fstream(string, std::ios_base::in);
}

SettingsFileService::~SettingsFileService()
{
	if (this->_settingsFile->is_open()) { this->_settingsFile->close(); }
	delete _settingsFile;
}

