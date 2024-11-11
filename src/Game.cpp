#include "Game.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

#include "ResourceManager.hpp"
#include "InputHandler.hpp"
#include "Weapon.hpp"
#include "Player.hpp"
#include "Rectangle.hpp"
#include "Vampire.hpp"
#include "Upgrade.hpp"
#include "UpgradeType.h"

#include <cmath>

Game::Game() :
    m_state(State::WAITING),
    m_pClock(std::make_unique<sf::Clock>()),
    m_pPlayer(std::make_unique<Player>(this)),
    m_vampireCooldown(2.0f),
    m_nextVampireCooldown(2.0f)
{
    m_pGameInput = std::make_unique<GameInput>(this, m_pPlayer.get());
}

Game::~Game()
{
}

bool Game::initialise()
{
    /*
        PAY ATTENTION HIVER!
            If you want to load any assets (fonts, textures) please use the pattern shown below
    */

    if (!m_font.loadFromFile(ResourceManager::getFilePath("Lavigne.ttf")))
    {
        std::cerr << "Unable to load font" << std::endl;
        return false;
    }
    if (!m_vampTexture.loadFromFile(ResourceManager::getFilePath("vampire.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
    if (!m_playerTexture.loadFromFile(ResourceManager::getFilePath("player.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }

	// temporary textures
    if (!m_tempTexture.loadFromFile(ResourceManager::getFilePath("temp.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }

	for (int i = 0; i < eUpgradeType::TYPE_MAX; i++)
		m_shopTextures[i] = m_tempTexture;

	if (!m_shopTextures[eUpgradeType::TYPE_SWORD].loadFromFile(ResourceManager::getFilePath("sword.png")))
	{
        std::cerr << "Unable to load texture" << std::endl;
        return false;
	}
	if (!m_shopTextures[eUpgradeType::TYPE_FIREBALL].loadFromFile(ResourceManager::getFilePath("fireball.png")))
	{
        std::cerr << "Unable to load texture" << std::endl;
        return false;
	}
	if (!m_shopTextures[eUpgradeType::TYPE_BOW].loadFromFile(ResourceManager::getFilePath("bow.png")))
	{
        std::cerr << "Unable to load texture" << std::endl;
        return false;
	}
	if (!m_shopTextures[eUpgradeType::TYPE_CROSSBOW].loadFromFile(ResourceManager::getFilePath("bow.png")))
	{
        std::cerr << "Unable to load texture" << std::endl;
        return false;
	}

	// initialize shop
	for (int i = 0; i < 3; i++)
		m_upgradeShop[i].initialize(
			m_font,
			sf::Vector2f((ScreenWidth / 5.f * i) + (ScreenWidth / 5.f),
			ScreenHeight / 2.f),
			i + 1
		);

    resetLevel();
    return true;
}

void Game::resetLevel()
{
	m_score = 0;

	m_killCount = 0;
	m_spawnCount = 0;
	m_vampireCooldown = 2.0f;
    m_nextVampireCooldown = 2.0f;
    m_pVampires.clear();
	m_pProjectiles.clear();

    m_pPlayer->initialise();
    m_pClock->restart();
}

void Game::update(float deltaTime)
{
    switch (m_state)
    {
        case State::WAITING:
        {
            if (m_pGameInput->anyKey())
            {
                m_state = State::ACTIVE;
                m_pClock->restart();
				deltaTime = 0;
            }
        }
        break;
            
        case State::ACTIVE:
        {
            m_pGameInput->update(deltaTime);
            m_pPlayer->update(deltaTime);

            vampireSpawner(deltaTime);

            for (auto& temp : m_pVampires)
            {
                temp->update(deltaTime);
            }

            for (auto& temp : m_pProjectiles)
            {
                temp->update(deltaTime);
            }

            if (m_pPlayer->isDead())
            {
                m_state = State::WAITING;
                resetLevel();
            }
        }
		break;

		case State::UPGRADE:
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
				m_pPlayer->increaseUpgrade(m_upgradeShop[0].getType());
				m_state = State::ACTIVE;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
				m_pPlayer->increaseUpgrade(m_upgradeShop[1].getType());
				m_state = State::ACTIVE;
			} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
				m_pPlayer->increaseUpgrade(m_upgradeShop[2].getType());
				m_state = State::ACTIVE;
			}
		}
        break;
    }

    int i = 0;
    while (i < m_pVampires.size())
    {
        if (m_pVampires[i]->isKilled())
        {
            std::swap(m_pVampires[i], m_pVampires.back());
            m_pVampires.pop_back();
            continue;
        }
        i++;
    }

	i = 0;
    while (i < m_pProjectiles.size())
    {
        if (!m_pProjectiles[i]->isActive())
        {
            std::swap(m_pProjectiles[i], m_pProjectiles.back());
            m_pProjectiles.pop_back();
            continue;
        }
        i++;
    }
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    //  Draw texts.
    if (m_state == State::WAITING)
    {
        sf::Text startText;
        startText.setFont(m_font);
        startText.setString("Use WASD to move, survive!");
        startText.setFillColor(sf::Color::White);
        startText.setPosition(ScreenWidth / 2, 80.0f);
		startText.move(sf::Vector2f(-startText.getGlobalBounds().width / 2, 0));
        startText.setStyle(sf::Text::Bold);
        target.draw(startText);
    }
    else
    {
        sf::Text scoreText;
        scoreText.setFont(m_font);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setStyle(sf::Text::Bold);
        scoreText.setString("Score: " + std::to_string(m_score));
        scoreText.setPosition(sf::Vector2f((ScreenWidth - scoreText.getLocalBounds().getSize().x) * 0.5, 20));
        target.draw(scoreText);
    }

    // Draw player.
    m_pPlayer->draw(target, states);

    //  Draw world.
    for (auto& temp : m_pVampires)
    {
        temp->draw(target, states);
    }

    for (auto& temp : m_pProjectiles)
    {
        temp->draw(target, states);
    }

	if (m_state == State::UPGRADE)
	{
		for (int i = 0; i < 3; i++)
			m_upgradeShop[i].draw(target);
	}
}


void Game::onKeyPressed(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyPressed(key);
}

void Game::onKeyReleased(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyReleased(key);
}

Player* Game::getPlayer() const 
{
    return m_pPlayer.get();
}

void Game::vampireSpawner(float deltaTime)
{
    if (m_vampireCooldown > 0.0f)
    {
        m_vampireCooldown -= deltaTime;
        return;
    }

    float randomXPos = rand() % ScreenWidth;
    float randomYPos = rand() % ScreenHeight;

    float distToRight = (float) ScreenWidth - randomXPos;
    float distToBottom = (float) ScreenHeight - randomYPos;

    float xMinDist = randomXPos < distToRight ? -randomXPos : distToRight;
    float yMinDist = randomYPos < distToBottom ? -randomYPos : distToBottom;

    if (std::abs(xMinDist) < std::abs(yMinDist))
        randomXPos += xMinDist;
    else
        randomYPos += yMinDist;

    sf::Vector2f spawnPosition = sf::Vector2f(randomXPos, randomYPos);
    m_pVampires.push_back(std::make_unique<Vampire>(this, spawnPosition));

    m_spawnCount++;
    if (m_nextVampireCooldown > 0.1f && m_killCount % 6 == 0)
    {
		m_killCount = 0;
        m_nextVampireCooldown -= 0.1f;
		std::cout << "difficulty increased: " << m_nextVampireCooldown << std::endl; 
    }
    m_vampireCooldown = std::clamp(m_nextVampireCooldown, 0.05f, 5.0f);
}

void Game::openUpgradeMenu()
{
	m_state = State::UPGRADE;

	for (int i = 0; i < 3; i++)
	{
		m_upgradeShop[i].randomize(this);
		m_upgradeShop[i].setUpgradeLevel(m_pPlayer->getUpgradeLevel(m_upgradeShop[i].getType()));
	}

}

Vampire* Game::getClosestVampire(const sf::Vector2f& pos, const float& range)
{
	Vampire* ptr = nullptr;
	float	closest = range;

	for (int i = 0; i < m_pVampires.size(); i++)
	{
		if (m_pVampires[i]->isKilled())
			continue;

		sf::Vector2f vPos = m_pVampires[i]->getCenter();
		float distance = std::sqrt((pos.x - vPos.x) * (pos.x - vPos.x) + (pos.y - vPos.y) * (pos.y - vPos.y));

		if (distance > range)
			continue;

		if (distance < closest)
		{
			closest = distance;
			ptr = m_pVampires[i].get();
		}
	}
	return ptr;
}


Vampire* Game::getFurthestVampire(const sf::Vector2f& pos, const float& range)
{
	Vampire* ptr = nullptr;
	float	closest = 0;

	for (int i = 0; i < m_pVampires.size(); i++)
	{
		if (m_pVampires[i]->isKilled())
			continue;

		sf::Vector2f vPos = m_pVampires[i]->getCenter();
		float distance = std::sqrt((pos.x - vPos.x) * (pos.x - vPos.x) + (pos.y - vPos.y) * (pos.y - vPos.y));

		if (distance > range)
			continue;

		if (distance > closest)
		{
			closest = distance;
			ptr = m_pVampires[i].get();
		}
	}
	return ptr;
}

std::vector<Vampire*> Game::getVampiresInRange(const sf::Vector2f& pos, const float& range)
{
	std::vector<Vampire*> collection;

	for (int i = 0; i < m_pVampires.size(); i++)
	{
		if (m_pVampires[i]->isKilled())
			continue;

		sf::Vector2f vPos = m_pVampires[i]->getCenter();
		float distance = std::sqrt((pos.x - vPos.x) * (pos.x - vPos.x) + (pos.y - vPos.y) * (pos.y - vPos.y));

		if (distance < range)
			collection.push_back(m_pVampires[i].get());
	}
	return collection;
}

Projectile*	Game::createProjectile(const sf::Vector2f& pos, eUpgradeType type)
{
	auto p = std::make_unique<Projectile>(this, pos, type, getProjectileTexture(type));
	Projectile* ptr = p.get();
	m_pProjectiles.push_back(std::move(p));
	return ptr;
}

void Game::increaseScore()
{
	m_killCount++;
	m_score++;

	int interval;
	if (m_score <= 50) {
		interval = 5;
	} else {
		interval = 10;
	}

	if (m_score % interval == 0)
		openUpgradeMenu();
}
