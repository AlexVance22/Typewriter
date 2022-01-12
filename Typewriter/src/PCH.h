#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <filesystem>
#include <exception>
#include <memory>

#include <unordered_map>
#include <string>
#include <vector>
#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

extern "C"
{
#include <wkhtmltox/pdf.h>
#include <wkhtmltox/image.h>
}

#include <Windows.h>

namespace fs = std::filesystem;