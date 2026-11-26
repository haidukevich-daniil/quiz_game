#pragma once

#include <QString>
class Question
{
public:
	QString text;
	QString answers[4];
	int correct = 0;
};

