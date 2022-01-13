#pragma once

#include "Widget.h"


class ToggleArray : public Widget
{
private:
	std::vector<sf::RectangleShape> m_shapes;
	std::vector<sf::Text> m_labels;

	size_t m_selected = 0;

	sf::RectangleShape m_background;

	sf::View m_view;

	bool m_enabled = true;

public:
	ToggleArray();

	void create(const std::vector<sf::String>& buttons, float width);

	size_t getSelected() const;
	void setSelected(size_t index);

	bool contains(const sf::RenderWindow& window, int x, int y) const;

	void setEnabled(bool enabled);
	bool getEnabled() const;

	void handleEvent(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};