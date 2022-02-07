#include "PCH.h"
#include "FileList.h"

#include "../widgets/Button.h"


std::string fileListPopup(sf::RenderWindow& window)
{
	sf::View view(sf::FloatRect(sf::Vector2f(0, 0), (sf::Vector2f)window.getSize()));
	sf::RectangleShape background;
	std::vector<std::string> filenames{ "empty" };
	std::vector<Button> buttons;
	Button confirm;
	Button remove;

	size_t selected = 0;

	for (const auto& entry : fs::directory_iterator("user"))
	{
		const std::string stem = entry.path().stem().generic_string();
		if (stem != "empty")
			filenames.push_back(stem);
	}

	background.setPosition(700, 300);
	background.setFillColor(sf::Color(40, 40, 60));
	background.setOutlineThickness(3);
	background.setOutlineColor(sf::Color(65, 65, 90));
	background.setSize(sf::Vector2f(520, 200.f + 40.f * filenames.size()));

	Button& e = buttons.emplace_back();
	e.create(sf::FloatRect(720, 320, 480, 40), "Empty");
	e.setView(view);

	for (size_t i = 1; i < filenames.size(); i++)
	{
		Button& b = buttons.emplace_back();
		b.create(sf::FloatRect(720, 340.f + 40.f * i, 480, 40), filenames[i]);
		b.setView(view);
	}

	confirm.create(sf::FloatRect(720.f, 450.f + buttons.size() * 40.f, 200.f, 40.f), "Open");
	confirm.setView(view);
	remove.create(sf::FloatRect(1000.f, 450.f + buttons.size() * 40.f, 200.f, 40.f), "Delete");
	remove.setView(view);

	sf::Event event;
	while (window.isOpen())
	{
		while (window.waitEvent(event))
		{
			for (auto& button : buttons)
				button.handleEvent(event);
			confirm.handleEvent(event);
			remove.handleEvent(event);

			switch (event.type)
			{
			case sf::Event::MouseButtonPressed:
				for (size_t i = 0; i < buttons.size(); i++)
				{
					if (buttons[i].getClick())
						selected = i;
				}
				if (confirm.getClick())
					return filenames[selected];
				else if (remove.getClick())
				{}
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					return "";
				break;

			case sf::Event::Resized:
				view.setCenter(960, static_cast<float>(event.size.height) * 0.5f);
				view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
				if (event.size.width < 1010)
					window.setSize(sf::Vector2u(1010, window.getSize().y));
				break;
			}

			window.setView(view);

			window.draw(background);
			for (const auto& button : buttons)
				window.draw(button);
			window.draw(confirm);
			window.draw(remove);

			window.display();
		}
	}

	return "";
}