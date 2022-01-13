#pragma once

#include "EditState.h"
#include "gui/DivEditor.h"


class Page : public sf::Drawable
{
private:
	std::vector<DivEditor> m_divs;
	size_t m_active = 0;

	sf::RectangleShape m_background;

	sf::View m_view;

private:
	void reallign();
	void refocus();

	bool findFocused();

public:
	Page();

	void addDivision(EditState state);
	void setDivisionState(EditState state);
	void eraseDivision();

	void clear();
	std::vector<DivEditor>::const_iterator begin() const;
	std::vector<DivEditor>::const_iterator end() const;

	DivEditor& getActive();
	size_t getActiveIndex() const;
	EditState getActiveState() const;

	void resetView();

	bool contains(const sf::RenderWindow& window, int x, int y) const;
	void removeFocus();

	void handleEvent(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};