#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include "Constants.h"
#include "Upgrade.hpp"
#include "UpgradeType.h"
#include "Projectile.hpp"

class Player;
class Game;
class GameInput;
class Vampire;

namespace sf { class Clock; }

class Game : public sf::Drawable
{
public:
    
    enum class State
    {
        WAITING,
        ACTIVE,
		UPGRADE,
    };
    
    Game();
    ~Game();
    
    bool initialise();
    void update(float deltaTime);
    void resetLevel();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    
    State getState() const { return m_state; }
    
    void onKeyPressed(sf::Keyboard::Key key);
    void onKeyReleased(sf::Keyboard::Key key);

    Player* getPlayer() const;
    sf::Texture* getPlayerTexture() { return &m_playerTexture; }
    sf::Texture* getVampireTexture() { return &m_vampTexture; }
	sf::Texture* getShopTexture(eUpgradeType type) { return &m_shopTextures[type]; };
	sf::Texture* getProjectileTexture(eUpgradeType type) { return &m_shopTextures[type]; };

	Vampire* getClosestVampire(const sf::Vector2f& pos, const float& range);
	Vampire* getFurthestVampire(const sf::Vector2f& pos, const float& range);
	std::vector<Vampire*> getVampiresInRange(const sf::Vector2f& pos, const float& range);
	Projectile* createProjectile(const sf::Vector2f& pos, eUpgradeType type);

    void vampireSpawner(float deltaTime);
	void openUpgradeMenu();
	void increaseScore();

	//int getPlayerUpgradeLevel(eUpgradeType type) { return m_pPlayer->getUpgradeLevel(type); }

private:
    std::unique_ptr<Player> m_pPlayer;

    std::vector<std::unique_ptr<Vampire>> m_pVampires;
	std::vector<std::unique_ptr<Projectile>> m_pProjectiles;

    State m_state;
    std::unique_ptr<sf::Clock> m_pClock;
    std::unique_ptr<GameInput> m_pGameInput;

    float m_vampireCooldown = 0.0f;
    float m_nextVampireCooldown = 2.0f;
    int m_spawnCount = 0;
    
    sf::Font m_font;
    sf::Texture m_vampTexture;
    sf::Texture m_playerTexture;
	sf::Texture m_tempTexture;
	sf::Texture	m_shopTextures[eUpgradeType::TYPE_MAX];

	Upgrade	m_upgradeShop[3];
	int		m_upgradeLevel[eUpgradeType::TYPE_MAX];
	int		m_upgradeId;

	int		m_score;
	int		m_killRequirement;
	int		m_killCount;

};
