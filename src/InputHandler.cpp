#include "InputHandler.hpp"
#include "Weapon.hpp"
#include "Player.hpp"

GameInput::GameInput(Game* pGame, Player* pPlayer) :
    m_pGame(pGame), m_pPlayer(pPlayer)
{

    
}

GameInput::~GameInput()
{
    
}

void GameInput::update(float deltaTime)
{
    if (m_inputData.hasInputs())
    {
        m_pPlayer->move(m_inputData, deltaTime);
    }

    if (m_inputData.m_space)
    {
        m_pPlayer->attack();
    }
}

void GameInput::onKeyPressed(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::W)
    	m_inputData.m_movingUp = true;
	if (key == sf::Keyboard::S)
		m_inputData.m_movingDown = true;
	if (key == sf::Keyboard::A)
    	m_inputData.m_movingLeft = true;
	if (key == sf::Keyboard::D)
		m_inputData.m_movingRight = true;
	if (key == sf::Keyboard::Space)
		m_inputData.m_space = true;
}

void GameInput::onKeyReleased(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::W)
    	m_inputData.m_movingUp = false;
	if (key == sf::Keyboard::S)
		m_inputData.m_movingDown = false;
	if (key == sf::Keyboard::A)
    	m_inputData.m_movingLeft = false;
	if (key == sf::Keyboard::D)
		m_inputData.m_movingRight = false;
	if (key == sf::Keyboard::Space)
		m_inputData.m_space = false;
}
