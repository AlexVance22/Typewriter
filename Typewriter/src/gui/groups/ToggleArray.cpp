#include "PCH.h"
#include "ToggleArray.h"


ToggleButton::ToggleButton()
{
	m_background.setFillColor(sf::Color(255, 255, 255));

	m_text.setCharacterSize(20);
	m_text.setFillColor(sf::Color(0, 0, 0));
}


void ToggleButton::create(sf::FloatRect bounds, const sf::String& text)
{
	m_bounds = bounds;

	m_background.setPosition(m_bounds.left, m_bounds.top);
	m_background.setSize(sf::Vector2f(m_bounds.width, m_bounds.height));

	m_text.setString(text);
	m_text.setFont(*p_font);

	const sf::Vector2f center(m_bounds.left + m_bounds.width * 0.5f, m_bounds.top + m_bounds.height * 0.5f);
	const sf::FloatRect textbounds = m_text.getGlobalBounds();

	m_text.setPosition(std::floor(center.x - textbounds.width * 0.5f), std::floor(center.y - textbounds.height * 0.5f - 5));
}


void ToggleButton::resize(sf::FloatRect bounds)
{
	m_bounds = bounds;

	m_background.setPosition(m_bounds.left, m_bounds.top);
	m_background.setSize(sf::Vector2f(m_bounds.width, m_bounds.height));

	const sf::Vector2f center(m_bounds.left + m_bounds.width * 0.5f, m_bounds.top + m_bounds.height * 0.5f);
	const sf::FloatRect textbounds = m_text.getGlobalBounds();

	m_text.setPosition(std::floor(center.x - textbounds.width * 0.5f), std::floor(center.y - textbounds.height * 0.5f - 5));
}


void ToggleButton::setToggled(bool toggled)
{
	m_toggled = toggled;

	if (toggled)
		m_background.setFillColor(sf::Color(255, 200, 255));
	else
		m_background.setFillColor(sf::Color(255, 255, 255));
}

bool ToggleButton::isToggled() const
{
	return m_toggled;
}


void ToggleButton::handleEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::MouseButtonPressed:
		m_toggled = m_bounds.contains(translate(event.mouseButton.x, event.mouseButton.y));

		if (m_toggled)
			m_background.setFillColor(sf::Color(255, 200, 255));
		else
			m_background.setFillColor(sf::Color(255, 255, 255));
		break;
	}
}

void ToggleButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background);
	target.draw(m_text);
}


ToggleArray::ToggleArray() : Group(sf::FloatRect(0, 0, 1920, 980))
{
	m_background.setPosition(0, 0);
	m_background.setFillColor(sf::Color(20, 20, 40));
}


void ToggleArray::create(const std::vector<sf::String>& buttons, float width)
{
	m_options.resize(buttons.size());

	m_background.setSize(sf::Vector2f(width, 60));
	float btnwidth = (width - 50) / buttons.size();

	for (size_t i = 0; i < buttons.size(); i++)
	{
		m_options[i].create(sf::FloatRect(i * btnwidth + (50 * (i + 1)) / (buttons.size() + 1), 10, btnwidth, 40), buttons[i]);
		m_options[i].setView(m_view);

		//m_shapes[i].setPosition(i * btnwidth + (50 * (i + 1)) / (buttons.size() + 1), 10);
		//m_shapes[i].setSize(sf::Vector2f(btnwidth, 40));
		//m_shapes[i].setFillColor(sf::Color(255, 255, 255));
		//m_shapes[i].setOutlineThickness(2);
		//m_shapes[i].setOutlineColor(sf::Color(0, 0, 0));

		//m_labels[i].setFillColor(sf::Color(0, 0, 0));
		//m_labels[i].setString(buttons[i]);
		//m_labels[i].setCharacterSize(20);
	}

	setSelected(0);

	/*
	for (size_t i = 0; i < m_labels.size(); i++)
	{
		m_labels[i].setFont(*p_font);

		const sf::Vector2f center = m_shapes[i].getPosition() + m_shapes[i].getSize() * 0.5f;
		const sf::Vector2f textsize = sf::Vector2f(m_labels[i].getGlobalBounds().width, 20);
		const sf::Vector2f pos = center - textsize * 0.5f;
		m_labels[i].setPosition(std::floor(pos.x), std::floor(pos.y));
	}
	*/
}


size_t ToggleArray::getSelected() const
{
	return m_selected;
}

void ToggleArray::setSelected(size_t index)
{
	m_options[m_selected].setToggled(false);

	m_selected = index;

	m_options[m_selected].setToggled(true);
}


void ToggleArray::removeFocus()
{

}


void ToggleArray::handleEvent(const sf::Event& event)
{
	if (m_enableUpdate || event.type == sf::Event::Resized)
	{
		for (auto& op : m_options)
			op.handleEvent(event);

		switch (event.type)
		{
		case sf::Event::MouseButtonPressed: {
			bool found = false;
			for (size_t i = 0; i < m_options.size(); i++)
			{
				if (m_options[i].isToggled())
				{
					setSelected(i);
					found = true;
				}
				if (!found)
					m_options[m_selected].setToggled(true);
			}
			break; }
		case sf::Event::Resized:
			m_view.reset(sf::FloatRect(0, 0, (float)event.size.width, (float)event.size.height));

			float btnwidth = (float)((event.size.width - 50) / m_options.size());

			m_background.setSize(sf::Vector2f((float)event.size.width, 60));

			for (size_t i = 0; i < m_options.size(); i++)
			{
				m_options[i].resize(sf::FloatRect(
					i * btnwidth + (50 * (i + 1)) / (m_options.size() + 1), 10,
					btnwidth, 40
				));
			}
			break;
		}
	}
}

void ToggleArray::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_enableDisplay)
	{
		target.setView(m_view);

		target.draw(m_background);

		for (const auto& op : m_options)
			target.draw(op);
	}
}