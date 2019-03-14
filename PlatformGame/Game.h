#ifndef Game_h
#define Game_h

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>

class AssetManager;

class ColliderComponent;

using namespace std;

class Game
{
public:
	Game();
	~Game();
	void init(int xpos, int ypos, int width, int height, bool fullscreen, std::string levelpath);
	void render();
	void pauseRender(std::string text);
	void exit();

	void update();
	void clean();
	void handleEvents();
	bool running();

	static bool haveGun;
	static bool destroyed;

	static int time;
	SDL_Color white = { 255,255,255,255 };
	SDL_Color black = { 0,0,0,255 };
	SDL_Color grey = { 105,105,105,255 };
	SDL_Color lightGrey = { 140,140,140,255 };
	static SDL_Renderer *renderer;
	static SDL_Event ev;

	bool isRunning=true;
	bool won;
	bool died;
	bool pause;



	static int pauseGui;
	
	static SDL_Rect camera;
	static SDL_Rect mapSize;

	static AssetManager assets;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupCharacters,
		groupColliders,
		groupProjectiles,
		groupPickups,
		groupPck,
		groupTraps,
		groupUI,
		groupEnemies
	};

private:
	
	SDL_Window *window;
	
};

#endif