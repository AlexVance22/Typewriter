#include "PCH.h"
#include "Application.h"

#include "io/Serializer.h"
#include "io/ConvertHTML.h"

#include "Exceptions.h"


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
	switch (event.type)
	{
	case sf::Event::Closed:
		m_window.close();
		break;

	case sf::Event::MouseWheelScrolled:
		m_scrollbar.move(0.f, -16.f * event.mouseWheelScroll.delta);
		break;

	case sf::Event::MouseButtonPressed:
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
		break;

	case sf::Event::KeyPressed:

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
		break;

	case sf::Event::TextEntered:
		m_title = m_titleEditor.getText();
		m_subtitle = m_subEditor.getText();
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
		Serializer::importFile(this);
	}
}

void Application::render()
{
	m_window.clear(sf::Color(20, 20, 40));
	m_window.draw(m_pageBackground);
	m_window.draw(m_titleEditor);
	m_window.draw(m_subEditor);
	m_window.setView(m_scrollbar);
	m_window.draw(m_page);
	m_window.setView(m_default);
	m_window.draw(m_tools);
}


Application::Application()
	: m_window(sf::VideoMode(1920, 1000), "TypeWriter", sf::Style::Default),
	m_tools({ "Scene", "Directorial", "Name", "Parenthetical", "Speech" }, 1920),
	m_titleEditor(sf::FloatRect(20, 100, 425, 50), m_font), m_subEditor(sf::FloatRect(20, 200, 425, 50), m_font),
	m_scrollbar(m_window.getDefaultView()), m_default(m_window.getDefaultView())
{
	ConvertHTML::init();

	sf::Image icon;
	icon.loadFromFile("res/icon.png");
	m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	m_window.setPosition(sf::Vector2i(0, 0));
	m_window.setFramerateLimit(60);

	m_font.loadFromFile("res/couriernew.ttf");
	m_tools.setFont(m_font);

	DivEditor::window = &m_window;
	DivEditor::font = &m_font;
	DivEditor::view = &m_scrollbar;

	m_pageBackground.setPosition(465, 60);
	m_pageBackground.setSize(sf::Vector2f(990, 950));
	m_pageBackground.setFillColor(sf::Color(255, 255, 255));

	render();

	Serializer::importFile(this);
}

void Application::run()
{
	while (m_window.isOpen())
	{
		sf::Event event;

		while (m_window.waitEvent(event))
		{
			m_tools.handleEvent(event);
			m_titleEditor.handleEvent(event);
			m_subEditor.handleEvent(event);
			m_page.handleEvent(event);
			handleEvent(event);

			render();
			m_window.display();
		}
	}
}