#include "PCH.h"
#include "Application.h"

#include "io/Serializer.h"
#include "io/ConvertHTML.h"

#include "Exceptions.h"

#include "gui/Widget.h"


const std::unordered_map<std::string, EditState> Application::s_invstringLUT = {
	{ "None",			EditState::None },
	{ "Scene",			EditState::Scene },
	{ "Directorial",	EditState::Directorial },
	{ "Name",			EditState::Name },
	{ "Parenthetical",	EditState::Parenthetical },
	{ "Speech",			EditState::Speech }
};

const std::unordered_map<EditState, std::string> Application::s_stringLUT = {
	{ EditState::None,			"None" },
	{ EditState::Scene,			"Scene" },
	{ EditState::Directorial,	"Directorial" },
	{ EditState::Name,			"Name" },
	{ EditState::Parenthetical,	"Parenthetical" },
	{ EditState::Speech,		"Speech"}
};

const std::unordered_map<EditState, EditState> Application::s_stateLUT = {
	{ EditState::None,			EditState::Scene },
	{ EditState::Scene,			EditState::Directorial },
	{ EditState::Directorial,	EditState::Name },
	{ EditState::Name,			EditState::Speech },
	{ EditState::Parenthetical,	EditState::Speech },
	{ EditState::Speech,		EditState::Name },
};


void Application::handleEvent(const sf::Event& event)
{
	if (m_fileList.getEnabled())
	{
		m_fileList.handleEvent(event);

		switch (event.type)
		{
		case sf::Event::Closed:
			m_window.close();
			break;
		case sf::Event::Resized:
			m_tools.handleEvent(event);
			m_titlePage.handleEvent(event);
			m_page.handleEvent(event);
			break;
		}
	}
	else
	{
		m_tools.handleEvent(event);
		m_titlePage.handleEvent(event);
		m_page.handleEvent(event);

		switch (event.type)
		{
		case sf::Event::Closed:
			m_window.close();
			break;
		case sf::Event::MouseButtonPressed:
			mouseClick(event);
			break;
		case sf::Event::KeyPressed:
			keyPressed(event);
			break;
		case sf::Event::TextEntered:
			textEntered(event);
			break;
		}
	}
}

void Application::controlKeys(sf::Keyboard::Key key)
{
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		return;

	switch (key)
	{
	case sf::Keyboard::Num1:
		m_tools.setSelected(0);
		m_state = EditState::Scene;
		m_page.setDivisionState(m_state);
		break;
	case sf::Keyboard::Num2:
		m_tools.setSelected(1);
		m_state = EditState::Directorial;
		m_page.setDivisionState(m_state);
		break;
	case sf::Keyboard::Num3:
		m_tools.setSelected(2);
		m_state = EditState::Name;
		m_page.setDivisionState(m_state);
		break;
	case sf::Keyboard::Num4:
		m_tools.setSelected(3);
		m_state = EditState::Parenthetical;
		m_page.setDivisionState(m_state);
		break;
	case sf::Keyboard::Num5:
		m_tools.setSelected(4);
		m_state = EditState::Speech;
		m_page.setDivisionState(m_state);
		break;

	case sf::Keyboard::S:
		if (m_newProject)
			Serializer::saveAsFile(this);
		else
			Serializer::saveFile(this);
		break;
	case sf::Keyboard::E:
		Serializer::exportFile(this);
		break;
	case sf::Keyboard::O:
		m_fileList.setEnabled(true);
		break;

	case sf::Keyboard::F:
		m_titlePage.setEnabled(!m_titlePage.getEnabled());
		break;
	case sf::Keyboard::G:
		m_tools.setEnabled(!m_tools.getEnabled());
		break;
	case sf::Keyboard::H:
		//m_tools.setEnabled(!m_tools.getEnabled());
		break;
	}
}


void Application::mouseClick(const sf::Event& event)
{
	if (m_tools.contains(m_window, event.mouseButton.x, event.mouseButton.y))
	{
		m_titlePage.removeFocus();
		m_page.removeFocus();
	}
	else if (m_titlePage.contains(m_window, event.mouseButton.x, event.mouseButton.y))
		m_page.removeFocus();

	if (m_state != m_page.getActiveState())
	{
		m_state = m_page.getActiveState();
		m_tools.setSelected((size_t)m_state - 1);
	}
	else if (m_state != (EditState)(m_tools.getSelected() + 1))
	{
		m_state = (EditState)(m_tools.getSelected() + 1);
		m_page.setDivisionState((EditState)(m_tools.getSelected() + 1));
	}
}

void Application::keyPressed(const sf::Event& event)
{
	controlKeys(event.key.code);

	switch (event.key.code)
	{
	case sf::Keyboard::Return:
		m_page.addDivision(s_stateLUT.at(m_page.getActiveState()));
		m_state = m_page.getActiveState();
		m_tools.setSelected((size_t)m_state - 1);
		break;
	case sf::Keyboard::Backspace:
		m_page.eraseDivision();
		break;
	}
}

void Application::textEntered(const sf::Event& event)
{
	m_title = m_titlePage.getTitle();
	m_subtitle = m_titlePage.getSubtitle();
}


void Application::render()
{
	m_window.clear(sf::Color(20, 20, 40));
	m_window.draw(m_page);
	m_window.draw(m_titlePage);
	m_window.draw(m_tools);

	m_window.draw(m_fileList);
}


Application::Application()
	: m_window(sf::VideoMode(1920, 980), "TypeWriter", sf::Style::Default)
{
	ConvertHTML::init();

	m_font.loadFromFile("res/couriernew.ttf");

	Widget::p_window = &m_window;
	Widget::p_font = &m_font;

	sf::Image icon;
	icon.loadFromFile("res/icon.png");
	m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	m_window.setPosition(sf::Vector2i(0, 0));
	m_window.setFramerateLimit(60);

	m_fileList.create(this);
	m_tools.create({ "Scene", "Directorial", "Name", "Parenthetical", "Speech" }, 1920);

	render();

	m_titlePage.setEnabled(false);
	m_fileList.setEnabled(true);

	// window size independence --------------------> [ ] 95%
	// clean up serialisation code -----------------> [ ] do with enable and menu popup
	// file io clickable buttons -------------------> [ ]
	// option to delete from project view ----------> [ ] do with enable and menu popup
	// make all side panels into toggleable groups -> [x]
	// -> keyboard only / focus mode ---------------> [x]
	// character list ------------------------------> [ ]
	//
	// ... autocomplete names? regex?

	// what do all groups need:
	// 
	// specific window resize behaviour -> [x]
	// toggle on and logic --------------> [ ]
	// toggle on off display ------------> [ ]
}

void Application::run()
{
	while (m_window.isOpen())
	{
		sf::Event event;

		while (m_window.waitEvent(event))
		{
			handleEvent(event);
			render();
			m_window.display();
		}
	}
}