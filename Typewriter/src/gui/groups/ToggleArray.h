#pragma once

#include "Group.h"

#include "../widgets/Widget.h"


class ToggleButton : public Widget
{
private:
	sf::Text m_text;

	bool m_toggled = false;

public:
	ToggleButton();

	void create(sf::FloatRect bounds, const sf::String& text);

	void resize(sf::FloatRect bounds);

	void setToggled(bool toggled);
	bool isToggled() const;

	void handleEvent(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};


class ToggleArray : public Group
{
private:
	std::vector<ToggleButton> m_options;

	size_t m_selected = 0;

public:
	ToggleArray();

	void create(const std::vector<sf::String>& buttons, float width);

	size_t getSelected() const;
	void setSelected(size_t index);

	void removeFocus() override;

	void handleEvent(const sf::Event& event) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};