#pragma once

#include "Rectangle.hpp"

#include <memory>
#include "UpgradeType.h"
#include "Constants.h"

struct InputData;

class Game;
class Weapon;

enum eDirection
{
    LEFT,
    RIGHT
};

class Player : public Rectangle
{
public:
    Player(Game* pGame);
    virtual ~Player() {}
    
    bool initialise();
    void move(InputData inputData, float deltaTime);
    void attack();
    void update(float deltaTime);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool isDead() const { return m_isDead; }
    void setIsDead(bool isDead) { m_isDead = isDead; }

    Weapon* getWeapon() { return m_pWeapon.get(); }
	void increaseUpgrade(eUpgradeType type) { m_upgradeLevel[type]++; }
	float getCooldown(eUpgradeType type) { return std::clamp(DefaultCooldowns[type] - (m_upgradeLevel[type] * CooldownUpgrade), 0.05f, 5.0f); }
	int	getUpgradeLevel(eUpgradeType type) { return m_upgradeLevel[type]; }

private:
    bool    m_isDead = false;
    eDirection m_direction = LEFT;
    Game*   m_pGame;
    std::unique_ptr<Weapon> m_pWeapon;

	int m_upgradeLevel[eUpgradeType::TYPE_MAX];
	float m_weaponTimer[eUpgradeType::TYPE_MAX];
	float m_weaponCooldown[eUpgradeType::TYPE_MAX];
	void shootProjectile(float deltaTime, eUpgradeType type);

};
