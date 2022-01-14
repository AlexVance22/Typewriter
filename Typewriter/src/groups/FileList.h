#pragma once

#include "Group.h"

#include "gui/Button.h"

class Application;


class FileList : public Group
{
private:
	std::vector<std::string> m_filenames;
	std::vector<Button> m_buttons;

	Button m_confirm;
	Button m_delete;

	size_t m_selected = 0;

	Application* p_parent = nullptr;

private:
	void refreshFiles();
	void refreshButtons();

public:
	FileList();

	void create(Application* parent);

	void setEnabled(bool enabled) override;

	void removeFocus() override;

	void handleEvent(const sf::Event& event) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};