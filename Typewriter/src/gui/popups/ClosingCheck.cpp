#include "PCH.h"
#include "ClosingCheck.h"

#include "../widgets/Button.h"


CloseMessage closeCheckPopup(sf::RenderWindow& window)
{
	sf::View view(sf::FloatRect(sf::Vector2f(0, 0), (sf::Vector2f)window.getSize()));
	sf::RectangleShape background;
	sf::Text warning;
	Button quitSave;
	Button quitLose;
	Button cancel;

	background.setPosition(700, 300);
	background.setSize(sf::Vector2f(520, 340));
	background.setFillColor(sf::Color(40, 40, 60));
	background.setOutlineThickness(3);
	background.setOutlineColor(sf::Color(10, 10, 15));

	quitSave.create(sf::FloatRect(740, 440, 440, 40), "Save and quit");
	quitSave.setView(view);

	quitLose.create(sf::FloatRect(740, 500, 440, 40), "Quit without saving");
	quitLose.setView(view);

	cancel.create(sf::FloatRect(740, 560, 440, 40), "Cancel");
	cancel.setView(view);

	sf::Event event;
	while (window.isOpen())
	{
		while (window.waitEvent(event))
		{
			quitSave.handleEvent(event);
			quitLose.handleEvent(event);
			cancel.handleEvent(event);

			switch (event.type)
			{
			case sf::Event::MouseButtonPressed:
				if (quitSave.getClick())
					return CloseMessage::SaveClose;
				if (quitLose.getClick())
					return CloseMessage::LoseClose;
				if (cancel.getClick())
					return CloseMessage::Cancel;
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					return CloseMessage::Cancel;
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
			window.draw(quitSave);
			window.draw(quitLose);
			window.draw(cancel);

			window.display();
		}
	}

	return CloseMessage::Cancel;
}