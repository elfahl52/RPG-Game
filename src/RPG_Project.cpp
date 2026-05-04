#include "RPG_Project.h"

// ========================
// Item Implementation
// ========================
Item::Item(string n, int v) : name(move(n)), value(v) {}
string Item::getName() const { return name; }
int Item::getValue() const { return value; }

Potion::Potion(string n, int v, int h) : Item(move(n), v), healAmount(h) {}
void Potion::use(Character& target) {
    cout << "🧪 " << target.getName() << " drank " << name << ".\n";
    target.heal(healAmount);
}
string Potion::getInfo() const { return name + " (Heals " + to_string(healAmount) + " HP)"; }

Weapon::Weapon(string n, int v, int b) : Item(move(n), v), bonusAtk(b) {}
void Weapon::use(Character& target) {
    cout << "️ " << target.getName() << " equipped " << name << " (ATK +" << bonusAtk << ")\n";
}
string Weapon::getInfo() const { return name + " (ATK +" + to_string(bonusAtk) + ")"; }

// ========================
// Character Implementation
// ========================
Character::Character(string n, int h, int a, int d)
    : name(move(n)), hp(h), maxHp(h), attackPower(a), defense(d), level(1) {}

string Character::getName() const { return name; }
int Character::getHp() const { return hp; }
int Character::getMaxHp() const { return maxHp; }
int Character::getLevel() const { return level; }
bool Character::isAlive() const { return hp > 0; }

void Character::levelUp() {
    level++;
    maxHp += 10; hp = maxHp;
    attackPower += 3; defense += 2;
    cout << "🎉 " << name << " leveled up to Level " << level << "!\n";
}

void Character::takeDamage(int dmg) {
    int actual = max(1, dmg - defense);
    hp = max(0, hp - actual);
    cout << "💥 " << name << " received " << actual << " damage (HP: " << hp << "/" << maxHp << ")\n";
}

void Character::heal(int amount) {
    int restored = min(amount, maxHp - hp);
    hp += restored;
    cout << "✨ " << name << " restored " << restored << " HP (HP: " << hp << "/" << maxHp << ")\n";
}

void Character::addItem(unique_ptr<Item> item) { inventory.push_back(move(item)); }
void Character::useItem(size_t index) {
    if (index >= inventory.size()) throw out_of_range("Invalid inventory index");
    inventory[index]->use(*this);
}
size_t Character::getInventorySize() const { return inventory.size(); }
Item* Character::getItem(size_t index) const { return index < inventory.size() ? inventory[index].get() : nullptr; }

// Warrior
Warrior::Warrior(string n) : Character(move(n), 100, 15, 5) {}
void Warrior::attack(Character& target) {
    int dmg = attackPower + (rand() % 6);
    cout << "⚔️ " << name << " uses Strike! (" << dmg << " damage)\n";
    target.takeDamage(dmg);
}
void Warrior::displayStats() const {
    cout << "🛡️ Warrior | HP: " << hp << "/" << maxHp << " | ATK: " << attackPower << " | DEF: " << defense << "\n";
}
string Warrior::getSaveString() const { return "Warrior," + name + "," + to_string(hp) + "," + to_string(level); }
void Warrior::loadFromString(const string& data) {
    level = stoi(data);
    maxHp = 100 + (level - 1) * 10; hp = maxHp;
    attackPower = 15 + (level - 1) * 3; defense = 5 + (level - 1) * 2;
}

// Mage
Mage::Mage(string n) : Character(move(n), 70, 20, 2) {}
void Mage::attack(Character& target) {
    int dmg = attackPower + (rand() % 10);
    cout << "🔮 " << name << " casts Fireball! (" << dmg << " damage)\n";
    target.takeDamage(dmg);
}
void Mage::displayStats() const {
    cout << " Mage | HP: " << hp << "/" << maxHp << " | ATK: " << attackPower << " | DEF: " << defense << "\n";
}
string Mage::getSaveString() const { return "Mage," + name + "," + to_string(hp) + "," + to_string(level); }
void Mage::loadFromString(const string& data) {
    level = stoi(data);
    maxHp = 70 + (level - 1) * 10; hp = maxHp;
    attackPower = 20 + (level - 1) * 3; defense = 2 + (level - 1) * 2;
}

// ========================
// Battle Implementation
// ========================
Battle::Battle(Character& p, Character& e) : player(p), enemy(e) {}

