#pragma once


struct File
{
	fs::path path;
	uint32_t type;
};


File openFileName(sf::WindowHandle handle, const char* filter, const std::string& defaultname = "");

File saveFileName(sf::WindowHandle handle, const char* filter, const std::string& defaultname = "");