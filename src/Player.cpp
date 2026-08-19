#include "Player.h"

#include <iostream>

Player::Player()
    : Character("Hero", 100, 20),
      level(1),
      experience(0),
      coins(0),
      potions(2),
      hasKey(false),
      enemiesKilled(0),
      position{1, 1}
{
}

std::string Player::getRole() const
{
    return "PLAYER";
}

char Player::getSymbol() const
{
    return '@';
}

std::string Player::getDescription() const
{
    return "Главный герой подземелья";
}

int Player::getLevel() const
{
    return level;
}

int Player::getExperience() const
{
    return experience;
}

int Player::getCoins() const
{
    return coins;
}

int Player::getPotions() const
{
    return potions;
}

bool Player::getKey() const
{
    return hasKey;
}

int Player::getEnemiesKilled() const
{
    return enemiesKilled;
}

void Player::addCoins(int amount)
{
    if (amount > 0)
        coins += amount;
}

void Player::addPotion()
{
    ++potions;
}

void Player::addEnemyKill()
{
    ++enemiesKilled;
}

void Player::obtainKey()
{
    hasKey = true;
}

bool Player::usePotion()
{
    if (potions <= 0)
    {
        std::cout << "\nУ тебя нет зелий!\n";
        return false;
    }

    if (hp == maxHp)
    {
        std::cout << "\nЗдоровье уже максимальное!\n";
        return false;
    }

    --potions;

    const int oldHp = hp;
    heal(30);

    std::cout << "\nТы использовал зелье.\n";
    std::cout << "Восстановлено HP: "
              << hp - oldHp << "\n";

    return true;
}

void Player::addExperience(int amount)
{
    if (amount <= 0)
        return;

    experience += amount;

    while (experience >= level * 100)
    {
        experience -= level * 100;
        ++level;

        increaseMaxHP(20);
        hp = maxHp;
        increaseAttack(5);

        std::cout << "\n=== НОВЫЙ УРОВЕНЬ! ===\n";
        std::cout << "Теперь ты " << level << " уровня!\n";
        std::cout << "Максимальное HP: " << maxHp << "\n";
        std::cout << "Атака: " << attackPower << "\n";
    }
}

void Player::setLevel(int value)
{
    if (value >= 1)
        level = value;
}

void Player::setExperience(int value)
{
    if (value >= 0)
        experience = value;
}

void Player::setCoins(int value)
{
    if (value >= 0)
        coins = value;
}

void Player::setPotions(int value)
{
    if (value >= 0)
        potions = value;
}

void Player::setKey(bool value)
{
    hasKey = value;
}

void Player::setEnemiesKilled(int value)
{
    if (value >= 0)
        enemiesKilled = value;
}

void Player::setPosition(Position value)
{
    position = value;
}