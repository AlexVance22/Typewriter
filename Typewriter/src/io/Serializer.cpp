#include "PCH.h"
#include "Serializer.h"

#include "ioHTML.h"
#include "OutPDF.h"
#include "OutIMG.h"
#include "gui/Dialogs.h"
#include "gui/Button.h"
#include "gui/EditBox.h"

#include "Application.h"


bool Serializer::saveFile(Application* app)
{
	const std::string outfile = "user/" + app->m_projName;
	
	return outHTML(app->m_page, app->m_title, app->m_subtitle, outfile);
}

bool Serializer::saveAsFile(Application* app)
{
	sf::RectangleShape background;
	background.setPosition(700, 300);
	background.setSize(sf::Vector2f(520, 200));
	background.setFillColor(sf::Color(40, 40, 60));
	background.setOutlineThickness(3);
	background.setOutlineColor(sf::Color(10, 10, 15));

	EditBox nameBox(sf::FloatRect(720, 320, 480, 50), app->m_font);
	nameBox.setFocused(true);

	Button confirm(sf::FloatRect(800, 400, 320, 40), "Save As", app->m_font);

	sf::Event event;
	while (app->m_window.waitEvent(event))
	{
		nameBox.handleEvent(event);

		switch (event.type)
		{
		case sf::Event::Closed:
			app->m_window.close();
			return false;

		case sf::Event::MouseButtonPressed:
			if (confirm.mouseClick(event.mouseButton.x, event.mouseButton.y))
			{
				if (nameBox.getText() == "")
					break;

				if (fs::exists("user/" + (std::string)nameBox.getText() + ".html"))
				{
					// display message
					break;
				}

				if (outHTML(app->m_page, app->m_title, app->m_subtitle, "user/" + (std::string)nameBox.getText()))
				{
					app->m_newProject = false;
					app->m_projName = nameBox.getText();
					return true;
				}
				
				// display message
			}
			break;
		case sf::Event::MouseMoved:
			confirm.mouseMove(event.mouseMove.x, event.mouseMove.y);
			break;

		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				return false;
			break;
		}

		app->render();
		app->m_window.draw(background);
		app->m_window.draw(nameBox);
		app->m_window.draw(confirm);
		app->m_window.display();
	}

	return false;
}

bool Serializer::exportFile(Application* app)
{
	const char* filter = "PDF Document (*.pdf)\0*.pdf\0HTML Document (*.html + *.css)\0*.html;*css\0";
							//PNG Image (*.png)\0*.png\0JPG Image (*.jpg)\0*.jpg\0SVG Image (*.svg)\0*.svg\0";
	auto [outpath, outtype] = saveFileName(app->m_window.getSystemHandle(), filter, app->m_projName);

	if (outpath.empty())
		return false;

	std::string source = "user/" + app->m_projName;
	std::string output = outpath.replace_extension().generic_u8string();

	if (!outHTML(app->m_page, app->m_title, app->m_subtitle, source))
		return false;

	switch (outtype)
	{
	case 1:
		return outPDF(source, output);
	case 2:
		return outHTML(app->m_page, app->m_title, app->m_subtitle, output, true);
	/*
	case 3:
		outIMG(source, output, "png");
		break;
	case 4:
		outIMG(source, output, "jpg");
		break;
	case 5:
		outIMG(source, output, "svg");
		break;
	*/
	default:
		return false;
	}
}

bool Serializer::importFile(Application* app)
{
	std::vector<std::string> filenames;
	filenames.emplace_back("empty");
	for (const auto& entry : fs::directory_iterator("user"))
	{
		const std::string stem = entry.path().stem().generic_string();
		if (stem != "empty")
			filenames.push_back(stem);
	}

	sf::RectangleShape background;
	background.setPosition(700, 300);
	background.setSize(sf::Vector2f(520, 200.f + 40.f * filenames.size()));
	background.setFillColor(sf::Color(40, 40, 60));
	background.setOutlineThickness(3);
	background.setOutlineColor(sf::Color(65, 65, 90));

	std::vector<Button> buttons;
	buttons.emplace_back(sf::FloatRect(720, 320, 480, 40), "empty", app->m_font);
	for (size_t i = 0; i < filenames.size(); i++)
		buttons.emplace_back(sf::FloatRect(720, 380.f + 40.f * i, 480, 40), filenames[i], app->m_font);

	sf::Event event;
	while (app->m_window.waitEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			app->m_window.close();
			return false;

		case sf::Event::MouseButtonPressed:
			for (size_t i = 0; i < buttons.size(); i++)
			{
				if (buttons[i].mouseClick(event.mouseButton.x, event.mouseButton.y))
				{
					if (inHTML(app->m_page, app->m_title, app->m_subtitle, ("user/" + filenames[i] + ".html").c_str()))
					{
						app->m_newProject = (filenames[i] == "empty");
						app->m_projName = filenames[i];
						app->m_boot = false;
						app->m_scrollbar = app->m_window.getDefaultView();
						app->m_titleEditor.setText(app->m_title);
						app->m_subEditor.setText(app->m_subtitle);
						return true;
					}
				}
			}
			break;
		case sf::Event::MouseMoved:
			for (auto& button : buttons)
				button.mouseMove(event.mouseMove.x, event.mouseMove.y);
			break;

		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
			{
				if (app->m_boot)
				{
					app->m_boot = false;
					app->m_titleEditor.setText(app->m_title);
					app->m_subEditor.setText(app->m_subtitle);
					return inHTML(app->m_page, app->m_title, app->m_subtitle, "user/empty.html");
				}
				else return false;
			}
			break;
		}

		app->render();
		app->m_window.draw(background);
		for (const auto& button : buttons)
			app->m_window.draw(button);
		app->m_window.display();
	}

	return false;
}