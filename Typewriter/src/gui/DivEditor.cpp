#include "PCH.h"
#include "DivEditor.h"

#include "Exceptions.h"


size_t DivEditor::charsPerLine() const
{
	const float linewidth = m_bounds.width - m_padding.left - m_padding.width;
	const float charwidth = m_text.getCharacterSize() * 0.6f;
	const float result = linewidth / charwidth;
	return (size_t)std::max(result, 1.f);
}
float DivEditor::getMinHeight() const
{
	return 23.f * m_lineSize.size() + m_padding.top + m_padding.height;
}

void DivEditor::setCursorToMouse(int x, int y)
{
	const sf::Vector2i relative = sf::Vector2i(x, y - 2) - (sf::Vector2i)m_text.getPosition();

	constexpr float width = 20.f * 0.6f;
	constexpr float height = 20.f + 3.f;

	const size_t lineindex = std::min((size_t)(relative.y / height), m_lineSize.size() - 1);
	const size_t charindex = (relative.x > m_lineSize[lineindex] * width) ? m_lineSize[lineindex] : (size_t)(relative.x / width + 0.5f);

	size_t previous = 0;
	for (size_t i = 0; i < lineindex; i++)
		previous += m_lineSize[i];

	const size_t displayindex = previous + charindex;

	const sf::Vector2f displaypos(charindex * width - 1, lineindex * height + 4);
	m_cursor.setPosition(m_text.getPosition() + displaypos);

	m_cursorPos = std::min(displayindex, m_string.getSize());
}

void DivEditor::updateSelection()
{
	if (m_selection[0] == m_selection[1])
	{
		m_selected.clear();
		m_highlight.clear();
		return;
	}

	if (m_selection[0] > m_selection[1])
		std::swap(m_selection[0], m_selection[1]);

	m_selected = m_string.substring(m_selection[0], m_selection[1] - m_selection[0]);
	updateHighlight(m_selection[0], m_selection[1]);
}
void DivEditor::updateHighlight(size_t start, size_t end)
{
	if (start == end)
	{
		m_highlight.clear();
		return;
	}
	if (end < start)
	{
		const size_t buf = start;
		start = end;
		end = buf;
	}

	const sf::Vector2f pos1 = m_text.findCharacterPos(start) + sf::Vector2f(0, 3);
	const sf::Vector2f pos2 = m_text.findCharacterPos(end) + sf::Vector2f(0, 3);

	const size_t difflines = (size_t)((pos2.y - pos1.y) / 23);
	const size_t startline = (size_t)((pos1.y - m_text.getPosition().y) / 23);

	m_highlight.resize(difflines + 1);

	for (size_t i = 0; i < difflines + 1; i++)
	{
		m_highlight[i].setPosition(m_bounds.left + m_padding.left, pos1.y + i * 23);
		m_highlight[i].setSize(sf::Vector2f((m_lineSize[startline] - 1) * 20.f * 0.6f, 20));
		m_highlight[i].setFillColor(sf::Color(255, 200, 255));
	}

	m_highlight.front().setSize(sf::Vector2f((m_highlight.front().getPosition() + m_highlight.front().getSize()).x - pos1.x, 20));
	m_highlight.front().setPosition(pos1);
	m_highlight.back().setSize(sf::Vector2f(pos2.x - m_highlight.back().getPosition().x, 20));
}

