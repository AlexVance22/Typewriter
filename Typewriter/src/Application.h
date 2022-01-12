#pragma once

#include "EditState.h"

#include "gui/ToggleArray.h"
#include "gui/EditBox.h"
#include "gui/Page.h"


class Application
{
private:
	sf::RenderWindow m_window;
	sf::Clock m_clock;
	sf::Font m_font;

	sf::RectangleShape m_pageBackground;
	ToggleArray m_tools;
	EditBox m_titleEditor;
	EditBox m_subEditor;
	Page m_page;

	EditState m_state = EditState::None;

	sf::View m_scrollbar;
	sf::View m_default;

	bool m_newProject = true;
	bool m_boot = true;

	sf::String m_projName = "empty";
	sf::String m_title = "";
	sf::String m_subtitle = "";

	const static std::unordered_map<std::string, EditState> s_invstringLUT;
	const static std::unordered_map<EditState, std::string> s_stringLUT;
	const static std::unordered_map<EditState, EditState> s_stateLUT;

private:
	void handleEvent(const sf::Event& event);

	void controlKeys(sf::Keyboard::Key key);

	void render();

public:
	static std::string stateToStr(EditState state) { return s_stringLUT.at(state); }
	static EditState strToState(const std::string& str) { return s_invstringLUT.at(str); }

public:
	Application();

	void run();


	friend class Serializer;
};