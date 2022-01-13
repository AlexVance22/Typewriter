#include "PCH.h"
#include "ToggleArray.h"


ToggleArray::ToggleArray()
	: m_view(sf::FloatRect(0, 0, 1920, 980))
{
	m_background.setPosition(0, 0);
	m_background.setFillColor(sf::Color(20, 20, 40));
}


void ToggleArray::create(const std::vector<sf::String>& buttons, float width)
{
	m_shapes.resize(buttons.size());
	m_labels.resize(buttons.size());

	m_background.setSize(sf::Vector2f(width, 60));
	float btnwidth = (width - 50) / buttons.size();

	for (size_t i = 0; i < buttons.size(); i++)
	{
		m_shapes[i].setPosition(i * btnwidth + (50 * (i + 1)) / (buttons.size() + 1), 10);
		m_shapes[i].setSize(sf::Vector2f(btnwidth, 40));
		m_shapes[i].setFillColor(sf::Color(255, 255, 255));
		m_shapes[i].setOutlineThickness(2);
		m_shapes[i].setOutlineColor(sf::Color(0, 0, 0));

		m_labels[i].setFillColor(sf::Color(0, 0, 0));
		m_labels[i].setString(buttons[i]);
		m_labels[i].setCharacterSize(20);
	}

	setSelected(0);

	for (size_t i = 0; i < m_labels.size(); i++)
	{
		m_labels[i].setFont(*p_font);

		const sf::Vector2f center = m_shapes[i].getPosition() + m_shapes[i].getSize() * 0.5f;
		const sf::Vector2f textsize = sf::Vector2f(m_labels[i].getGlobalBounds().width, 20);
		const sf::Vector2f pos = center - textsize * 0.5f;
		m_labels[i].setPosition(std::floor(pos.x), std::floor(pos.y));
	}
}


size_t ToggleArray::getSelected() const
{
	return m_selected;
}

void ToggleArray::setSelected(size_t index)
{
	m_selected = index;

	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		if (i == index)
			m_shapes[i].setFillColor(sf::Color(220, 215, 255));
		else
			m_shapes[i].setFillColor(sf::Color(255, 255, 255));
	}
}


bool ToggleArray::contains(const sf::RenderWindow& window, int x, int y) const
{
	const sf::Vector2f pos = window.mapPixelToCoords(sf::Vector2i(x, y), m_view);
	return m_background.getGlobalBounds().contains(pos);
}


void ToggleArray::setEnabled(bool enabled)
{
	m_enabled = enabled;
}

bool ToggleArray::getEnabled() const
{
	return m_enabled;
}


void ToggleArray::handleEvent(const sf::Event& event)
{
	if (m_enabled)
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			for (size_t i = 0; i < m_shapes.size(); i++)
			{
				if (m_shapes[i].getGlobalBounds().contains((float)event.mouseButton.x, (float)event.mouseButton.y))
					setSelected(i);
			}
			break;
		case sf::Event::Resized:
			m_view.reset(sf::FloatRect(0, 0, (float)event.size.width, 980));

			float btnwidth = (float)((event.size.width - 50) / m_shapes.size());

			m_background.setSize(sf::Vector2f((float)event.size.width, 60));

			for (size_t i = 0; i < m_shapes.size(); i++)
			{
				m_shapes[i].setPosition(i * btnwidth + (50 * (i + 1)) / (m_shapes.size() + 1), 10);
				m_shapes[i].setSize(sf::Vector2f(btnwidth, 40));

				const sf::Vector2f center = m_shapes[i].getPosition() + m_shapes[i].getSize() * 0.5f;
				const sf::Vector2f textsize = sf::Vector2f(m_labels[i].getGlobalBounds().width, 20);
				const sf::Vector2f pos = center - textsize * 0.5f;
				m_labels[i].setPosition(floor(pos.x), floor(pos.y));
			}
			break;
		}
	}
}

void ToggleArray::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_enabled)
	{
		target.setView(m_view);

		target.draw(m_background);

		for (const auto& s : m_shapes)
			target.draw(s);

		for (const auto& l : m_labels)
			target.draw(l);
	}
}