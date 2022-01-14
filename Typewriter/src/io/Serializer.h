#pragma once

class Application;


class Serializer
{
public:
	static bool saveFile(Application* app);
	static bool saveAsFile(Application* app, const std::string& outfile);
	static bool exportFile(Application* app);
	static bool importFile(Application* app, const std::string& infile = "");
};