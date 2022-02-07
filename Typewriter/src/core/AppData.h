#pragma once

#include "EditState.h"


enum class SaveState { New, Unsaved, Saved };


struct Paragraph
{
	std::string text;
	EditState state = EditState::None;

	Paragraph() = default;
	Paragraph(const std::string& _text, EditState _state);
	Paragraph(const Paragraph&) = delete;
	Paragraph(Paragraph&& other) noexcept;

	Paragraph& operator=(const Paragraph&) = delete;
	Paragraph& operator=(Paragraph && other) noexcept;
};

struct Document
{
	std::string title;
	std::string subtitle;
	std::vector<Paragraph> content;

	Document() = default;
	Document(const Document&) = delete;
	Document(Document&& other) noexcept;

	Document& operator=(const Document&) = delete;
	Document& operator=(Document&& other) noexcept;
};

struct AppData
{
	std::string filepath;
	std::string filename;

	EditState editstate = EditState::None;
	SaveState savestate = SaveState::New;

	AppData() = default;
	AppData(const AppData&) = delete;
	AppData(AppData&& other) noexcept;

	AppData& operator=(const AppData&) = delete;
	AppData& operator=(AppData&& other) noexcept;
};