================================================================
  MONSTER TAMER v2.0 - OOP Lab Project
  Subject  : Object-Oriented Programming (OOP)
  Language : C++
================================================================

----------------------------------------------------------------
  FILES IN THIS REPOSITORY
----------------------------------------------------------------

1. MonsterTamerGame.cpp
   - The main (and only) source file for the entire project.
   - Contains all classes, game logic, and the entry point (main).
   - Compile and run this single file to play the game.

2. readme.txt
   - This file. Explains the purpose of all submitted files
     and how the OOP concepts are applied in the project.

----------------------------------------------------------------
  PURPOSE OF THE PROJECT
----------------------------------------------------------------

This project implements a turn-based Monster Tamer RPG game
in C++ to demonstrate core Object-Oriented Programming concepts
including inheritance, polymorphism, abstract classes, operator
overloading, templates, and file I/O.

The Game base class was provided by the instructor. The student
derived MonsterTamerGame from it and implemented the full game
inside the overridden play() method.

----------------------------------------------------------------
  CLASS STRUCTURE & OOP CONCEPTS USED
----------------------------------------------------------------

[1] Game  (Abstract Base Class - provided by instructor)
    - Has a private string 'title' and a pure virtual play().
    - Acts as the interface that all games must follow.
    - Demonstrates: Abstract class, pure virtual function.

[2] MonsterTamerGame  (Derived from Game)
    - Inherits Game publicly.
    - Passes "Monster Tamer v2.0" to the base constructor.
    - Overrides play() with the full game loop.
    - main() holds a Game* pointer to MonsterTamerGame,
      demonstrating runtime polymorphism.
    - Demonstrates: Inheritance, method overriding,
                    polymorphism, constructor chaining.

[3] Monster  (Abstract Base Class)
    - Protected data: name, type, hp, maxHp, atk, def, lvl, exp.
    - Pure virtual methods: special(), spDmg().
    - Concrete methods: attack(), hit(), heal(), gainExp().
    - Overloads operator<< for formatted console output.
    - Demonstrates: Abstraction, encapsulation,
                    operator overloading.

[4] FireMon, WaterMon, GrassMon  (Derived from Monster)
    - Each provides its own special() name and spDmg() value.
    - FireMon also overrides attack() with higher variance.
    - Demonstrates: Concrete derived classes, method overriding.

[5] Player
    - Manages a team of up to 4 monsters (vector of unique_ptr).
    - Handles money, win count, save/load (file I/O).
    - Demonstrates: Composition, smart pointers, file streams.

[6] typeBonus<T>  (Function Template)
    - Calculates type-matchup multiplier (Fire/Water/Grass).
    - Demonstrates: Templates.

----------------------------------------------------------------
  HOW TO COMPILE & RUN
----------------------------------------------------------------

Requirements:
  - A C++17 compatible compiler (g++, MSVC, clang++)

Compile (Linux / macOS / Windows with g++):
  g++ -std=c++17 -o MonsterTamer MonsterTamerGame.cpp

Run:
  ./MonsterTamer          (Linux / macOS)
  MonsterTamer.exe        (Windows)

Note: ANSI colour codes are used for coloured headings.
  - Works on: Linux terminal, macOS Terminal, Windows Terminal.
  - May not display colours in: old Windows Command Prompt (cmd).

----------------------------------------------------------------
  GAMEPLAY OVERVIEW
----------------------------------------------------------------

- On first launch, choose "New Game", enter your name, and
  pick one of three starter monsters:
    * Flamox   (Fire)  - high attack
    * Aquatail (Water) - high HP
    * Leafang  (Grass) - balanced stats

- From the World Map menu you can:
    [1] Explore  - pick a location and battle a wild monster
    [2] My Team  - view your current team and their stats
    [3] Heal     - fully restore all monsters' HP
    [4] Save     - save progress to save.txt
    [5] Quit     - exit the game

- In battle you can:
    [1] Attack         - normal attack with slight randomness
    [2] Special move   - high damage move (usable once per battle)
    [3] Catch          - attempt to capture the wild monster
    [4] Run            - flee from battle

- Type matchups (Fire > Grass > Water > Fire) deal 2x / 0.5x damage.
- Defeating or catching monsters earns EXP; enough EXP = Level Up.
- Your team can hold up to 4 monsters.


================================================================
