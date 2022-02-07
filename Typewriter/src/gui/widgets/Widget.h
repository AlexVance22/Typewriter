#pragma once


class Widget : public sf::Drawable
{
protected:
	sf::RectangleShape m_background;
	sf::FloatRect m_bounds;

	const sf::View* p_view = nullptr;

	sf::Vector2f translate(int x, int y)
	{
		return p_window->mapPixelToCoords(sf::Vector2i(x, y), *p_view);
	}

public:
	const static sf::RenderWindow* p_window;
	const static sf::Font* p_font;

public:
	Widget() = default;
	virtual ~Widget() = default;

	void setView(const sf::View& view)
	{
		p_view = &view;
	}

	virtual void handleEvent(const sf::Event& event) = 0;
};