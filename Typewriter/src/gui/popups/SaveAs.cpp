#include "PCH.h"
#include "SaveAs.h"

#include "../widgets/EditBox.h"
#include "../widgets/Button.h"


std::string saveAsPopup(sf::RenderWindow& window)
{
	sf::View view(sf::FloatRect(sf::Vector2f(0, 0), (sf::Vector2f)window.getSize()));
	sf::RectangleShape background;
	EditBox nameBox;
	Button confirm;

	background.setPosition(700, 300);
	background.setSize(sf::Vector2f(520, 200));
	background.setFillColor(sf::Color(40, 40, 60));
	background.setOutlineThickness(3);
	background.setOutlineColor(sf::Color(10, 10, 15));

	nameBox.create(sf::FloatRect(720, 320, 480, 50));
	nameBox.setFocused(true);
	nameBox.setView(view);

	confirm.create(sf::FloatRect(800, 400, 320, 40), "Save As");
	confirm.setView(view);

	sf::Event event;
	while (window.isOpen())
	{
		while (window.waitEvent(event))
		{
			nameBox.handleEvent(event);
			confirm.handleEvent(event);

			switch (event.type)
			{
			case sf::Event::MouseButtonPressed:
				if (confirm.getClick())
				{
					if (nameBox.getText() == "")
						break;

					return nameBox.getText();
				}
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
			window.draw(nameBox);
			window.draw(confirm);

			window.display();
		}
	}

	return "";
}