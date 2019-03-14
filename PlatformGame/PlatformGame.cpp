#include "PlatformGame.h"
#include <experimental/filesystem>
#include <iostream>
#include "Game.h"
namespace fs = std::experimental::filesystem;
using namespace std;

Game *game;
PlatformGame::PlatformGame(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::PlatformGameClass)
{
	ui->setupUi(this);
	connect(ui->editorButton, SIGNAL(clicked()), this, SLOT(openEditor()));
	connect(ui->refreshButton, SIGNAL(clicked()), this, SLOT(refreshLevels()));
	connect(ui->playButton, SIGNAL(clicked()), this, SLOT(playGame()));
	ShowLevels();
}

void PlatformGame::ShowLevels()
{
	levels.clear();
	ui->comboBox->clear();
	for (auto& p : fs::directory_iterator("./levels/"))
	{
		std::string text = p.path().string().c_str();

		std::replace(text.begin(), text.end(),'\\', '/');

		levels.push_back(text);
		text.erase(0, 9);
		QString label = text.c_str();
		ui->comboBox->addItem(label);
	}
}

void PlatformGame::refreshLevels()
{
	ShowLevels();
}

void PlatformGame::playGame()
{
	playGame(levels[ui->comboBox->currentIndex()]);
}



void PlatformGame::playGame(std::string levelpath)
{
	QMainWindow::hide();
	int fps = 60;
	int frameDelay = 1000 / fps;
	Uint32 frameStart;
	int frameTime;
	game = new Game();
	
	game->init(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 960, false, levelpath);


	while (game->running())
	{
		if (game->ev.type == SDL_KEYDOWN)
		{
			if (game->ev.key.keysym.sym == SDLK_ESCAPE) {
			if(!game->won && !game->died)
				if (game->pause)
					game->pause = false;
				else game->pause = true;
				game->ev.type = SDL_KEYUP;
			}
			if (game->ev.key.keysym.sym == SDLK_s && (game->pause || game->won || game->died)) {
				game->pauseGui = (game->pauseGui + 1) % 2;
				game->ev.type = SDL_KEYUP;
			}
			if (game->ev.key.keysym.sym == SDLK_w && (game->pause || game->won || game->died)) {
				game->pauseGui = (game->pauseGui - 1) % 2;
				if (game->pauseGui < 0) game->pauseGui = 1;
				game->ev.type = SDL_KEYUP;
			}

			if (game->ev.key.keysym.sym == SDLK_SPACE && (game->pause || game->won || game->died)) {
				switch (game->pauseGui)
				{
				case 0:
					if (!game->died && !game->won)
					{
						game->pause = false;
						game->pauseGui = 0;
					}
					break;

				case 1:
					game->isRunning = false;
					game->pauseGui = 0;
					break;


				default:
					break;
				}
			}


		}

		frameStart = SDL_GetTicks();
		game->handleEvents();

		if (game->pause)
		{
			game->pauseRender("Pause");
		}
		else if (game->died)
		{
			game->pauseRender("You Died!");
		}
		else if (game->won)
		{
			game->pauseRender("You won!");
		}
		else {
			game->update();
			game->render();
		}

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime) {
			SDL_Delay(frameDelay - frameTime);
		}
	}
	game->pause = false;
	game->clean();
	delete game;
	QMainWindow::show();
	QApplication::closeAllWindows();
}

void PlatformGame::openEditor()
{
	system(".\\mapEditor.exe");
}

void PlatformGame::closeEvent(QCloseEvent *event)
{

	game->exit();
}