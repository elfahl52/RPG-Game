#ifndef RPG_PROJECT_H
#define RPG_PROJECT_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <algorithm>

using namespace std;

class Character; // Forward declaration

// ========================
// Item Classes
// ========================
class Item {
protected:
    string name;
    int value;
public:
    Item(string n, int v);
    virtual ~Item() = default;
    string getName() const;
    int getValue() const;
    virtual void use(Character& target) = 0;
    virtual string getInfo() const = 0;
};

class Potion : public Item {
    int healAmount;
public:
    Potion(string n, int v, int h);
    void use(Character& target) override;
    string getInfo() const override;
};

class Weapon : public Item {
    int bonusAtk;
public:
    Weapon(string n, int v, int b);
    void use(Character& target) override;
    string getInfo() const override;
};

// ========================
// Character Classes
// ========================
class Character {
protected:
    string name;
    int hp, maxHp;
    int attackPower, defense;
    int level;
    vector<unique_ptr<Item>> inventory;
public:
    Character(string n, int h, int a, int d);
    virtual ~Character() = default;

    string getName() const;
    int getHp() const;
    int getMaxHp() const;
    int getLevel() const;
    bool isAlive() const;

    virtual void attack(Character& target) = 0;
    virtual void levelUp();
    void takeDamage(int dmg);
    void heal(int amount);
    void addItem(unique_ptr<Item> item);
    void useItem(size_t index);
    size_t getInventorySize() const;
    Item* getItem(size_t index) const;

    virtual void displayStats() const = 0;
    virtual string getSaveString() const = 0;
    virtual void loadFromString(const string& data) = 0;
};

class Warrior : public Character {
public:
    Warrior(string n);
    void attack(Character& target) override;
    void displayStats() const override;
    string getSaveString() const override;
    void loadFromString(const string& data) override;
};

class Mage : public Character {
public:
    Mage(string n);
    void attack(Character& target) override;
    void displayStats() const override;
    string getSaveString() const override;
    void loadFromString(const string& data) override;
};

// ========================
// Battle Class
// ========================
class Battle {
    Character& player;
    Character& enemy;
public:
    Battle(Character& p, Character& e);
    void fight();
};

// ========================
// Game Class
// ========================
class Game {
    unique_ptr<Character> player;
public:
    void start();
    void menu();
    void saveGame(const string& filename);
    void loadGame(const string& filename);
};

#endif // RPG_PROJECT_H
