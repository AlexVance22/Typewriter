#include "PCH.h"
#include "AppData.h"


Paragraph::Paragraph(const std::string& _text, EditState _state)
	: text(_text), state(_state)
{}

Paragraph::Paragraph(Paragraph&& other) noexcept
	: text(std::move(other.text)), state(other.state)
{}

Paragraph& Paragraph::operator=(Paragraph&& other) noexcept
{
	text = std::move(other.text);
	state = other.state;

	return *this;
}


Document::Document(Document&& other) noexcept
	: title(std::move(other.title)), subtitle(std::move(other.subtitle)), content(std::move(other.content))
{}

Document& Document::operator=(Document&& other) noexcept
{
	title = std::move(other.title);
	subtitle = std::move(other.subtitle);
	content = std::move(other.content);

	return *this;
}


AppData::AppData(AppData&& other) noexcept
	: filepath(std::move(filepath)), filename(std::move(filename))
{}

AppData& AppData::operator=(AppData&& other) noexcept
{
	filepath = std::move(filepath);
	filename = std::move(filename);

	return *this;
}