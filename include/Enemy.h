#pragma once

#include "Character.h"
#include "Position.h"

class Enemy : public Character
{
private:
    std::string enemyType;

public:
    Position position;

    Enemy(
        const std::string& type,
        int health,
        int attack,
        Position position);

    std::string getRole() const override;
    char getSymbol() const override;
    std::string getDescription() const override;
};