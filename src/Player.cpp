#include "Player.hpp"
#include "Weapon.hpp"
#include "InputHandler.hpp"
#include "Constants.h"
#include <vector>
#include "Game.hpp"
#include "MathUtils.h"
#include "UpgradeType.h"
#include "Projectile.hpp"
#include "Vampire.hpp"

#include <iostream>

Player::Player(Game* pGame) :
    Rectangle(sf::Vector2f(PlayerWidth, PlayerHeight)),
    m_pGame(pGame),
    m_pWeapon(std::make_unique<Weapon>())
{
    setOrigin(sf::Vector2f(PlayerWidth / 2, PlayerHeight / 2));
}

bool Player::initialise()
{
    m_sprite.setTexture(*m_pGame->getPlayerTexture());
    m_sprite.setScale(3.5f, 3.5f);
    setIsDead(false);
    setPosition(ScreenWidth / 2, ScreenHeight / 2);
    m_sprite.setPosition(getPosition());
	m_pWeapon->setActive(false);

	for (int i = 0; i < eUpgradeType::TYPE_MAX; i++)
	{
		m_upgradeLevel[i] = 0;
		if (i <= eUpgradeType::TYPE_CROSSBOW)
			m_weaponCooldown[i] = DefaultCooldowns[i];
	}

	m_upgradeLevel[eUpgradeType::TYPE_SWORD] = 1;

    return true;
}

void Player::move(InputData inputData, float deltaTime)
{
    float xSpeed = 0.0f;
    float ySpeed = 0.0f;
    
    xSpeed -= inputData.m_movingLeft;
    xSpeed += inputData.m_movingRight;

    ySpeed -= inputData.m_movingUp;
    ySpeed += inputData.m_movingDown;

	sf::Vector2f direction = VecNormalized(sf::Vector2f(xSpeed, ySpeed));
    sf::Transformable::move(direction * PlayerSpeed * deltaTime);
    setPosition(
		std::clamp(getPosition().x, 0.0f,(float)ScreenWidth - m_sprite.getGlobalBounds().width), 
		std::clamp(getPosition().y, 0.0f,(float)ScreenHeight - m_sprite.getGlobalBounds().height));

    if (m_pWeapon->isActive() == false)
    {
        if (inputData.m_movingLeft == true)
            m_direction = LEFT;
        else
            m_direction = RIGHT;
    }
}

void Player::attack()
{
    m_pWeapon->setActive(true);
}

void Player::update(float deltaTime)
{
    sf::Vector2f weaponSize = m_pWeapon->getSize();

    m_sprite.setPosition(getPosition());
    m_pWeapon->setPosition(sf::Vector2f(
        getCenter().x - (m_direction == LEFT ? weaponSize.x : 0.0f),
        getCenter().y - weaponSize.y / 2.0f));
    m_pWeapon->update(deltaTime);

	for (int i = 0; i <= eUpgradeType::TYPE_CROSSBOW; i++)
	{
		eUpgradeType type = static_cast<eUpgradeType>(i);

		if (m_upgradeLevel[type] > 0)
		{
			shootProjectile(deltaTime, type);
		}
	}

}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    Rectangle::draw(target, states);
    m_pWeapon->draw(target, states);
}

void Player::shootProjectile(float deltaTime, eUpgradeType type)
{
	if (m_weaponTimer[type] > 0.f)
	{
		m_weaponTimer[type] -= deltaTime;
		return;
	}

	float range = 400;

	switch (type)
	{
		default:
		case eUpgradeType::TYPE_SWORD:
			range = 200;
		break;
		case eUpgradeType::TYPE_FIREBALL:
		case eUpgradeType::TYPE_CROSSBOW:
			range = 400;
		break;
		case eUpgradeType::TYPE_BOW:
			range = 800;
		break;
	}

	Vampire* target = nullptr;

	switch (type)
	{
		default:
			target = m_pGame->getClosestVampire(getCenter(), range);
		break;
		case eUpgradeType::TYPE_BOW:
			target = m_pGame->getFurthestVampire(getCenter(), range);
		break;
	}

	if (target == nullptr)
		return;

	Projectile* projectile = m_pGame->createProjectile(getCenter(), type);

	if (projectile == nullptr)
	 	return;

	projectile->setDirection(VecNormalized(target->getCenter() - getCenter()));
	m_weaponTimer[type] = getCooldown(type);
}