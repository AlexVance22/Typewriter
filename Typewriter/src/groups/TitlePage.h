#pragma once

#include "gui/EditBox.h"


class TitlePage : public sf::Drawable
{
private:
	EditBox m_titleEditor;
	EditBox m_subEditor;

	sf::RectangleShape m_background;

	sf::View m_view;

	bool m_enabled = true;

public:
	TitlePage();

	void setTitle(const sf::String& title);
	void setSubtitle(const sf::String& subtitle);

	const sf::String& getTitle() const;
	const sf::String& getSubtitle() const;

	bool contains(const sf::RenderWindow& window, int x, int y) const;
	void removeFocus();

	void setEnabled(bool enabled);
	bool getEnabled() const;

	void handleEvent(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};