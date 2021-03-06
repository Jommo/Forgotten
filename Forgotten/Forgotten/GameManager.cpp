#include "GameManager.h"

GameManager::GameManager()
	:
	m_level1("Data/Levels/Level1_walkData.png", 50, 20, "Data/Levels/FGN_kapitel_1_hall.png"),
	m_currentLevel(&m_level1)
{
	// Create the player
	m_player = new Player(m_currentLevel->GetNodeMap());
	m_player->SetNodePosition(6, 24);

	// Set the view size
	m_view.setSize(1024, 576);
	m_view.setCenter(720, 288);

	// Save default view
	m_defaultView = m_window.getDefaultView();

	// Set the window size and other properties
	if(FULLSCREEN){
		m_window.create(sf::VideoMode(1680, 1050, 32), "Forgotten", sf::Style::Fullscreen);
	}else{
		m_window.create(sf::VideoMode(1680, 1050, 32), "Forgotten");
	}

	// Set frame limit
	m_window.setFramerateLimit(60);

	// Load debug font
	const unsigned int fontSize = 8;
	m_debugFont.loadFromFile("Data/Fonts/Minecraftia.ttf");
	m_mousePosition.setFont(m_debugFont);
	m_mousePosition.setScale(0.3, 0.4);
	m_mouseNodePosition.setFont(m_debugFont);
	m_mouseNodePosition.setScale(0.3, 0.4);
	m_fps.setFont(m_debugFont);
	m_fps.setScale(0.3, 0.4);

	// Push the player into the entityvector
	m_entities.push_back(m_player);
}

void GameManager::Process(){

	// Move player
	/*if(mouse->wasPressed()){
		// Get node coordinates,
		// player->goto(coordinates)
	}*/
	sf::Vector2f nodePos;
	sf::Vector2f mousePosition = m_window.convertCoords(sf::Mouse::getPosition(m_window));
	nodePos.x = floor(mousePosition.x / m_currentLevel->GetNodeMap().GetNodeSize().x);
	nodePos.y = floor(mousePosition.y / m_currentLevel->GetNodeMap().GetNodeSize().y);

	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		m_player->GoTo(nodePos);
	}

	// Update all entities
	for(std::vector<Entity*>::iterator i = m_entities.begin(); i != m_entities.end(); i++){
		(*i)->Update();
	}

}

void GameManager::Render(){

		// Clear the screen 
		m_window.clear(sf::Color(0, 0, 0));

		// Draw the background
		m_window.draw(m_currentLevel->GetBackgroundImage());

		// Draw all debug stuff on top layer
		if(DEBUG){

			// Set the default view for reference of mouse pointer and node drawing
			sf::Vector2f mousePosition = m_window.convertCoords(sf::Mouse::getPosition(m_window));

			// Draw the nodeMap
			if(DEBUG_NODE){

				sf::RectangleShape nodeRect;
				
				for(int x = 0; x < m_currentLevel->GetNodeMap().GetMapSize().x; x++){
					for(int y = 0; y < m_currentLevel->GetNodeMap().GetMapSize().y; y++){
						// Draw a rectangle for each node/tile 
						nodeRect.setPosition(x * m_currentLevel->GetNodeMap().GetNodeSize().x, y * m_currentLevel->GetNodeMap().GetNodeSize().y);
						nodeRect.setSize(sf::Vector2f(m_currentLevel->GetNodeMap().GetNodeSize()));
						nodeRect.setOutlineColor(sf::Color::Black);
						nodeRect.setOutlineThickness(1);
						if(m_currentLevel->GetNodeMap().isWalkable(x, y)){
							nodeRect.setFillColor(sf::Color(0, 255, 0, 80));
						}else{
							nodeRect.setFillColor(sf::Color(255, 0, 0, 100));
						}
						m_window.draw(nodeRect);
					}
				}
				
			}

			// Mouse position
			m_debugStream.str("");
			m_debugStream << "Mouse position:\n";
			m_debugStream << "X: " << mousePosition.x << " Y: " << mousePosition.y;
			m_mousePosition.setString(m_debugStream.str());
			m_mousePosition.setPosition(10, 10);

			// Mouse node position
			m_debugStream.str("");
			m_debugStream << "Mouse node position:\n";
			m_debugStream << "X: " << floor(mousePosition.x / m_currentLevel->GetNodeMap().GetNodeSize().x) << " Y: " << floor(mousePosition.y / m_currentLevel->GetNodeMap().GetNodeSize().y);
			m_mouseNodePosition.setString(m_debugStream.str());
			m_mouseNodePosition.setPosition(10, 50);

			// Frames per second
			float fps = floor(1.0f/m_debugClock.getElapsedTime().asSeconds());
			m_debugClock.restart();
			m_debugStream.str("");
			m_debugStream << "FPS: " << fps;
			m_fps.setString(m_debugStream.str());
			m_fps.setPosition(10, 90);

			// Set the default view to draw text in right place
			m_window.setView(m_defaultView);

			// Debug text
			m_window.draw(m_mousePosition);
			m_window.draw(m_mouseNodePosition);
			m_window.draw(m_fps);
		}

		// Set view
		m_window.setView(m_view);

		// Draw entities
		for(std::vector<Entity*>::iterator i = m_entities.begin(); i != m_entities.end(); i++){

			m_window.draw((*i)->GetSprite());

		}

		// Display all rendered items
		m_window.display();

}

sf::RenderWindow& GameManager::GetWindow(){ return m_window; }