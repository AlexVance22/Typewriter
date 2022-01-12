#pragma once

class Application;


class Serializer
{
public:
	static bool saveFile(Application* app);
	static bool saveAsFile(Application* app);
	static bool exportFile(Application* app);
	static bool importFile(Application* app);
};