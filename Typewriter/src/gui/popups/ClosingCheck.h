#pragma once


enum class CloseMessage { LoseClose, SaveClose, Cancel };

CloseMessage closeCheckPopup(sf::RenderWindow& window);