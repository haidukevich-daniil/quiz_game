# Qt Quiz Game

A quiz game in C++ and Qt with 10 questions, each with four possible answers and exactly one correct. Made as a university assignment (STU Bratislava).

## Features

- Player enters a name and picks a difficulty (easy, medium, hard)
- Optional random order of questions
- Scoring: +1 for a correct answer, -1 for a wrong one, -0.5 for a skipped question
- 50:50 jokers: 3 on easy, 2 on medium, 1 on hard. Each joker can be used once per game and leaves the correct answer plus one wrong answer
- Final score shown in a message box when the game ends
- Questions loaded from a CSV file

## Project structure

| File | Purpose |
|---|---|
| `cvic8_hra.h / .cpp` | Main window: game logic and slots |
| `cvic8_hra.ui` | Interface made in Qt Designer |
| `question.h / .cpp` | `Question` class (text, 4 answers, correct index) |
| `player.h / .cpp` | `Player` class (name, score, jokers) |
| `Vzorove_otazky.csv` | The 10 questions |
| `main.cpp` | Application entry point |

## Questions file format

One question per line, no header:

```
question,correct letter (a-d),answer A,answer B,answer C,answer D
```

Example:

```
Ktoré mesto je hlavným mestom Slovenska?,b,Košice,Bratislava,Žilina,Trnava
```

The file must be in the working directory of the program (next to the `.exe`).

## Build and run

Requirements: Qt [6.x] and [Visual Studio 2022 with the Qt VS Tools extension / Qt Creator].

1. Open the project in your IDE.
2. Build the project.
3. Copy `Vzorove_otazky.csv` into the folder of the built `.exe`.
4. Run.

## How to play

1. Enter your name and choose a difficulty.
2. Optionally tick "Random" for random order.
3. Press "New game".
4. Select an answer and press "Confirm", or press "Skip" to skip.
5. Use a 50:50 joker if you need it.
6. Press "End" to quit early.