void Battle::fight() {
    cout << "\n⚔️ Battle started between " << player.getName() << " and " << enemy.getName() << "\n";
    while (player.isAlive() && enemy.isAlive()) {
        cout << "\n--- Your Turn ---\n";
        player.displayStats();
        enemy.displayStats();
        cout << "1. Attack\n2. Use Item\n3. Flee\nChoose: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "❌ Invalid input. Try again.\n";
            continue;
        }
        cin.ignore();

        if (choice == 1) {
            player.attack(enemy);
        } else if (choice == 2) {
            if (player.getInventorySize() == 0) {
                cout << " Inventory is empty.\n";
                continue;
            }
            cout << "📦 Select item index (0-" << player.getInventorySize() - 1 << "): ";
            size_t idx; cin >> idx; cin.ignore();
            try { player.useItem(idx); }
            catch (const exception& e) { cout << "⚠️ " << e.what() << "\n"; }
        } else {
            cout << "🏃 You fled! Battle ended.\n";
            return;
        }

        if (!enemy.isAlive()) {
            cout << "🏆 Victory! Gained XP.\n";
            player.levelUp();
            return;
        }

        cout << "\n--- Enemy's Turn ---\n";
        enemy.attack(player);
        if (!player.isAlive()) cout << "💀 Defeat! You have fallen.\n";
    }
}

// ========================
// Game Implementation
// ========================
void Game::start() {
    cout << "🎮 Welcome to RPG Battle Simulator\n";
    cout << "Choose your hero:\n1. Warrior\n2. Mage\nChoose: ";
    int c; cin >> c; cin.ignore();

    if (c == 1) player = make_unique<Warrior>("Arthas");
    else player = make_unique<Mage>("Gandalf");

    player->addItem(make_unique<Potion>("Health Potion", 10, 30));
    player->addItem(make_unique<Weapon>("Iron Sword", 20, 5));

    string filename = "save.dat";
    ifstream f(filename);
    if (f.is_open()) {
        cout << " Save file found. Load progress? (y/n): ";
        char ch; cin >> ch; cin.ignore();
        if (ch == 'y' || ch == 'Y') loadGame(filename);
        f.close();
    }
    menu();
}

void Game::menu() {
    while (true) {
        cout << "\n📋 Main Menu:\n1. View Stats\n2. Random Battle\n3. Save Progress\n4. Exit\nChoose: ";
        int ch;
        if (!(cin >> ch)) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "❌ Invalid input.\n";
            continue;
        }
        cin.ignore();

        try {
            switch (ch) {
                case 1: player->displayStats();
                        cout << " Inventory (" << player->getInventorySize() << " items):\n";
                        for (size_t i = 0; i < player->getInventorySize(); ++i)
                            cout << "  [" << i << "] " << player->getItem(i)->getInfo() << "\n";
                        break;
                case 2: {
                    if (!player->isAlive()) {
                        cout << "💀 You need to rest first. Reviving...\n";
                        player = make_unique<Warrior>("Revived Hero");
                    }
                    unique_ptr<Character> enemy;
                    if (rand() % 2) enemy = make_unique<Warrior>("Goblin");
                    else enemy = make_unique<Mage>("Dark Sorcerer");
                    Battle b(*player, *enemy);
                    b.fight();
                    break;
                }
                case 3: saveGame("save.dat"); break;
                case 4: cout << "👋 Goodbye!\n"; return;
                default: cout << "❌ Invalid choice\n";
            }
        } catch (const exception& e) {
            cerr << "⚠️ Error: " << e.what() << "\n";
        }
    }
}

void Game::saveGame(const string& filename) {
    ofstream out(filename);
    out << player->getSaveString() << "\n";
    cout << "💾 Game saved successfully.\n";
}

void Game::loadGame(const string& filename) {
    ifstream in(filename);
    string line;
    if (getline(in, line)) {
        size_t comma = line.find(',');
        string type = line.substr(0, comma);
        string rest = line.substr(comma + 1);
        size_t lastComma = rest.rfind(',');
        string lvlStr = rest.substr(lastComma + 1);

        if (type == "Warrior") player = make_unique<Warrior>("Arthas");
        else player = make_unique<Mage>("Gandalf");

        player->loadFromString(lvlStr);
        cout << "📥 Progress loaded successfully.\n";
    }
}
