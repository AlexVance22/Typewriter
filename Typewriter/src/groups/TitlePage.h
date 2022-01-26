#pragma once

#include "Group.h"

#include "gui/EditBox.h"


class TitlePage : public Group
{
private:
	EditBox m_titleEditor;
	EditBox m_subEditor;

public:
	TitlePage();

	void create();

	void setTitle(const sf::String& title);
	void setSubtitle(const sf::String& subtitle);

	const sf::String& getTitle() const;
	const sf::String& getSubtitle() const;

	void removeFocus() override;
	bool isFocused() const;

	void handleEvent(const sf::Event& event) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};