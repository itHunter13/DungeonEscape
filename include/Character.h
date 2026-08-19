#pragma once

#include <string>

class Character
{
protected:
    std::string name;
    int hp;
    int maxHp;
    int attackPower;

public:
    Character(const std::string& characterName, int health, int attack);
    virtual ~Character() = default;

    // Виртуальные методы позволяют использовать полиморфизм.
    virtual std::string getRole() const = 0;
    virtual char getSymbol() const = 0;
    virtual std::string getDescription() const;

    bool isAlive() const;

    int getHP() const;
    int getMaxHP() const;
    int getAttack() const;
    std::string getName() const;

    void takeDamage(int damage);
    void heal(int amount);

    void setHP(int value);
    void setMaxHP(int value);
    void setAttack(int value);

    void increaseMaxHP(int amount);
    void increaseAttack(int amount);
};