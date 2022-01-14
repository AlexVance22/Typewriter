#pragma once

#include "Group.h"

#include "gui/EditBox.h"
#include "gui/Button.h"

class Application;


class SaveAs : public Group
{
private:
	EditBox m_nameBox;
	Button m_confirm;

	Application* p_parent = nullptr;

public:
	SaveAs();

	void create(Application* parent);

	void setEnabled(bool enabled) override;

	void removeFocus() override;

	void handleEvent(const sf::Event& event) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};