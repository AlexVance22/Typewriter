#include "PCH.h"
#include "Button.h"


Button::Button(sf::FloatRect bounds, const sf::String& text, const sf::Font& font)
{
	m_bounds = bounds;

	m_shape.setPosition(m_bounds.left, m_bounds.top);
	m_shape.setSize(sf::Vector2f(m_bounds.width, m_bounds.height));
	m_shape.setFillColor(sf::Color(65, 65, 90));

	m_text.setString(text);
	m_text.setFont(font);
	m_text.setCharacterSize(20);
	m_text.setFillColor(sf::Color(255, 255, 255));

	const sf::Vector2f center(m_bounds.left + m_bounds.width * 0.5f, m_bounds.top + m_bounds.height * 0.5f);
	const sf::FloatRect textbounds = m_text.getGlobalBounds();

	m_text.setPosition(std::floor(center.x - textbounds.width * 0.5f), std::floor(center.y - textbounds.height * 0.5f - 5));
}


bool Button::mouseClick(int x, int y)
{
	if (m_bounds.contains((float)x, (float)y))
	{
		m_shape.setFillColor(sf::Color(90, 90, 120));
		return true;
	}
	return false;
}

void Button::mouseMove(int x, int y)
{
	if (m_bounds.contains((float)x, (float)y))
		m_shape.setFillColor(sf::Color(80, 80, 110));
	else
		m_shape.setFillColor(sf::Color(65, 65, 90));
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_shape);
	target.draw(m_text);
}