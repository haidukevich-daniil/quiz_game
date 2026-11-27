#include "cvic8_hra.h"
#include <fstream>
#include <vector>
#include <QMessageBox>

#include <algorithm>
#include <random>

cvic8_hra::cvic8_hra(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    ui.levelComboBox->addItem("lahka");
    ui.levelComboBox->addItem("stredna");
    ui.levelComboBox->addItem("tazka");

    loadQuestions();
    connect(ui.newPushButton, &QPushButton::clicked, this, &cvic8_hra::startGame);
    connect(ui.continuePushButton, &QPushButton::clicked, this, &cvic8_hra::confirm);
    connect(ui.skipPushButton, &QPushButton::clicked, this, &cvic8_hra::skip);
    connect(ui.endPushButton, &QPushButton::clicked, this, &cvic8_hra::finishGame);
    connect(ui.pushButton1, &QPushButton::clicked, this, &cvic8_hra::useJoker);
    connect(ui.pushButton2, &QPushButton::clicked, this, &cvic8_hra::useJoker);
    connect(ui.pushButton3, &QPushButton::clicked, this, &cvic8_hra::useJoker);
}

cvic8_hra::~cvic8_hra()
{}

void cvic8_hra::loadQuestions() {
    std::ifstream file("Vzorove_otazky.csv");
    if (!file.is_open()) {
        qDebug() << "Cannot open file";
        return;
    }

    std::string line;
    int i = 0;

    while (i != 10) {
        getline(file, line, ',');
        questions[i].text = QString::fromStdString(line);

        getline(file, line, ',');
        questions[i].correct = line[0] - 'a';

        for (int j = 0; j < 3; j++) {
            getline(file, line, ',');
            questions[i].answers[j] = QString::fromStdString(line);
        }
        getline(file, line);
        questions[i].answers[3] = QString::fromStdString(line);

        i++;
    }

    qDebug() << "Loaded successfuly";
}

void cvic8_hra::startGame() {
    if (ui.lineEdit->text().isEmpty()) return;
    if (currentQuestion == 10) finishGame();

    gameStarted = true;
    currentQuestion = 0;
    player.score = 0;
    
    int dif = ui.levelComboBox->currentIndex();
    if (dif == 0) player.jokers = 3;
    else if (dif == 1) player.jokers = 2;
    else player.jokers = 1;

    ui.pushButton1->setVisible(player.jokers >= 1);
    ui.pushButton2->setVisible(player.jokers >= 2);
    ui.pushButton3->setVisible(player.jokers >= 3);

    ui.pushButton1->setEnabled(true);
    ui.pushButton2->setEnabled(true);
    ui.pushButton3->setEnabled(true);

    ui.nameLable->setEnabled(false);
    ui.randomCheckBox->setEnabled(false);
    ui.newPushButton->setEnabled(false);
    ui.levelComboBox->setEnabled(false);
    ui.continuePushButton->setEnabled(true);
    ui.skipPushButton->setEnabled(true);

    for (int i = 0; i < 10; i++)
        order[i] = i;

    if (ui.randomCheckBox->isChecked()) {
        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(std::begin(order), std::end(order), g);
    }

    qDebug() << "Player:" << ui.lineEdit->text();
    qDebug() << "Difficulty:" << dif;
    qDebug() << "Jokers:" << player.jokers;

    showQuestions();
}

void cvic8_hra::showQuestions() {
    QRadioButton* radios[4] = { ui.aRadioButton, ui.bRadioButton, ui.cRadioButton, ui.dRradioButton };
    for (QRadioButton* r : radios) {
        r->setAutoExclusive(false);
        r->setChecked(false);
        r->setAutoExclusive(true);
        r->setEnabled(true);
    }

    if (!gameStarted)
        return;

    if (currentQuestion >= 10) {
        finishGame();
        return;
    }

    ui.questionTextEdit->setText(questions[order[currentQuestion]].text);

    ui.doubleSpinBox->setValue(player.score);

    ui.aRadioButton->setText(questions[order[currentQuestion]].answers[0]);
    ui.bRadioButton->setText(questions[order[currentQuestion]].answers[1]);
    ui.cRadioButton->setText(questions[order[currentQuestion]].answers[2]);
    ui.dRradioButton->setText(questions[order[currentQuestion]].answers[3]);
}

void cvic8_hra::confirm(){


    int answer = -1;
   
    if (ui.aRadioButton->isChecked()) answer = 0;
    else if (ui.bRadioButton->isChecked()) answer = 1;
    else if (ui.cRadioButton->isChecked()) answer = 2;
    else if (ui.dRradioButton->isChecked()) answer = 3;

    if (answer == -1)
        return;

    if (answer == questions[order[currentQuestion]].correct) {
        player.score += 1;
        qDebug() << "Correct!";
    }
    else {
        player.score -= 1;
        qDebug() << "Incorrect!";
    }

    currentQuestion++;
    showQuestions();
}

void cvic8_hra::skip() {
    if (currentQuestion >= 10) {
        finishGame();
        return;
    }

    player.score -= 0.5;
    currentQuestion++;
    showQuestions();
}

void cvic8_hra::finishGame()
{
    gameStarted = false;

    qDebug() << "Final score:" << player.score;

    ui.nameLable->setEnabled(true);
    ui.randomCheckBox->setEnabled(true);
    ui.newPushButton->setEnabled(true);
    ui.levelComboBox->setEnabled(true);

    ui.continuePushButton->setEnabled(false);
    ui.skipPushButton->setEnabled(false);

    ui.doubleSpinBox->setValue(0);
    ui.questionTextEdit->clear();

    for (auto r : { ui.aRadioButton, ui.bRadioButton, ui.cRadioButton, ui.dRradioButton }) {
        r->setAutoExclusive(false);
        r->setChecked(false);
        r->setAutoExclusive(true);
        r->setText("");
        r->setEnabled(false);
    }

    QMessageBox::information(this, "End of the game",
        "Player: " + ui.lineEdit->text() + "\n"
        "Score: " + QString::number(player.score, 'f', 1));
}

void cvic8_hra::useJoker()
{
    if (!gameStarted || currentQuestion >= 10)
        return;

    QRadioButton* radios[4] = { ui.aRadioButton, ui.bRadioButton, ui.cRadioButton, ui.dRradioButton };
    int correct = questions[order[currentQuestion]].correct;

    std::vector<int> wrong;
    for (int i = 0; i < 4; i++)
        if (i != correct)
            wrong.push_back(i);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(wrong.begin(), wrong.end(), g);

    for (int k = 0; k < 2; k++) {
        QRadioButton* r = radios[wrong[k]];
        r->setAutoExclusive(false);
        r->setChecked(false);
        r->setAutoExclusive(true);
        r->setEnabled(false);
    }

    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (btn)
        btn->setEnabled(false);
}