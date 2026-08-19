#include "Enemy.h"

Enemy::Enemy(
    const std::string& type,
    int health,
    int attack,
    Position enemyPosition)
    : Character(type, health, attack),
      enemyType(type),
      position(enemyPosition)
{
}

std::string Enemy::getRole() const
{
    return "ENEMY";
}

char Enemy::getSymbol() const
{
    return 'E';
}

std::string Enemy::getDescription() const
{
    return enemyType + " — опасный противник";
}