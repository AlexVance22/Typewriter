#pragma once

#include "Widget.h"


class Button : public Widget
{
private:
	sf::Text m_text;

	bool m_clicked = false;
	bool m_hovered = false;

public:
	Button();

	void create(sf::FloatRect bounds, const sf::String& text);

	bool getClick() const;

	void handleEvent(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};