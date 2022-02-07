#pragma once

#include "EditState.h"

#include "AppData.h"

#include "gui/groups/TitlePage.h"
#include "gui/groups/ToggleArray.h"
#include "gui/groups/Page.h"


class Application
{
private:
	sf::RenderWindow m_window;
	sf::Clock m_clock;
	sf::Font m_font;

	TitlePage m_titlePage;
	ToggleArray m_tools;
	Page m_page;

	sf::Text m_alert;

	AppData m_appData;
	Document m_doc;

private:
	void handleEvent(const sf::Event& event);

	void controlKeys(sf::Keyboard::Key key);
	void mouseClick(const sf::Event& event);
	void keyPressed(const sf::Event& event);
	void textEntered(const sf::Event& event);
	void resized(const sf::Event& event);

	void save();
	void imp();
	void exp();

	void changeState(EditState state);

	void render();

public:
	Application();

	void run();
};