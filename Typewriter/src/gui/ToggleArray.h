#pragma once


class ToggleArray : public sf::Drawable
{
private:
	std::vector<sf::RectangleShape> m_shapes;
	std::vector<sf::Text> m_labels;

	sf::RectangleShape m_background;

	size_t m_selected = 0;

public:
	ToggleArray(const std::vector<sf::String>& buttons, float width);

	void setFont(const sf::Font& font);

	size_t getSelected() const;
	void setSelected(size_t index);

	void handleEvent(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};