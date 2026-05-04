# 🎮 RPG Battle Simulator

> A Console-Based RPG Game built with Modern C++ (C++14/17) applying Object-Oriented Programming principles.

![C++](https://img.shields.io/badge/C%2B%2B-14/17-blue?style=for-the-badge)
![OOP](https://img.shields.io/badge/OOP-Encapsulation%20%7C%20Inheritance%20%7C%20Polymorphism-green?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

---

## ✨ Features

- ✅ **Inheritance & Polymorphism**: `Character` base class with `Warrior` and `Mage` derived classes
- ✅ **Encapsulation & Abstraction**: Private members with controlled access via public methods
- ✅ **RAII & Smart Pointers**: Memory safety using `std::unique_ptr`
- ✅ **File I/O System**: Save/Load game progress using `fstream`
- ✅ **Input Validation**: Robust error handling for user input

---

## 🛠️ How to Run

### Option 1: Terminal
```bash
g++ -std=c++14 -Wall -Wextra main.cpp RPG_Project.cpp -o rpg_game
./rpg_game

### Option 2: Code::Blocks
Open RPG_Project.cbp
Press F9 to Build & Run

 Files
RPG-Game/
├── main.cpp
├── RPG_Project.h
├── RPG_Project.cpp
├── .gitignore
└── README.md

👤 Author
**Khaled Elfahl**

📄 License
MIT License
