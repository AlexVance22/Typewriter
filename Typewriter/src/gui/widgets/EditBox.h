#pragma once

#include "Widget.h"


class EditBox : public Widget
{
private:
	std::string m_string;
	sf::Text m_text;

	sf::RectangleShape m_cursor;
	size_t m_cursorPos = 0;

	sf::RectangleShape m_highlight;
	size_t m_selection[2] = { 0 };
	std::string m_selected;

	bool m_focused = false;
	bool m_held = false;

private:
	void setCursorToMouse(int x, int y);

	void updateSelection();
	void updateHighlight(size_t start, size_t end);

	void enterChar(uint32_t character);
	void keyPress(sf::Keyboard::Key key);
	void mouseClick(int x, int y, sf::Mouse::Button button);
	void mouseMove(int x, int y);
	void mouseRelease(int x, int y, sf::Mouse::Button button);

public:
	EditBox();

	void create(sf::FloatRect bounds);

	void setText(const std::string& text);
	void setText(std::string&& text);
	const std::string& getText() const;

	void setCursorPosition(size_t pos);
	const std::string& getSelectedText() const;

	void setFocused(bool focus);
	bool getFocused() const;

	void handleEvent(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};