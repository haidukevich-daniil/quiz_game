#pragma once
#define QT_NO_DEPRECATED_WARNINGS

#include <QtWidgets/QMainWindow>
#include "ui_cvic8_hra.h"
#include <Question.h>
#include <Player.h>

class cvic8_hra : public QMainWindow
{
    Q_OBJECT

public:
    cvic8_hra(QWidget *parent = nullptr);
    ~cvic8_hra();

private:
    Ui::cvic8_hraClass ui;
    Question questions[10];
    Player player;
    int order[10];
    void loadQuestions();
    bool gameStarted = false;
    int currentQuestion = 0;
    void startGame();
    void showQuestions();
    void confirm();
    void skip();
    void finishGame();
    void useJoker();
};

