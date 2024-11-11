#pragma once

#include <SFML/System.hpp>
#include "Rectangle.hpp"
#include "UpgradeType.h"

class Game;

class Projectile : public Rectangle
{
public:
    Projectile(Game* game, sf::Vector2f position, eUpgradeType type, sf::Texture* tex);
    virtual ~Projectile() {}

    void setActive(bool isActive);
    bool isActive() { return m_isActive; }
    void update(float deltaTime);
	void setDirection(const sf::Vector2f& direction);
	bool checkCollision();
	void setSprite();

private:
    bool m_isActive = false;
    float m_timer;
	float m_speed;
	float m_angle;
	sf::Vector2f m_direction;
	eUpgradeType m_type;
	Game*	m_pGame;
};