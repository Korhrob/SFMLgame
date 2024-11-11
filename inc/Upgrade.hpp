#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "UpgradeType.h"

class Game;

class Upgrade
{
public:
	void	randomize(Game* game);
	void	initialize(const sf::Font& font, const sf::Vector2f& pos, const int& id);
	void	draw(sf::RenderTarget& target) const;
	void	setUpgradeLevel(int level) { m_level.setString("Level:" + std::to_string(level)); }
	eUpgradeType	getType() const { return m_type; };

private:
	eUpgradeType	m_type;

    sf::Text		m_text;
	sf::Text		m_desc;
	sf::Text		m_hotkey;
	sf::Text		m_level;
	sf::Vector2f	m_pos;
	sf::Sprite		m_sprite;
	sf::RectangleShape m_background;
};