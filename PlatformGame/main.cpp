#include "PlatformGame.h"
#include "SDL.h"
#include <QtWidgets/QApplication>

int SDL_main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	PlatformGame w;
	w.show();


	return a.exec();
}