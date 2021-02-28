#include "SettingsFileService.hpp"

SettingsFileService::SettingsFileService()
{
	this->_fileName = "server.properties";
}

SettingsFileService::~SettingsFileService()
{
	delete[] this->_fileText;
	delete _settingsStruct;
}

bool SettingsFileService::ReadFile()
{
	delete[] this->_fileText;

	this->_file.open(this->_fileName.c_str(), std::ios_base::in);

	this->_file.seekg(0, this->_file.end);
	this->_fileLength = this->_file.tellg();
	this->_file.seekg(0, this->_file.beg);

	this->_fileText = new char[this->_fileLength];
	this->_file.read(this->_fileText, this->_fileLength);

	this->_file.close();

	for (size_t i = 0; i < this->_fileLength; i++) {
		if()

	}

}