void DivEditor::enterChar(uint32_t character)
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
				wrap();
				return;
			}
		}

		if (character == 0x8) // backspace
		{
			if (m_string.getSize() == 0)
				return;

			m_cursorPos--;
			m_lineSize.back()--;

			m_string.erase(m_cursorPos, 1);

			if (m_cursorPos == m_string.getSize() && m_lineSize.back() > 0)
				m_text.setString(m_text.getString().substring(0, m_text.getString().getSize() - 1));
			else
				wrap();

			setCursorPosition(m_cursorPos);
		}
		else
		{
			switch (m_case)
			{
			case Style::Lower:
				character += 32 * (character > 64 && character < 91);
				break;
			case Style::Upper:
				character -= 32 * (character > 96 && character < 123);
				break;
			}

			m_string.insert(m_cursorPos, character);

			m_cursorPos++;
			m_lineSize.back()++;

			if (m_cursorPos == m_string.getSize() && m_lineSize.back() < m_charsPerLine)
				m_text.setString(m_text.getString() + character);
			else
				wrap();

			setCursorPosition(m_cursorPos);
		}

		m_selection[0] = m_cursorPos;
		m_selection[1] = m_cursorPos;
	}
}
void DivEditor::keyPress(sf::Keyboard::Key key)
{
	if (m_focused && !m_held)
	{
		switch (key)
		{
		case sf::Keyboard::Left:
			if (m_cursorPos > 0)
			{
				m_cursorPos--;
				setCursorPosition(m_cursorPos);
				break;
			}
			else throw OutOfRange(-1);

		case sf::Keyboard::Right:
			if (m_cursorPos < m_string.getSize())
			{
				m_cursorPos++;
				setCursorPosition(m_cursorPos);
				break;
			}
			else throw OutOfRange(1);

		case sf::Keyboard::Up:
			if (m_cursorPos >= m_charsPerLine)
			{
				m_cursorPos -= m_charsPerLine;
				setCursorPosition(m_cursorPos);
				break;
			}
			else throw OutOfRange(-1);

		case sf::Keyboard::Down:
			if (m_cursorPos < m_string.getSize() - m_charsPerLine)
			{
				m_cursorPos += m_charsPerLine;
				setCursorPosition(m_cursorPos);
				break;
			}
			else throw OutOfRange(1);

		case sf::Keyboard::X:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			{
				sf::Clipboard::setString(m_selected);

				if (m_selection[0] != m_selection[1])
				{
					m_string.erase(m_selection[0], m_selection[1] - m_selection[0]);
					m_cursorPos = m_selection[0];
					m_selection[1] = m_cursorPos;
					updateSelection();

					setCursorPosition(m_cursorPos);

					wrap();
				}
			}
			break;
		case sf::Keyboard::C:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
				sf::Clipboard::setString(m_selected);
			break;
		case sf::Keyboard::V:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
			{
				std::string copied = sf::Clipboard::getString();
				if (copied.empty())
					return;

				switch (m_case)
				{
				case Style::Input:
					for (auto& c : copied)
						c += 32 * (c > 64 && c < 91);
					break;
				case Style::Upper:
					for (auto& c : copied)
						c -= 32 * (c > 96 && c < 123);
					break;
				}

				m_string.insert(m_cursorPos, copied);
				m_cursorPos += copied.size();
				
				wrap();

				setCursorPosition(m_cursorPos);
				m_selection[0] = m_cursorPos;
				m_selection[1] = m_cursorPos;
			}
			break;
		}
	}
}
void DivEditor::mouseClick(int x, int y, sf::Mouse::Button button)
{
	if (button == sf::Mouse::Left)
	{
		const sf::Vector2f pos = translate(x, y);

		m_held = true;
		if (m_bounds.contains(pos))
		{
			setFocused(true);

			setCursorToMouse((int)pos.x, (int)pos.y);
			m_selection[0] = m_cursorPos;
			m_selection[1] = m_cursorPos;
		}
		else
			setFocused(false);
	}
}
void DivEditor::mouseMove(int x, int y)
{
	if (m_held)
	{
		const sf::Vector2f pos = translate(x, y);

		if (m_bounds.contains(pos) && m_focused)
		{
			setCursorToMouse((int)pos.x, (int)pos.y);
			m_selection[1] = m_cursorPos;
			updateHighlight(m_selection[1], m_selection[0]);

		}
	}
}
void DivEditor::mouseRelease(int x, int y, sf::Mouse::Button button)
{
	if (button == sf::Mouse::Left)
	{
		const sf::Vector2f pos = translate(x, y);

		m_held = false;
		if (m_bounds.contains(pos) && m_focused)
		{
			setCursorToMouse((int)pos.x, (int)pos.y);
			m_selection[1] = m_cursorPos;
			if (m_selection[0] > m_selection[1])
				std::swap(m_selection[0], m_selection[1]);
			updateSelection();
		}
	}
}

