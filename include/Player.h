#pragma once

#include "Character.h"
#include "Position.h"

class Player : public Character
{
private:
    int level;
    int experience;
    int coins;
    int potions;
    bool hasKey;
    int enemiesKilled;

public:
    Position position;

    Player();

    std::string getRole() const override;
    char getSymbol() const override;
    std::string getDescription() const override;

    int getLevel() const;
    int getExperience() const;
    int getCoins() const;
    int getPotions() const;
    bool getKey() const;
    int getEnemiesKilled() const;

    void addCoins(int amount);
    void addPotion();
    void addEnemyKill();
    void obtainKey();

    bool usePotion();
    void addExperience(int amount);

    void setLevel(int value);
    void setExperience(int value);
    void setCoins(int value);
    void setPotions(int value);
    void setKey(bool value);
    void setEnemiesKilled(int value);
    void setPosition(Position value);
};