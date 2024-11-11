#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "Projectile.hpp"
#include "UpgradeType.h"
#include "Constants.h"
#include "Game.hpp"
#include "Vampire.hpp"

#include <cmath>

class Game;

Projectile::Projectile(Game* game, sf::Vector2f position, eUpgradeType type, sf::Texture* tex) :
    Rectangle(sf::Vector2f(ProjectileWidth, ProjectileHeight)),
    m_pGame(game),
	m_type(type),
	m_speed(400.0f)
{
    setPosition(position);
    setOrigin(0, 0);
    setActive(true);

    m_sprite.setTexture(*tex);
	m_sprite.setScale(1.0f, 1.0f);
	m_sprite.setPosition(position);
	m_sprite.setOrigin(sf::Vector2f(ProjectileWidth / 2, ProjectileHeight / 2));

	switch (type)
	{
		case eUpgradeType::TYPE_SWORD:
		{
			m_speed = SwordSwingSpeed;
			m_timer = 0.75f;
			m_sprite.setScale(2.0f, 2.0f);
		}
		break;
		case eUpgradeType::TYPE_FIREBALL:
		{
			m_speed = FireballSpeed;
			m_timer = 2.0f;
		}
		break;
		case eUpgradeType::TYPE_BOW:
		{
			m_speed = ArrowSpeed;
			m_timer = 0.0f;
		}
		break;
		case eUpgradeType::TYPE_CROSSBOW:
		{
			m_speed = XbowArrowSpeed;
			m_timer = 2.5f;
			m_sprite.setScale(0.5f, 0.5f);
		}
		break;

		default:
		{
			m_timer = 0.0f;
		}
	}
}

void Projectile::update(float deltaTime)
{

	if (m_timer > 0.0f)
		m_timer -= deltaTime;

	if (checkCollision() || m_timer < 0.0f)
	{
		setActive(false);
		return;
	}

    sf::Transformable::move(m_direction * m_speed * deltaTime);
    m_sprite.setPosition(getPosition());

	if (m_type == eUpgradeType::TYPE_FIREBALL)
		m_sprite.rotate(400.0f * deltaTime);
}

void Projectile::setActive(bool isActive)
{
    m_isActive = isActive;

	if (!isActive && m_type == eUpgradeType::TYPE_FIREBALL)
	{
		std::vector<Vampire*> aoe = m_pGame->getVampiresInRange(getCenter(), 128);
		for (auto& temp : aoe)
		{
			temp->takeDamage();
		}
	}
}

void Projectile::setDirection(const sf::Vector2f& direction)
{
	m_direction = direction;

	float radian = std::atan2(direction.y, direction.x);
	m_angle = radian * 180.0f / M_PI + 90.0f;

	if (m_angle < 0)
		m_angle += 360.0f;

	m_sprite.setRotation(m_angle);
}

bool Projectile::checkCollision()
{
	Vampire* target = m_pGame->getClosestVampire(getCenter(), 32.0f);
	
	if (target == nullptr)
		return false;

	if (!this->collidesWith(target))
		return false;

	switch (m_type)
	{
		case eUpgradeType::TYPE_SWORD:
		case eUpgradeType::TYPE_CROSSBOW:
		{
			// affected by pierce upgrade
			target->takeDamage();
		}
		break;
		case eUpgradeType::TYPE_FIREBALL:
		{
			// foreach in range
		}
		break;
		default:
		{
			target->takeDamage();
			setActive(false);
		}
	}

	return true;
}