void DivEditor::wrap()
{
	constexpr size_t MAX_LINE_SIZE = 64;

	m_charsPerLine = charsPerLine();

	char linebuffer[MAX_LINE_SIZE] = { 0 };
	char wordbuffer[MAX_LINE_SIZE] = { 0 };

	sf::String wrapped = "";

	m_lineSize.clear();
	m_lineSize.push_back(0);
	size_t wordsize = 0;

	auto pushLineAndFlush = [&]()
	{
		if (linebuffer[m_lineSize.back() - 1] == ' ')
			m_lineSize.back()--;
		linebuffer[m_lineSize.back()++] = '\n';
		linebuffer[m_lineSize.back()] = '\0';
		wrapped += linebuffer;
		memset(linebuffer, '\0', MAX_LINE_SIZE);
		m_lineSize.push_back(0);
	};

	for (char c : m_string)
	{
		if (wordsize >= m_charsPerLine) // if max word size
		{
			if (m_lineSize.back() > 0)
				pushLineAndFlush();

			wordbuffer[m_charsPerLine - 1] = '\n';
			wordbuffer[m_charsPerLine] = '\0';
			wrapped += wordbuffer;
			m_lineSize.back() = m_charsPerLine;
			memset(wordbuffer, '\0', MAX_LINE_SIZE);
			wordsize = 0;
			m_lineSize.push_back(0);

			if (c != ' ')
				wordbuffer[wordsize++] = c;
		}
		else if (wordsize + m_lineSize.back() >= m_charsPerLine) // if word overflows line
		{
			if (c == ' ')
			{
				pushLineAndFlush();

				for (size_t i = 0; i < wordsize; i++)
					linebuffer[m_lineSize.back()++] = wordbuffer[i];
				linebuffer[m_lineSize.back()++] = ' ';

				memset(wordbuffer, '\0', MAX_LINE_SIZE);
				wordsize = 0;
			}
			else
				wordbuffer[wordsize++] = c;
		}
		else
		{
			if (c == ' ')
			{
				for (size_t i = 0; i < wordsize; i++)
					linebuffer[m_lineSize.back()++] = wordbuffer[i];
				linebuffer[m_lineSize.back()++] = ' ';

				memset(wordbuffer, '\0', MAX_LINE_SIZE);
				wordsize = 0;
			}
			else
				wordbuffer[wordsize++] = c;
		}
	}

	if (wordsize > 0)
	{
		if (m_lineSize.back() + wordsize >= m_charsPerLine)
			pushLineAndFlush();

		for (size_t i = 0; i < wordsize; i++)
			linebuffer[m_lineSize.back()++] = wordbuffer[i];
	}
	if (m_lineSize.back() && linebuffer[0] != '\0')
		wrapped += linebuffer;

	m_text.setString(wrapped);
	m_bounds.height = getMinHeight();
	m_resized = true;

	setCursorPosition(m_cursorPos);
}

DivEditor::DivEditor()
{
	m_cursor.setPosition(m_text.getPosition());
	m_cursor.setFillColor(sf::Color(0, 0, 0));
	m_cursor.setSize(sf::Vector2f(1, 20));

	m_text.setFillColor(sf::Color(0, 0, 0));
	m_text.setCharacterSize(20);
	m_text.setFont(*p_font);
}

