#pragma once

#include "Widget.h"

#include "core/EditState.h"


class DivEditor : public Widget
{
public:
	enum class Style
	{
		Regular = 0,
		Bold	= 1,
		Input	= 2,
		Upper	= 3,
		Lower	= 4
	};

private:
	Style m_case = Style::Input;
	std::string m_string;
	sf::Text m_text;

	sf::FloatRect m_padding;

	std::vector<size_t> m_lineSize;
	size_t m_charsPerLine = 1;

	sf::RectangleShape m_cursor;
	size_t m_cursorPos = 0;

	std::vector<sf::RectangleShape> m_highlight;
	size_t m_selection[2] = { 0 };
	std::string m_selected;

	bool m_focused = false;
	bool m_resized = false;
	bool m_held = false;

	EditState m_state;

private:
	size_t charsPerLine() const;
	float getMinHeight() const;

	void setCursorToMouse(int x, int y);

	void updateSelection();
	void updateHighlight(size_t start, size_t end);

	void enterChar(uint32_t character);
	void keyPress(sf::Keyboard::Key key);
	void mouseClick(int x, int y, sf::Mouse::Button button);
	void mouseMove(int x, int y);
	void mouseRelease(int x, int y, sf::Mouse::Button button);

	void wrap();

public:
	DivEditor();

	void setText(const sf::String& text);
	const std::string& getText() const;
	void setTextStyle(Style textstyle, Style textcase);

	void setBoxPosition(sf::Vector2f position);
	void setBoxSize(sf::Vector2f size);
	void setInnerPadding(float left, float top, float right, float bottom);

	sf::Vector2f getBoxPosition() const;
	sf::Vector2f getBoxSize() const;

	void resizeToFit();
	bool isResized() const;

	void setState(EditState state);
	EditState getState() const;

	void setFocused(bool focus);
	bool isFocused() const;

	void setCursorPosition(size_t pos);
	const std::string& getSelectedText() const;
	size_t getEndOfString() const;

	void handleEvent(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};