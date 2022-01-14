#include "PCH.h"
#include "Button.h"


Button::Button()
{
	m_background.setFillColor(sf::Color(65, 65, 90));

	m_text.setCharacterSize(20);
	m_text.setFillColor(sf::Color(255, 255, 255));
}


void Button::create(sf::FloatRect bounds, const sf::String& text)
{
	m_bounds = bounds;

	m_background.setPosition(m_bounds.left, m_bounds.top);
	m_background.setSize(sf::Vector2f(m_bounds.width, m_bounds.height));

	m_text.setString(text);
	m_text.setFont(*p_font);

	const sf::Vector2f center(m_bounds.left + m_bounds.width * 0.5f, m_bounds.top + m_bounds.height * 0.5f);
	const sf::FloatRect textbounds = m_text.getGlobalBounds();

	m_text.setPosition(std::floor(center.x - textbounds.width * 0.5f), std::floor(center.y - textbounds.height * 0.5f - 5.f));
}


bool Button::getClick() const
{
	return m_clicked;
}


void Button::handleEvent(const sf::Event& event)
{
	m_clicked = false;

	if (m_hovered)
		m_background.setFillColor(sf::Color(80, 80, 110));
	else
		m_background.setFillColor(sf::Color(65, 65, 90));

	switch (event.type)
	{
	case sf::Event::MouseButtonPressed:
		if (m_bounds.contains(translate(event.mouseButton.x, event.mouseButton.y)))
		{
			m_background.setFillColor(sf::Color(90, 90, 120));
			m_clicked = true;
		}
		break;
	case sf::Event::MouseMoved:
		m_hovered = m_bounds.contains(translate(event.mouseMove.x, event.mouseMove.y));
		break;
	}
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background);
	target.draw(m_text);
}