#include "PCH.h"
#include "SaveAs.h"

#include "io/Serializer.h"


SaveAs::SaveAs() : Group(sf::FloatRect(0, 0, 1920, 980))
{
	m_background.setPosition(700, 300);
	m_background.setSize(sf::Vector2f(520, 200));
	m_background.setFillColor(sf::Color(40, 40, 60));
	m_background.setOutlineThickness(3);
	m_background.setOutlineColor(sf::Color(10, 10, 15));

	m_nameBox.setView(m_view);
	m_confirm.setView(m_view);
}


void SaveAs::create(Application* parent)
{
	m_nameBox.create(sf::FloatRect(720, 320, 480, 50));
	m_nameBox.setFocused(true);

	m_confirm.create(sf::FloatRect(800, 400, 320, 40), "Save As");

	p_parent = parent;
}


void SaveAs::setEnabled(bool enabled)
{
	m_enableUpdate = enabled;
	m_enableDisplay = enabled;

	if (enabled)
		m_nameBox.setText("");
}


void SaveAs::removeFocus()
{

}


void SaveAs::handleEvent(const sf::Event& event)
{
	if (m_enableUpdate || event.type == sf::Event::Resized)
	{
		m_nameBox.handleEvent(event);
		m_confirm.handleEvent(event);

		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			if (m_confirm.getClick())
			{
				if (m_nameBox.getText() == "")
					break;

				Serializer::saveAsFile(p_parent, m_nameBox.getText());

				setEnabled(false);
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

void SaveAs::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_enableDisplay)
	{
		target.setView(m_view);

		target.draw(m_background);

		target.draw(m_nameBox);
		target.draw(m_confirm);
	}
}