#pragma once

#include "gui/Button.h"

class Application;


class FileList : public sf::Drawable
{
private:
	std::vector<std::string> m_filenames;
	std::vector<Button> m_buttons;

	Button m_confirm;
	Button m_delete;

	size_t m_selected = 0;

	sf::RectangleShape m_background;

	sf::View m_view;

	bool m_enabled = false;

	Application* p_parent;

private:
	void refreshFiles();
	void refreshButtons();

public:
	FileList();

	void create(Application* parent);

	void setEnabled(bool enabled);
	bool getEnabled() const;

	void handleEvent(const sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};