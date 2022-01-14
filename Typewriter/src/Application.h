#pragma once

#include "EditState.h"

#include "groups/TitlePage.h"
#include "groups/ToggleArray.h"
#include "groups/Page.h"

#include "groups/FileList.h"
#include "groups/SaveAs.h"


class Application
{
private:
	sf::RenderWindow m_window;
	sf::Clock m_clock;
	sf::Font m_font;

	TitlePage m_titlePage;
	ToggleArray m_tools;
	Page m_page;

	FileList m_fileList;
	SaveAs m_saveAs;

	EditState m_state = EditState::None;

	bool m_newProject = true;

	sf::String m_projName = "empty";
	sf::String m_title = "[Title here]";
	sf::String m_subtitle = "By [Author here]";

	const static std::unordered_map<std::string, EditState> s_invstringLUT;
	const static std::unordered_map<EditState, std::string> s_stringLUT;
	const static std::unordered_map<EditState, EditState> s_stateLUT;

private:
	void handleEvent(const sf::Event& event);

	void controlKeys(sf::Keyboard::Key key);

	void mouseClick(const sf::Event& event);
	void keyPressed(const sf::Event& event);
	void textEntered(const sf::Event& event);
	void resized(const sf::Event& event);

	void render();

public:
	static std::string stateToStr(EditState state) { return s_stringLUT.at(state); }
	static EditState strToState(const std::string& str) { return s_invstringLUT.at(str); }

public:
	Application();

	void run();


	friend class Serializer;
};