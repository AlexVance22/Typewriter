#include "PCH.h"
#include "Group.h"


Group::Group(sf::FloatRect view) : m_view(view)
{

}

void Group::setUpdateEnabled(bool enable)
{
	m_enableUpdate = enable;
}
bool Group::isUpdateEnabled() const
{
	return m_enableUpdate;
}

void Group::setDisplayEnabled(bool enable)
{
	m_enableDisplay = enable;
}
bool Group::isDisplayEnabled() const
{
	return m_enableDisplay;
}

void Group::setEnabled(bool enable)
{
	m_enableUpdate = enable;
	m_enableDisplay = enable;
}
bool Group::getEnabled() const
{
	return m_enableUpdate && m_enableDisplay;
}

bool Group::contains(const sf::RenderWindow & window, int x, int y) const
{
	const sf::Vector2f pos = window.mapPixelToCoords(sf::Vector2i(x, y), m_view);
	return m_background.getGlobalBounds().contains(pos);
}