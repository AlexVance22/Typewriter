#include "PCH.h"
#include "EditBox.h"


void EditBox::setCursorToMouse(int x, int y)
{
	const sf::Vector2i relative = sf::Vector2i(x, y - 2) - (sf::Vector2i)m_text.getPosition();

	constexpr float width = 20.f * 0.6f;
	constexpr float height = 20.f + 3.f;

	const size_t charindex = (relative.x > m_string.getSize() * width) ? m_string.getSize() : (size_t)(relative.x / width + 0.5f);

	const sf::Vector2f displaypos(charindex * width - 1, 4);
	m_cursor.setPosition(m_text.getPosition() + displaypos);

	m_cursorPos = std::min(charindex, m_string.getSize());
}

void EditBox::updateSelection()
{
	if (m_selection[0] == m_selection[1])
	{
		m_selected.clear();
		m_highlight.setSize(sf::Vector2f(0, 20));
		return;
	}

	if (m_selection[0] > m_selection[1])
		std::swap(m_selection[0], m_selection[1]);

	m_selected = m_string.substring(m_selection[0], m_selection[1] - m_selection[0]);
	updateHighlight(m_selection[0], m_selection[1]);
}
void EditBox::updateHighlight(size_t start, size_t end)
{
	if (start == end)
	{
		m_highlight.setSize(sf::Vector2f(0, 20));
		return;
	}
	if (end < start)
	{
		const size_t buf = start;
		start = end;
		end = buf;
	}

	m_highlight.setPosition(m_text.getPosition() + sf::Vector2f(start * 20.f * 0.6f, 3.f));
	m_highlight.setSize(sf::Vector2f((end - start) * 20.f * 0.6f, 20.f));
}

void EditBox::enterChar(uint32_t character)
{
	if (m_focused && !m_held)
	{
		if (character < 0x20 && character != 0x8) // whitespace
			return;

		if (m_selection[0] != m_selection[1])
		{
			m_string.erase(m_selection[0], m_selection[1] - m_selection[0]);
			m_cursorPos = m_selection[0];
			m_selection[1] = m_cursorPos;
			updateSelection();

			setCursorPosition(m_cursorPos);

			if (character == 0x8) //backspace
			{
				m_text.setString(m_string);
				return;
			}
		}

		if (character == 0x8) // backspace
		{
			if (m_string.getSize() == 0)
				return;

			m_cursorPos--;
			m_string.erase(m_cursorPos, 1);
			m_text.setString(m_string);

			setCursorPosition(m_cursorPos);
		}
		else
		{
			m_string.insert(m_cursorPos, character);
			m_cursorPos++;
			m_text.setString(m_string);

			setCursorPosition(m_cursorPos);
		}

		m_selection[0] = m_cursorPos;
		m_selection[1] = m_cursorPos;
	}
}
void EditBox::keyPress(sf::Keyboard::Key key)
{
	if (m_focused)
	{
		switch (key)
		{
		case sf::Keyboard::Left:
			if (m_cursorPos > 0)
			{
				m_cursorPos--;
				setCursorPosition(m_cursorPos);
			}
			break;

		case sf::Keyboard::Right:
			if (m_cursorPos < m_string.getSize())
			{
				m_cursorPos++;
				setCursorPosition(m_cursorPos);
			}
			break;
		}
	}
}
void EditBox::mouseClick(int x, int y, sf::Mouse::Button button)
{
	if (button == sf::Mouse::Left)
	{
		const sf::Vector2f pos = translate(x, y);

		m_held = true;
		if (m_bounds.contains(pos))
		{
			setFocused(true);

			setCursorToMouse(pos.x, pos.y);
			m_selection[0] = m_cursorPos;
			m_selection[1] = m_cursorPos;
		}
		else
			setFocused(false);
	}
}
void EditBox::mouseMove(int x, int y)
{
	if (m_held)
	{
		const sf::Vector2f pos = translate(x, y);

		if (m_bounds.contains(pos) && m_focused)
		{
			setCursorToMouse(pos.x, pos.y);
			m_selection[1] = m_cursorPos;
			updateHighlight(m_selection[1], m_selection[0]);

		}
	}
}
void EditBox::mouseRelease(int x, int y, sf::Mouse::Button button)
{
	if (button == sf::Mouse::Left)
	{
		const sf::Vector2f pos = translate(x, y);

		m_held = false;
		if (m_bounds.contains(pos) && m_focused)
		{
			setCursorToMouse(pos.x, pos.y);
			m_selection[1] = m_cursorPos;
			if (m_selection[0] > m_selection[1])
				std::swap(m_selection[0], m_selection[1]);
			updateSelection();
		}
	}
}

EditBox::EditBox()
{
	m_background.setFillColor(sf::Color(10, 10, 15));
	m_background.setOutlineThickness(2);
	m_background.setOutlineColor(sf::Color(60, 60, 90));

	m_text.setFillColor(sf::Color(255, 255, 255));
	m_text.setCharacterSize(20);

	m_cursor.setPosition(m_text.getPosition());
	m_cursor.setFillColor(sf::Color(255, 255, 255));
	m_cursor.setSize(sf::Vector2f(1, 20));

	m_highlight.setSize(sf::Vector2f(0, 20));
	m_highlight.setFillColor(sf::Color(100, 80, 100));
}

void EditBox::create(sf::FloatRect bounds)
{
	m_bounds = bounds;

	m_background.setPosition(bounds.left, bounds.top);
	m_background.setSize(sf::Vector2f(bounds.width, bounds.height));

	m_text.setPosition(std::floor(bounds.left + 10.f), std::floor(bounds.top + 10.f));
	m_text.setFont(*p_font);

	m_highlight.setPosition(bounds.left, bounds.top);
}

void EditBox::setText(const sf::String& text)
{
	m_string = text;
	m_text.setString(text);
	setCursorPosition(m_string.getSize());
}
const sf::String& EditBox::getText() const
{
	return m_string;
}

void EditBox::setCursorPosition(size_t pos)
{
	if (m_string.isEmpty())
		m_cursorPos = 0;
	else if (pos <= m_string.getSize())
		m_cursorPos = pos;
	else
		m_cursorPos = m_string.getSize();

	m_selection[0] = m_cursorPos;
	m_selection[1] = m_cursorPos;

	m_cursor.setPosition(m_text.getPosition() + sf::Vector2f(20.f * 0.6f * m_cursorPos, 3.f));
}
const sf::String& EditBox::getSelectedText() const
{
	return m_selected;
}

void EditBox::setFocused(bool focus)
{
	m_focused = focus;

	if (!focus)
	{
		m_selection[0] = 0;
		m_selection[1] = 0;
		m_selected.clear();
		m_highlight.setSize(sf::Vector2f(0.f, 20.f));
	}
}
bool EditBox::getFocused() const
{
	return m_focused;
}

void EditBox::handleEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::TextEntered:
		enterChar(event.text.unicode);
		break;
	case sf::Event::KeyPressed:
		keyPress(event.key.code);
		break;
	case sf::Event::MouseButtonPressed:
		mouseClick(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button);
		break;
	case sf::Event::MouseMoved:
		mouseMove(event.mouseMove.x, event.mouseMove.y);
		break;
	case sf::Event::MouseButtonReleased:
		mouseRelease(event.mouseButton.x, event.mouseButton.y, event.mouseButton.button);
		break;
	}
}
void EditBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background);
	target.draw(m_highlight);
	target.draw(m_text);

	if (m_focused)
		target.draw(m_cursor);
}