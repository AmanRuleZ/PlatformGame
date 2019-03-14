#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QEvent>
#include "ui_PlatformGame.h"

class PlatformGame : public QMainWindow
{
	Q_OBJECT

public:
	PlatformGame(QWidget *parent = Q_NULLPTR);
	void ShowLevels();
	std::vector<std::string> levels;
	void playGame(std::string levelpath);
	void closeEvent(QCloseEvent *event);

private slots:
	void openEditor();
	void refreshLevels();
	void playGame();


private:
	Ui::PlatformGameClass *ui;
};
