#pragma once


struct File
{
	fs::path path;
	uint32_t type;
};


File openFileName(HWND handle, const char* filter, const std::string& defaultname = "");

File saveFileName(HWND handle, const char* filter, const std::string& defaultname = "");