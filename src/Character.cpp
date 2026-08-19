#include "Character.h"

Character::Character(
    const std::string& characterName,
    int health,
    int attack)
    : name(characterName),
      hp(health),
      maxHp(health),
      attackPower(attack)
{
}

std::string Character::getDescription() const
{
    return name;
}

bool Character::isAlive() const
{
    return hp > 0;
}

int Character::getHP() const
{
    return hp;
}

int Character::getMaxHP() const
{
    return maxHp;
}

int Character::getAttack() const
{
    return attackPower;
}

std::string Character::getName() const
{
    return name;
}

void Character::takeDamage(int damage)
{
    if (damage <= 0)
        return;

    hp -= damage;

    if (hp < 0)
        hp = 0;
}

void Character::heal(int amount)
{
    if (amount <= 0)
        return;

    hp += amount;

    if (hp > maxHp)
        hp = maxHp;
}

void Character::setHP(int value)
{
    if (value < 0)
        value = 0;

    if (value > maxHp)
        value = maxHp;

    hp = value;
}

void Character::setMaxHP(int value)
{
    if (value > 0)
    {
        maxHp = value;

        if (hp > maxHp)
            hp = maxHp;
    }
}

void Character::setAttack(int value)
{
    if (value > 0)
        attackPower = value;
}

void Character::increaseMaxHP(int amount)
{
    if (amount > 0)
        maxHp += amount;
}

void Character::increaseAttack(int amount)
{
    if (amount > 0)
        attackPower += amount;
}