void DivEditor::setText(const sf::String& text)
{
	m_string = text;

	wrap();

	setCursorPosition(text.getSize());
}
const sf::String& DivEditor::getText() const
{
	return m_string;
}
void DivEditor::setTextStyle(Style textstyle, Style textcase)
{
	m_text.setStyle((uint32_t)textstyle);

	switch (textstyle)
	{
	case Style::Regular:
		m_text.setLetterSpacing(1.f);
		break;
	case Style::Bold:
		m_text.setLetterSpacing(0.78f);
		break;
	}

	switch (textcase)
	{
	case Style::Input:
		if (m_case == Style::Input)
			break;
	case Style::Lower:
		for (auto& c : m_string)
			c += 32 * (c > 64 && c < 91);
		break;
	case Style::Upper:
		for (auto& c : m_string)
			c -= 32 * (c > 96 && c < 123);
		break;
	}

	m_case = textcase;

	wrap();
}

void DivEditor::setBoxPosition(sf::Vector2f position)
{
	m_bounds.left = position.x;
	m_bounds.top = position.y;

	m_text.setPosition(std::floor(m_bounds.left + m_padding.left), std::floor(m_bounds.top + m_padding.top));

	setCursorPosition(m_cursorPos);
}
void DivEditor::setBoxSize(sf::Vector2f size)
{
	m_bounds.width = size.x;
	m_bounds.height = size.y;

	wrap();

	setCursorPosition(m_cursorPos);
}
void DivEditor::setInnerPadding(float left, float top, float right, float bottom)
{
	m_padding.left = left;
	m_padding.top = top;
	m_padding.width = right;
	m_padding.height = bottom;

	m_text.setPosition(std::floor(m_bounds.left + m_padding.left), std::floor(m_bounds.top + m_padding.top));

	wrap();

	setCursorPosition(m_cursorPos);
}

sf::Vector2f DivEditor::getBoxPosition() const
{
	return sf::Vector2f(m_bounds.left, m_bounds.top);
}
sf::Vector2f DivEditor::getBoxSize() const
{
	return sf::Vector2f(m_bounds.width, m_bounds.height);
}

void DivEditor::resizeToFit()
{
	m_bounds.height = getMinHeight();
}
bool DivEditor::isResized() const
{
	return m_resized;
}

void DivEditor::setState(EditState state)
{
	m_state = state;
}
EditState DivEditor::getState() const
{
	return m_state;
}

void DivEditor::setFocused(bool focus)
{
	m_focused = focus;

	if (!focus)
	{
		m_selection[0] = 0;
		m_selection[1] = 0;
		m_selected.clear();
		m_highlight.clear();
	}
}
bool DivEditor::isFocused() const
{
	return m_focused;
}

void DivEditor::setCursorPosition(size_t pos)
{
	if (m_string.isEmpty())
		m_cursorPos = 0;
	else if (pos <= m_string.getSize())
		m_cursorPos = pos;
	else
		m_cursorPos = m_string.getSize();

	m_selection[0] = m_cursorPos;
	m_selection[1] = m_cursorPos;

	if (m_cursorPos % m_charsPerLine == 0 && m_cursorPos == m_string.getSize())
		m_cursor.setPosition(m_text.findCharacterPos(m_cursorPos - 1));
	else
		m_cursor.setPosition(m_text.findCharacterPos(m_cursorPos));
}
const sf::String& DivEditor::getSelectedText() const
{
	return m_selected;
}
size_t DivEditor::getEndOfString() const
{
	return m_string.getSize();
}

void DivEditor::handleEvent(const sf::Event& event)
{
	m_resized = false;

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
void DivEditor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	/*
	sf::RectangleShape debugBorder;
	debugBorder.setFillColor(sf::Color(0, 0, 0, 0));
	debugBorder.setPosition(getBoxPosition());
	debugBorder.setSize(getBoxSize());
	debugBorder.setOutlineThickness(1);
	debugBorder.setOutlineColor(sf::Color(m_focused * 255, 0, !m_focused * 255));

	target.draw(debugBorder);
	*/

	for (const auto& line : m_highlight)
		target.draw(line);

	target.draw(m_text);

	if (m_focused)
		target.draw(m_cursor);
}