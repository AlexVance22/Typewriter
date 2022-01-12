#include "PCH.h"
#include "Page.h"

#include "Exceptions.h"


constexpr float inToPx(float in)
{
	return (in / 8.25f) * 990.f;
}


void Page::reallign()
{
	float lasttop = 60 + inToPx(1.f);

	for (auto& div : m_divs)
	{
		div.setBoxPosition(sf::Vector2f(div.getBoxPosition().x, lasttop));
		lasttop += div.getBoxSize().y;
	}
}
void Page::refocus()
{
	for (size_t i = 0; i < m_divs.size(); i++)
		m_divs[i].setFocused((i == m_active));
}

bool Page::findFocused()
{
	for (size_t i = 0; i < m_divs.size(); i++)
	{
		if (m_divs[i].isFocused())
		{
			m_active = i;
			return true;
		}
	}

	return false;
}

void Page::addDivision(EditState state)
{
	DivEditor area;

	area.setBoxPosition(sf::Vector2f(465, 121));
	area.setBoxSize(sf::Vector2f(990, 0));
	area.setState(state);

	switch (state)
	{
	case EditState::Scene:
		area.setInnerPadding(inToPx(1.5f), 0, inToPx(1.f), inToPx(0.2f));
		area.setTextStyle(DivEditor::Style::Bold, DivEditor::Style::Upper);
		break;
	case EditState::Directorial:
		area.setInnerPadding(inToPx(1.5f), 0, inToPx(1.f), inToPx(0.2f));
		area.setTextStyle(DivEditor::Style::Regular, DivEditor::Style::Input);
		break;
	case EditState::Name:
		area.setInnerPadding(inToPx(3.7f), 0, inToPx(2.5f), 0);
		area.setTextStyle(DivEditor::Style::Regular, DivEditor::Style::Upper);
		break;
	case EditState::Parenthetical:
		area.setInnerPadding(inToPx(3.1f), 0, inToPx(2.5f), 0);
		area.setTextStyle(DivEditor::Style::Regular, DivEditor::Style::Input);
		break;
	case EditState::Speech:
		area.setInnerPadding(inToPx(2.5f), 0, inToPx(2.5f), inToPx(0.2f));
		area.setTextStyle(DivEditor::Style::Regular, DivEditor::Style::Input);
		break;
	}

	if (!m_divs.empty())
		m_active++;

	m_divs.insert(m_divs.begin() + m_active, area);

	reallign();
	refocus();
}
void Page::setDivisionState(EditState state)
{
	DivEditor& area = m_divs[m_active];

	switch (state)
	{
	case EditState::Scene:
		area.setInnerPadding(inToPx(1.5f), 0, inToPx(1.f), inToPx(0.2f));
		area.setTextStyle(DivEditor::Style::Bold, DivEditor::Style::Upper);
		break;
	case EditState::Directorial:
		area.setInnerPadding(inToPx(1.5f), 0, inToPx(1.f), inToPx(0.2f));
		area.setTextStyle(DivEditor::Style::Regular, DivEditor::Style::Input);
		break;
	case EditState::Name:
		area.setInnerPadding(inToPx(3.7f), 0, inToPx(2.5f), 0);
		area.setTextStyle(DivEditor::Style::Regular, DivEditor::Style::Upper);
		break;
	case EditState::Parenthetical:
		area.setInnerPadding(inToPx(3.1f), 0, inToPx(2.5f), 0);
		area.setTextStyle(DivEditor::Style::Regular, DivEditor::Style::Input);
		break;
	case EditState::Speech:
		area.setInnerPadding(inToPx(2.5f), 0, inToPx(2.5f), inToPx(0.2f));
		area.setTextStyle(DivEditor::Style::Regular, DivEditor::Style::Input);
		break;
	}

	area.setState(state);
	area.setFocused(true);

	reallign();
}
void Page::eraseDivision()
{
	auto removed = m_divs.begin() + m_active;

	if (m_active == 0 || removed->getText().getSize() != 0)
		return;

	m_active--;

	refocus();

	m_divs.erase(removed);
	m_divs[m_active].setCursorPosition(m_divs[m_active].getEndOfString());

	reallign();
}

void Page::clear()
{
	m_divs.clear();
	m_active = 0;
}
std::vector<DivEditor>::const_iterator Page::begin() const
{
	return m_divs.begin();
}
std::vector<DivEditor>::const_iterator Page::end() const
{
	return m_divs.cend();
}

DivEditor& Page::getActive()
{
	return m_divs[m_active];
}
size_t Page::getActiveIndex() const
{
	return m_active;
}
EditState Page::getActiveState() const
{
	return m_divs[m_active].getState();
}

void Page::handleEvent(const sf::Event & event)
{
	try
	{
		for (auto& div : m_divs)
			div.handleEvent(event);
	}
	catch (const OutOfRange& e)
	{
		if (e.bound == 1 && m_active < m_divs.size() - 1)
		{
			m_active++;
			m_divs[m_active].setCursorPosition(0);
		}
		else if (e.bound == -1 && m_active > 0)
		{
			m_active--;
			m_divs[m_active].setCursorPosition(m_divs[m_active].getEndOfString());
		}

		refocus();
	}

	if (m_divs[m_active].isResized())
		reallign();

	if (event.type == sf::Event::MouseButtonPressed && !m_divs[m_active].isFocused())
	{
		const sf::Vector2i pos = (sf::Vector2i)DivEditor::window->mapPixelToCoords(
			sf::Vector2i(event.mouseButton.x, event.mouseButton.y), *DivEditor::view);

		if (sf::IntRect(465, 60, 990, 950).contains(pos))
		{
			if (findFocused())
				return;

			if (pos.y < m_divs.front().getBoxPosition().y)
				m_active = 0;
			else
				m_active = m_divs.size() - 1;

			m_divs[m_active].setFocused(true);
		}
	}
}
void Page::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const auto& div : m_divs)
		target.draw(div);
}