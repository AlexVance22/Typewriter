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
	const bool editmode = !(m_fileList.getEnabled() || m_saveAs.getEnabled());

	m_tools.setUpdateEnabled(editmode);
	m_titlePage.setUpdateEnabled(editmode);
	m_page.setUpdateEnabled(editmode);

	m_tools.handleEvent(event);
	m_titlePage.handleEvent(event);
	m_page.handleEvent(event);

	m_fileList.handleEvent(event);
	m_saveAs.handleEvent(event);

	switch (event.type)
	{
	case sf::Event::Closed:
		if (m_saved)
			m_window.close();
		break;
	case sf::Event::Resized:
		resized(event);
		break;
	}
	
	if (editmode)
	{
		switch (event.type)
		{
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
		m_saved = false;
		break;
	case sf::Keyboard::Num2:
		m_tools.setSelected(1);
		m_state = EditState::Directorial;
		m_page.setDivisionState(m_state);
		m_saved = false;
		break;
	case sf::Keyboard::Num3:
		m_tools.setSelected(2);
		m_state = EditState::Name;
		m_page.setDivisionState(m_state);
		m_saved = false;
		break;
	case sf::Keyboard::Num4:
		m_tools.setSelected(3);
		m_state = EditState::Parenthetical;
		m_page.setDivisionState(m_state);
		m_saved = false;
		break;
	case sf::Keyboard::Num5:
		m_tools.setSelected(4);
		m_state = EditState::Speech;
		m_page.setDivisionState(m_state);
		m_saved = false;
		break;

	case sf::Keyboard::S:
		if (m_newProject)
			m_saveAs.setEnabled(true);
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
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
	{
		if (m_titlePage.isFocused())
		{
			m_title = m_titlePage.getTitle();
			m_subtitle = m_titlePage.getSubtitle();
			m_saved = false;
		}
		if (m_page.isFocused())
			m_saved = false;
	}
}

void Application::resized(const sf::Event& event)
{
	if (event.size.width < 1010)
		m_window.setSize(sf::Vector2u(1010, m_window.getSize().y));
}


void Application::render()
{
	m_window.clear(sf::Color(20, 20, 40));
	m_window.draw(m_page);
	if (!m_saved)
		m_window.draw(m_alert);
	m_window.draw(m_titlePage);
	m_window.draw(m_tools);

	if (m_fileList.getEnabled() || m_saveAs.getEnabled())
	{
		sf::RectangleShape grey;
		grey.setSize((sf::Vector2f)m_window.getSize());
		grey.setPosition(0, 0);
		grey.setFillColor(sf::Color(100, 100, 100, 100));

		m_window.setView(sf::View((sf::Vector2f)m_window.getSize() * 0.5f, (sf::Vector2f)m_window.getSize()));
		m_window.draw(grey);
	}

	m_window.draw(m_fileList);
	m_window.draw(m_saveAs);
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
	ShowWindow(m_window.getSystemHandle(), SW_MAXIMIZE);

	m_alert.setCharacterSize(20);
	m_alert.setFont(m_font);
	m_alert.setString("Unsaved");
	m_alert.setStyle(sf::Text::Italic);
	m_alert.setPosition(1650, 100);

	m_fileList.create(this);
	m_saveAs.create(this);
	m_titlePage.create();
	m_tools.create({ "Scene", "Directorial", "Name", "Parenthetical", "Speech" }, 1920);

	m_page.addDivision(EditState::Scene);
	m_state = EditState::Scene;
	m_titlePage.setTitle("[Title here]");
	m_titlePage.setSubtitle("By [Author here]");

	m_titlePage.setEnabled(false);
	m_saveAs.setEnabled(false);
	m_fileList.setEnabled(true);

	// window size independence --------------------> [x]
	// clean up serialisation code -----------------> [x] (better)
	// file io clickable buttons -------------------> [ ]
	// option to delete from project view ----------> [ ]
	// make all side panels into toggleable groups -> [x]
	// -> keyboard only / focus mode ---------------> [x]
	// blurry text bug -----------------------------> [ ]
	// unsaved warning -----------------------------> [ ]
	// copy cut paste ------------------------------> [x]
	// character list ------------------------------> [ ]
	//
	// ... autocomplete names? regex?
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