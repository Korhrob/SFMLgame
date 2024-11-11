#include <SFML/Graphics.hpp>
#include "Upgrade.hpp"
#include "Game.hpp"
#include "Constants.h"

#include <string>

void	Upgrade::randomize(Game* game)
{
	m_type = static_cast<eUpgradeType>(rand() % (TYPE_MAX));

	switch (m_type)
	{
		case TYPE_SWORD:
			m_text.setString(name_sword);
			m_desc.setString(desc_sword);
		break;
		case TYPE_FIREBALL:
			m_text.setString(name_staff);
			m_desc.setString(desc_staff);
		break;
		case TYPE_BOW:
			m_text.setString(name_bow);
			m_desc.setString(desc_bow);
		break;
		case TYPE_CROSSBOW:
			m_text.setString(name_crossbow);
			m_desc.setString(desc_crossbow);
		break;
		default:
			m_text.setString("placeHolder");
		break;
	}

	m_text.setPosition(m_pos);
	m_desc.setPosition(m_pos);

	// alignment
	sf::Vector2f offset;

	offset = sf::Vector2f(-m_text.getGlobalBounds().width / 2, 0);
	m_text.setPosition(m_pos + offset);

	offset = sf::Vector2f(-m_desc.getGlobalBounds().width / 2, 48);
	m_desc.setPosition(m_pos + offset);

	offset = sf::Vector2f(-m_hotkey.getGlobalBounds().width / 2, 240 - 64);
	m_hotkey.setPosition(m_pos + offset);

	m_sprite.setTexture(*game->getShopTexture(m_type));
	m_sprite.setScale(2.0f, 2.0f);


}

void	Upgrade::initialize(const sf::Font& font, const sf::Vector2f& pos, const int& id)
{
	m_pos = pos;
	m_text.setFont(font);
	m_text.setFillColor(sf::Color::White);
	m_text.setStyle(sf::Text::Bold);

	m_desc.setFont(font);
	m_desc.setCharacterSize(20);
	m_desc.setFillColor(sf::Color::White);
	m_desc.setStyle(sf::Text::Bold);

	m_hotkey.setFont(font);
	m_hotkey.setStyle(sf::Text::Bold);
	m_hotkey.setString("< " + std::to_string(id) + " >");

	m_level.setFont(font);
	m_level.setStyle(sf::Text::Bold);
	m_level.setString("Level: 0");
	sf::Vector2f offset = sf::Vector2f(ScreenWidth / 5.0f / 2.0f -m_level.getGlobalBounds().width / 2, -142);
	m_level.setPosition(pos + offset);

	const float padding = 32.0f;

	m_background = sf::RectangleShape(sf::Vector2f(ScreenWidth / 5.0f - padding, 480.0f));
	m_background.setFillColor(sf::Color(0, 0, 0, 100));
	offset = sf::Vector2f(padding / 2, -240);
	m_background.setPosition(m_pos + offset);

	offset = sf::Vector2f(ScreenWidth / 5.0f / 2.0f, 0);
	m_pos += offset;

	offset = sf::Vector2f(0, -64);
	m_sprite.setOrigin(16.0f, 16.0f);
	m_sprite.setScale(1.0f, 1.0f);
	m_sprite.setPosition(m_pos + offset);

}

void	Upgrade::draw(sf::RenderTarget& target) const
{
	target.draw(m_background);
	target.draw(m_sprite);
	target.draw(m_text);
	target.draw(m_desc);
	target.draw(m_hotkey);
	target.draw(m_level);
}

