#pragma once


class Button : public sf::Drawable
{
private:
	sf::FloatRect m_bounds;
	sf::RectangleShape m_shape;
	sf::Text m_text;

public:
	Button(sf::FloatRect bounds, const sf::String& text, const sf::Font& font);

	bool mouseClick(int x, int y);
	void mouseMove(int x, int y);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};