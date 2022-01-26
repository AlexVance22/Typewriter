#pragma once

#include "Group.h"

#include "EditState.h"
#include "gui/DivEditor.h"


class Page : public Group
{
private:
	std::vector<DivEditor> m_divs;
	size_t m_active = 0;

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

	void removeFocus() override;
	bool isFocused() const;

	void handleEvent(const sf::Event& event) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};