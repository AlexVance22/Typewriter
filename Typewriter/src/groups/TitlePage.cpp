#include "PCH.h"
#include "TitlePage.h"


TitlePage::TitlePage() : Group(sf::FloatRect(0, 0, 1920, 980))
{
	m_titleEditor.setView(m_view);
	m_subEditor.setView(m_view);

	m_background.setPosition(0, 0);
	m_background.setSize(sf::Vector2f(455, 980));
	m_background.setFillColor(sf::Color(70, 70, 100, 70));
}

void TitlePage::create()
{
	m_titleEditor.create(sf::FloatRect(15, 100, 425, 50));
	m_subEditor.create(sf::FloatRect(15, 200, 425, 50));
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

void TitlePage::removeFocus()
{
	m_titleEditor.setFocused(false);
	m_subEditor.setFocused(false);
}
bool TitlePage::isFocused() const
{
	return m_titleEditor.getFocused() || m_subEditor.getFocused();
}

void TitlePage::handleEvent(const sf::Event& event)
{
	if (m_enableUpdate || event.type == sf::Event::Resized)
	{
		m_titleEditor.handleEvent(event);
		m_subEditor.handleEvent(event);

		switch (event.type)
		{
		case sf::Event::Resized:
			m_view.reset(sf::FloatRect(0, 0, (float)event.size.width, (float)event.size.height));
			m_background.setSize(sf::Vector2f(455, (float)event.size.height));
			break;
		}
	}
}
void TitlePage::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_enableDisplay)
	{
		target.setView(m_view);

		target.draw(m_background);
		target.draw(m_titleEditor);
		target.draw(m_subEditor);
	}
}