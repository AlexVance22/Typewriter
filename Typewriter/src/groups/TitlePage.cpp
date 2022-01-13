#include "PCH.h"
#include "TitlePage.h"


TitlePage::TitlePage()
	: m_view(sf::FloatRect(0, 0, 1920, 980))
{
	m_titleEditor.create(sf::FloatRect(20, 100, 425, 50));
	m_titleEditor.setView(m_view);
	m_subEditor.create(sf::FloatRect(20, 200, 425, 50));
	m_subEditor.setView(m_view);

	m_background.setPosition(0, 0);
	m_background.setSize(sf::Vector2f(485, 980));
	m_background.setFillColor(sf::Color(70, 70, 100, 70));
}

void TitlePage::setTitle(const sf::String& title)
{
	m_titleEditor.setText(title);
}
void TitlePage::setSubtitle(const sf::String& subtitle)
{
	m_subEditor.setText(subtitle);
}

const sf::String& TitlePage::getTitle() const
{
	return m_titleEditor.getText();
}
const sf::String& TitlePage::getSubtitle() const
{
	return m_subEditor.getText();
}

bool TitlePage::contains(const sf::RenderWindow& window, int x, int y) const
{
	const sf::Vector2f pos = window.mapPixelToCoords(sf::Vector2i(x, y), m_view);
	return m_background.getGlobalBounds().contains(pos);
}
void TitlePage::removeFocus()
{
	m_titleEditor.setFocused(false);
	m_subEditor.setFocused(false);
}

void TitlePage::setEnabled(bool enabled)
{
	m_enabled = enabled;
}
bool TitlePage::getEnabled() const
{
	return m_enabled;
}

void TitlePage::handleEvent(const sf::Event& event)
{
	if (m_enabled)
	{
		m_titleEditor.handleEvent(event);
		m_subEditor.handleEvent(event);

		switch (event.type)
		{
		case sf::Event::Resized:
			m_view.reset(sf::FloatRect(0, 0, (float)event.size.width, 980));
			break;
		}
	}
}
void TitlePage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_enabled)
	{
		target.setView(m_view);

		target.draw(m_background);
		target.draw(m_titleEditor);
		target.draw(m_subEditor);
	}
}