#include "Vampire.hpp"
#include "Constants.h"
#include "Game.hpp"
#include "Weapon.hpp"
#include "Player.hpp"
#include "MathUtils.h"
#include "Game.hpp"

Vampire::Vampire(Game* game, sf::Vector2f position) :
    Rectangle(sf::Vector2f(VampireWidth, VampireHeight)),
    m_pGame(game),
	m_maxHealth(1),
	m_curHealth(m_maxHealth)
{
    setPosition(position);
    setOrigin(sf::Vector2f(VampireWidth / 2, VampireHeight / 2));
    setIsKilled(false);

    m_sprite.setTexture(*m_pGame->getVampireTexture());
    m_sprite.setScale(2.0f, 2.0f);
}

void Vampire::update(float deltaTime)
{
    if (m_isKilled)
        return;
    
    Player* pPlayer = m_pGame->getPlayer();

    if (collidesWith(pPlayer->getWeapon()))
    {
        setIsKilled(true);
        return;
    }

    if (collidesWith(pPlayer))
        pPlayer->setIsDead(true);

    sf::Vector2f playerCenter = pPlayer->getCenter();
    sf::Vector2f direction = VecNormalized(playerCenter - getCenter());
    sf::Transformable::move(direction * VampireSpeed * deltaTime);
    m_sprite.setPosition(getPosition());
}

void Vampire::takeDamage()
{
	setIsKilled(true);
	m_pGame->increaseScore();
}