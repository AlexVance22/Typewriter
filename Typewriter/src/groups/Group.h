#pragma once


class Group : public sf::Drawable
{
protected:
	sf::RectangleShape m_background;
	sf::View m_view;

	bool m_enableUpdate = true;
	bool m_enableDisplay = true;

public:
	Group(sf::FloatRect view);
	virtual ~Group() = default;

	virtual void setUpdateEnabled(bool enable);
	bool isUpdateEnabled() const;

	virtual void setDisplayEnabled(bool enable);
	bool isDisplayEnabled() const;

	virtual void setEnabled(bool enable);
	bool getEnabled() const;

	bool contains(const sf::RenderWindow& window, int x, int y) const;

	virtual void removeFocus() = 0;

	virtual void handleEvent(const sf::Event& event) = 0;
};