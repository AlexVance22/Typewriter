#pragma once

#include "Group.h"

#include "../widgets/EditBox.h"


class TitlePage : public Group
{
private:
	EditBox m_titleEditor;
	EditBox m_subEditor;

public:
	TitlePage();

	void create();

	void setTitle(const std::string& title);
	void setSubtitle(const std::string& subtitle);

	const std::string& getTitle() const;
	const std::string& getSubtitle() const;

	void removeFocus() override;
	bool isFocused() const;

	void handleEvent(const sf::Event& event) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};