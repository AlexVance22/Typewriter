#pragma once

class Page;


bool inHTML(Page& page, sf::String& title, sf::String& subtitle, const char* infile);

bool outHTML(const Page& page, const sf::String& title, const sf::String& subtitle, const fs::path& outfile, bool standalone = false);