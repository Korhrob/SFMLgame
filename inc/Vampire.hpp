#pragma once

#include "Rectangle.hpp"

class Game;

class Vampire : public Rectangle
{
public:
    Vampire(Game* game, sf::Vector2f position);
    virtual ~Vampire() {}
    
    void update(float deltaTime);

	void takeDamage();
    void setIsKilled(bool isKilled) { m_isKilled = isKilled; }
    bool isKilled() { return m_isKilled; }

private:
    Game* m_pGame;
    bool m_isKilled = false;
	int m_maxHealth;
	int m_curHealth;
};