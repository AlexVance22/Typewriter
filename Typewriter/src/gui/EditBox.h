#pragma once


class EditBox : public sf::Drawable
{
private:
	sf::String m_string;
	sf::Text m_text;

	sf::FloatRect m_bounds;

	sf::RectangleShape m_background;

	sf::RectangleShape m_cursor;
	size_t m_cursorPos = 0;

	sf::RectangleShape m_highlight;
	size_t m_selection[2] = { 0 };
	sf::String m_selected;

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
	EditBox(sf::FloatRect bounds, const sf::Font& font);

	void setText(const sf::String& text);
	const sf::String& getText() const;

	void setFocused(bool focus);

	void setCursorPosition(size_t pos);
	const sf::String& getSelectedText() const;

	void handleEvent(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};