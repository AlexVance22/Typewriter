#include "PCH.h"
#include "Application.h"

#include "io/Serializer.h"
#include "io/Dialogs.h"

#include "gui/popups/FileList.h"
#include "gui/popups/SaveAs.h"
#include "gui/popups/ClosingCheck.h"

#include "Exceptions.h"


constexpr size_t stateToIdx(EditState state)
{
	return static_cast<size_t>(state) - 1;
}

constexpr EditState idxToState(size_t state)
{
	return static_cast<EditState>(state + 1);
}


void Application::handleEvent(const sf::Event& event)
{

	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (m_tools.getEnabled() && m_tools.contains(m_window, event.mouseButton.x, event.mouseButton.y))
		{
			m_page.removeFocus();
			m_titlePage.removeFocus();
			m_tools.handleEvent(event);
		}
		else if (m_titlePage.getEnabled() && m_titlePage.contains(m_window, event.mouseButton.x, event.mouseButton.y))
		{
			m_titlePage.handleEvent(event);
			m_page.removeFocus();
		}
		else
		{
			m_page.handleEvent(event);
			m_titlePage.removeFocus();
		}
	}
	else
	{
		m_tools.handleEvent(event);
		m_titlePage.handleEvent(event);
		m_page.handleEvent(event);
	}


	switch (event.type)
	{
	case sf::Event::Closed:
		switch (closeCheckPopup(m_window))
		{
		case CloseMessage::SaveClose:
			save();
		case CloseMessage::LoseClose:
			m_window.close();
			break;
		case CloseMessage::Cancel:
			break;
		}
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
	case sf::Event::Resized:
		resized(event);
		break;
	}
}


void Application::controlKeys(sf::Keyboard::Key key)
{
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
		return;

	switch (key)
	{
	case sf::Keyboard::Num1:
		changeState(EditState::Scene);
		break;
	case sf::Keyboard::Num2:
		changeState(EditState::Directorial);
		break;
	case sf::Keyboard::Num3:
		changeState(EditState::Name);
		break;
	case sf::Keyboard::Num4:
		changeState(EditState::Parenthetical);
		break;
	case sf::Keyboard::Num5:
		changeState(EditState::Speech);
		break;

	case sf::Keyboard::S:
		save();
		break;
	case sf::Keyboard::E:
		save();
		exp();
		break;
	case sf::Keyboard::O:
		save();
		imp();
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
	if (m_appData.editstate != m_page.getActiveState())
	{
		m_appData.editstate = m_page.getActiveState();
		m_tools.setSelected(stateToIdx(m_appData.editstate));
	}
	else if (m_appData.editstate != idxToState(m_tools.getSelected()))
	{
		m_appData.editstate = idxToState(m_tools.getSelected());
		m_page.setDivisionState(idxToState(m_tools.getSelected()));
	}
}

void Application::keyPressed(const sf::Event& event)
{
	controlKeys(event.key.code);

	switch (event.key.code)
	{
	case sf::Keyboard::Return:
		m_page.addDivision(StateOps::StateToNext(m_page.getActiveState()));
		m_appData.editstate = m_page.getActiveState();
		m_tools.setSelected((size_t)m_appData.editstate - 1);
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
		if (m_appData.savestate != SaveState::New)
			m_appData.savestate = SaveState::Unsaved;
	}
}

void Application::resized(const sf::Event& event)
{
	if (event.size.width < 1010)
		m_window.setSize(sf::Vector2u(1010, m_window.getSize().y));
}


void Application::save()
{
	if (m_appData.savestate == SaveState::New)
	{
		const std::string temp = saveAsPopup(m_window);
		if (temp == "")
			return;
		m_appData.filename = temp;
		m_appData.filepath = "user/" + temp;
	}

	m_doc.title = m_titlePage.getTitle();
	m_doc.subtitle = m_titlePage.getSubtitle();
	m_doc.content.clear();
	for (const auto& div : m_page)
		m_doc.content.emplace_back(div.getText(), div.getState());

	saveFile(m_appData, m_doc);
}

void Application::imp()
{
	m_appData.filename = fileListPopup(m_window);
	m_appData.filepath = "user/" + m_appData.filename;
	importFile(m_appData, m_doc);
	m_titlePage.setTitle(m_doc.title);
	m_titlePage.setSubtitle(m_doc.subtitle);
	m_page.clear();
	for (const auto& div : m_doc.content)
	{
		m_page.addDivision(div.state);
		m_page.getActive().setText(div.text);
	}
}

void Application::exp()
{
	exportFile(m_appData, m_doc, saveFileName(m_window.getSystemHandle(),
		"PDF Document (*.pdf)\0*.pdf\0HTML Document (*.html + *.css)\0*.html;*css\0", m_appData.filename));
}


void Application::changeState(EditState state)
{
	m_tools.setSelected(stateToIdx(state));
	m_appData.editstate = state;
	m_page.setDivisionState(m_appData.editstate);
	if (m_appData.savestate != SaveState::New)
		m_appData.savestate = SaveState::Unsaved;
}


void Application::render()
{
	m_window.clear(sf::Color(20, 20, 40));
	m_window.draw(m_page);
	if (m_appData.savestate != SaveState::Saved)
		m_window.draw(m_alert);
	m_window.draw(m_titlePage);
	m_window.draw(m_tools);

	/*
	sf::RectangleShape grey;
	grey.setSize((sf::Vector2f)m_window.getSize());
	grey.setPosition(0, 0);
	grey.setFillColor(sf::Color(100, 100, 100, 100));

	m_window.setView(sf::View((sf::Vector2f)m_window.getSize() * 0.5f, (sf::Vector2f)m_window.getSize()));
	m_window.draw(grey);
	*/
}


Application::Application()
	: m_window(sf::VideoMode(1920, 980), "TypeWriter", sf::Style::Default)
{
	wkhtmltopdf_init(0);

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

	m_tools.create({ "Scene", "Directorial", "Name", "Parenthetical", "Speech" }, 1920);
	m_tools.setEnabled(true);

	m_page.addDivision(EditState::Scene);
	m_appData.editstate = EditState::Scene;

	m_titlePage.create();
	m_titlePage.setTitle("[Title here]");
	m_titlePage.setSubtitle("By [Author here]");
	m_titlePage.setEnabled(true);

	imp();

	// window size independence --------------------> [x]
	// clean up serialisation code -----------------> [x] (better)
	// file io clickable buttons -------------------> [ ]
	// option to delete from project view ----------> [ ]
	// make all side panels into toggleable groups -> [x]
	// -> keyboard only / focus mode ---------------> [x]
	// blurry text bug -----------------------------> [ ]
	// unsaved warning -----------------------------> [x] (ugly)
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