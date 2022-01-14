#include "PCH.h"
#include "FileList.h"

#include "io/Serializer.h"


void FileList::refreshFiles()
{
	m_filenames.clear();
	m_filenames.emplace_back("empty");
	for (const auto& entry : fs::directory_iterator("user"))
	{
		const std::string stem = entry.path().stem().generic_string();
		if (stem != "empty")
			m_filenames.push_back(stem);
	}

	m_background.setPosition(700, 300);
	m_background.setSize(sf::Vector2f(520, 200.f + 40.f * m_filenames.size()));
}

void FileList::refreshButtons()
{
	m_buttons.clear();
	Button& e = m_buttons.emplace_back();
	e.create(sf::FloatRect(720, 320, 480, 40), "Empty");
	e.setView(m_view);

	for (size_t i = 1; i < m_filenames.size(); i++)
	{
		Button& b = m_buttons.emplace_back();
		b.create(sf::FloatRect(720, 340.f + 40.f * i, 480, 40), m_filenames[i]);
		b.setView(m_view);
	}

	m_confirm.create(sf::FloatRect(720, 450 + m_buttons.size() * 40, 200, 40), "Open");
	m_confirm.setView(m_view);
	m_delete.create(sf::FloatRect(1000, 450 + m_buttons.size() * 40, 200, 40), "Delete");
	m_delete.setView(m_view);
}


FileList::FileList() : Group(sf::FloatRect(0, 0, 1920, 1000))
{
	refreshFiles();

	m_background.setPosition(700, 300);
	m_background.setFillColor(sf::Color(40, 40, 60));
	m_background.setOutlineThickness(3);
	m_background.setOutlineColor(sf::Color(65, 65, 90));
}


void FileList::create(Application* parent)
{
	p_parent = parent;

	refreshButtons();
}


void FileList::setEnabled(bool enabled)
{
	m_enableUpdate = enabled;
	m_enableDisplay = enabled;

	if (enabled)
	{
		refreshFiles();
		refreshButtons();
	}
}


void FileList::removeFocus()
{

}


void FileList::handleEvent(const sf::Event& event)
{
	if (m_enableUpdate || event.type == sf::Event::Resized)
	{
		for (auto& button : m_buttons)
			button.handleEvent(event);
		m_confirm.handleEvent(event);
		m_delete.handleEvent(event);

		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			for (size_t i = 0; i < m_buttons.size(); i++)
			{
				if (m_buttons[i].getClick())
					m_selected = i;
			}
			if (m_confirm.getClick())
			{
				if (m_selected == 0)
					Serializer::importFile(p_parent);
				else
					Serializer::importFile(p_parent, m_filenames[m_selected]);

				setEnabled(false);
			}
			if (m_delete.getClick())
			{

			}
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				setEnabled(false);
			break;

		case sf::Event::Resized:
			m_view.setCenter(960, (float)event.size.height * 0.5f);
			m_view.setSize((float)event.size.width, (float)event.size.height);
			break;
		}
	}
}

void FileList::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_enableDisplay)
	{
		target.setView(m_view);

		target.draw(m_background);

		for (const auto& button : m_buttons)
			target.draw(button);

		target.draw(m_confirm);
		target.draw(m_delete);
	}
}