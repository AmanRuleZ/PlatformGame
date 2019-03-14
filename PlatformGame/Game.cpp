#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "HPSystem.h"
#include "AssetManager.h"
#include <fstream>



Map* mapa;
Manager manager;
int godMode = 0;
SDL_Renderer* Game::renderer = nullptr;

SDL_Event Game::ev;
int Game::time;

SDL_Rect Game::camera;
SDL_Rect Game::mapSize;

AssetManager Game::assets(&manager);


bool Game::haveGun = false;
bool Game::destroyed = true;


int Game::pauseGui = 0;

Entity& background(manager.addEntity());

Entity& portal(manager.addEntity());

Entity& player(manager.addEntity());
Entity& label(manager.addEntity());
Entity& playerHead(manager.addEntity());

Entity& UIBar(manager.addEntity());
Entity& rockIcon(manager.addEntity());
Entity& pistolIcon(manager.addEntity());
Entity& AKIcon(manager.addEntity());
Entity& actualGun(manager.addEntity());

Entity& rockCap(manager.addEntity());
Entity& pistolAmmo(manager.addEntity());
Entity& AKAmmo(manager.addEntity());


Entity& resumeTitle(manager.addEntity());
Entity& exitButton(manager.addEntity());
Entity& resumeButton(manager.addEntity());



Game::Game()
{
}


Game::~Game()
{
}

void Game::init(int xpos, int ypos, int width, int height, bool fullscreen, std::string levelpath)
{
	isRunning = false;
	pause = false;
	died = false;
	won = false;
	time = 0;
	bool isFullScreen = NULL;
	if (fullscreen)
		isFullScreen = SDL_WINDOW_FULLSCREEN;


	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_AllocateChannels(64);


	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		cout << "SDL Init succesfull" << endl;

		window = SDL_CreateWindow("Platform game", xpos, ypos, width, height, isFullScreen);
		if (window) {
			cout << "Window Created Succesfull" << endl;
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			cout << "Renderer Created Succesfull" << endl;
		}
		isRunning = true;
	}
	else {
		isRunning = false;
	}

	Game::haveGun = false;


	if (TTF_Init() == -1)
	{
		cout << "Error: SDL_TTF" << endl;
	}

	assets.AddTexture("background", "maps/staticBackground.png");

	assets.AddTexture("portal", "assets/portal.png");
	assets.AddTexture("terrain", "maps/mapEditor.png");
	assets.AddTexture("player", "assets/playeranims.png");
	assets.AddTexture("enemy", "assets/enemyanims.png");
	assets.AddTexture("playerHead", "assets/head.png");
	assets.AddTexture("projectile", "assets/proj.png");
	assets.AddTexture("AK", "assets/AK.png");
	assets.AddTexture("AK2", "assets/AK2.png");
	assets.AddTexture("blank", "assets/blank.png");
	assets.AddTexture("pistol", "assets/pistol.png");
	assets.AddTexture("pistol2", "assets/pistol2.png");
	assets.AddTexture("rock", "assets/rock.png");
	assets.AddTexture("rock2", "assets/rock2.png");
	assets.AddTexture("heart", "assets/heart.png");
	assets.AddTexture("shield", "assets/shield.png");
	assets.AddTexture("spikes", "assets/spikes.png");
	assets.AddTexture("shooter", "assets/shooter.png");
	assets.AddTexture("dog", "assets/dog_anim.png");
	assets.AddTexture("UIBar", "assets/UIBar.png");
	assets.AddTexture("Frame", "assets/ColTex.png");
	assets.AddTexture("rockblack","assets/rock2black.png");
	assets.AddTexture("pistolblack", "assets/pistol2black.png");
	assets.AddTexture("AKblack", "assets/AK2black.png");
	assets.AddSFX("reload", "sounds/reload.wav");
	assets.AddSFX("jump", "sounds/jump.wav");
	assets.AddSFX("shoot", "sounds/shoot.wav");
	assets.AddSFX("landing", "sounds/landing.wav");
	assets.AddSFX("walk", "sounds/walk.wav");
	assets.AddSFX("throw", "sounds/throw.wav");
	assets.AddSFX("spikes", "sounds/spikes.wav");
	assets.AddSFX("shooter", "sounds/shooter.wav");
	assets.AddSFX("dog", "sounds/dog.wav");
	assets.AddFont("HPFont", "assets/font.ttf", 20);
	assets.AddFont("AmmoFont", "assets/font.ttf", 12);
	assets.AddFont("PauseFont", "assets/font.ttf", 40);
	assets.AddFont("TitleFont", "assets/font2.ttf", 110);

	////////////////////Wczytywanie mapy/////////////////////
	char c;
	std::string c2;
	std::fstream mapInfo;
	mapInfo.open(levelpath + "/map.info" );
	int length=0;
	while (mapInfo.get(c)) {
		length++;
	}
	mapInfo.close();
	mapInfo.open(levelpath + "/map.info");
	mapInfo.get(c);
	c2 = c;
	mapInfo.get(c);
	c2 += c;
	int X = atoi(c2.c_str());
	mapInfo.ignore();
	mapInfo.get(c);
	c2 = c;
	mapInfo.get(c);
	c2 += c;
	int Y = atoi(c2.c_str());

	mapa = new Map("terrain", SCALE, BLOCK_HEIGHT);
	mapa->LoadMap(levelpath + "/map", X, Y);
	Game::camera = { 0,0,1600,992};
	Game::mapSize = { 0,0,X*BLOCK_HEIGHT*SCALE,Y*BLOCK_WIDTH*SCALE};

	mapInfo.ignore();
	mapInfo.get(c);
	c2 = c;
	int pom = -1;
	mapInfo.get(c);
	if (c != ' ')
	{
		c2 += c;
		mapInfo.ignore();
		pom = 0;
	}
	X = atoi(c2.c_str());

	mapInfo.get(c);
	c2 = c;
	if (length == 11+pom) {
		mapInfo.get(c);
		c2 += c;
	}
	Y = atoi(c2.c_str());

	portal.addComponent<TransformComponent>(X*BLOCK_HEIGHT*SCALE, Y*BLOCK_WIDTH*SCALE, BLOCK_HEIGHT, BLOCK_WIDTH, SCALE);
	portal.addComponent<SpriteComponent>("portal", true, 0);
	portal.addComponent<PickupComponent>(6);
	portal.addComponent<ColliderComponent>("pickup", X*BLOCK_HEIGHT*SCALE, Y*BLOCK_WIDTH*SCALE, 1);
	portal.addGroup(groupPickups);
	
	mapInfo.close();
	///////////////////////////////////////////////////////////

	////////////////Tworzenie postaci gracza////////////////////
	std::fstream playerInfo;
	playerInfo.open(levelpath + "/player.info");
	playerInfo.get(c);
	c2 = c;
	playerInfo.get(c);
	if (c != ' ')
	{
		c2 += c;
		playerInfo.ignore();
	}
	X = atoi(c2.c_str());

	playerInfo.get(c);
	c2 = c;
	playerInfo.get(c);
	if (c != ' ')
	{
		c2 += c;
		playerInfo.ignore();
	}
	Y = atoi(c2.c_str());
	playerInfo.get(c);
	int hp = atoi(&c);
	playerInfo.ignore();
	playerInfo.get(c);
	int mhp = atoi(&c);
	playerInfo.ignore();
	playerInfo.get(c);
	int armor = atoi(&c);
	playerInfo.ignore();
	playerInfo.get(c);
	int marmor = atoi(&c);
	player.addComponent<TransformComponent>(X*BLOCK_HEIGHT*SCALE, Y*BLOCK_WIDTH*SCALE, 54, 32, 0.87);
	player.addComponent<SpriteComponent>("player", true, 0);
	player.addComponent<MouseController>(false);
	player.addComponent<GunComponent>("blank", 64, 64, 1);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addComponent<HPComponent>(hp * 100, mhp * 100, armor * 100, marmor * 100);
	player.addGroup(groupCharacters);
	//////////////////////////////////////////////////////////////////
	
	background.addComponent<TransformComponent>(0.0f, 0.0f, 480, 800, 2);
	background.addComponent<SpriteComponent>("background", false, 0);

	resumeTitle.addComponent<UILabel>(300, 100, "ERROR", "TitleFont", black);

	label.addComponent<UILabel>(10, 920, "ERROR", "HPFont", white);
	label.addGroup(groupUI);
	rockCap.addComponent<UILabel>(708, 945, "ERROR", "AmmoFont", white);
	rockCap.addGroup(groupUI);
	pistolAmmo.addComponent<UILabel>(750, 945, "ERROR", "AmmoFont", white);
	pistolAmmo.addGroup(groupUI);
	AKAmmo.addComponent<UILabel>(798, 945, "ERROR", "AmmoFont", white);
	AKAmmo.addGroup(groupUI);

	resumeButton.addComponent<UILabel>(500, 600, "Resume", "PauseFont", black);
	exitButton.addComponent<UILabel>(500, 680, "Exit", "PauseFont", black);

	playerHead.addComponent<TransformComponent>(0.0f, 0.0f, 16, 16, 0.9);
	playerHead.addComponent<SpriteComponent>("playerHead", false, 0);
	playerHead.addComponent<MouseController>(true);

	rockIcon.addComponent<TransformComponent>(704.0f, 908.0f, 32, 32, 0.8);
	rockIcon.addComponent<SpriteComponent>("rock2", false, 0);
	rockIcon.addGroup(groupUI);

	pistolIcon.addComponent<TransformComponent>(752.0f, 904.0f, 32, 32, 1);
	pistolIcon.addComponent<SpriteComponent>("pistol2", false,-40);
	pistolIcon.addGroup(groupUI);

	AKIcon.addComponent<TransformComponent>(798.0f,904.0f, 32, 32, 1);
	AKIcon.addComponent<SpriteComponent>("AK2", false,-40);
	AKIcon.addGroup(groupUI);

	actualGun.addComponent<TransformComponent>(100.0f, 902.0f, 32, 32, 1.1);
	actualGun.addComponent<SpriteComponent>("Frame", false, 0);


	UIBar.addComponent<TransformComponent>(0.0f, 896.0f, 32, 1600, 2);
	UIBar.addComponent<SpriteComponent>("UIBar", false, 0);


}

auto& tiles(manager.getGroup(Game::groupMap));
auto& characters(manager.getGroup(Game::groupCharacters));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& pickups(manager.getGroup(Game::groupPickups));
auto& pck(manager.getGroup(Game::groupPck));

auto& traps(manager.getGroup(Game::groupTraps));
auto& UI(manager.getGroup(Game::groupUI));
auto& enemies(manager.getGroup(Game::groupEnemies));


void Game::handleEvents()
{
	SDL_PollEvent(&ev);
}

void Game::update()
{
	manager.refresh();
	manager.update();

	time++;
	if (time > 1000000000) time = 0;

	for (auto& p : characters)
	{
		p->getComponent<TransformComponent>().anyCollision = false;
		SDL_Rect pCol = p->getComponent<ColliderComponent>().collider;
		p->getComponent<TransformComponent>().onGround = false;

		if ((!p->getComponent<TransformComponent>().canMoveLeft && p->getComponent<TransformComponent>().velocity.x < 0) || (!p->getComponent<TransformComponent>().canMoveRight && p->getComponent<TransformComponent>().velocity.x > 0)) p->getComponent<TransformComponent>().velocity.x = 0;
		for (auto c : colliders) {
			SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;


			if (Collision::ColGround(pCol, cCol)) {
				p->getComponent<TransformComponent>().onGround = true;
				p->getComponent<TransformComponent>().position.y = cCol.y - p->getComponent<ColliderComponent>().collider.h;
			}

			if (Collision::ColLeft(pCol, cCol) && p->getComponent<TransformComponent>().velocity.x < 0) {
				p->getComponent<TransformComponent>().canMoveLeft = false;
				p->getComponent<TransformComponent>().position.x = cCol.x + cCol.w + cCol.w/2 - pCol.w;
				p->getComponent<TransformComponent>().anyCollision = true;
				
				if (p->hasComponent<ColliderComponent>()) {
					if (p->getComponent<ColliderComponent>().tag == "player") cout << "Kolizja z lewej..." << endl;
				}
			}

			if (Collision::ColRight(pCol, cCol) && p->getComponent<TransformComponent>().velocity.x > 0) {
				p->getComponent<TransformComponent>().canMoveRight = false;
				p->getComponent<TransformComponent>().position.x = cCol.x - cCol.w / 2;
				p->getComponent<TransformComponent>().anyCollision = true;
				if (p->hasComponent<ColliderComponent>()) {
					if (p->getComponent<ColliderComponent>().tag == "player") cout << "Kolizja z prawej..." << endl;
				}
			}

			if (Collision::ColUp(pCol, cCol)) {
				p->getComponent<TransformComponent>().velocity.y = 0.03;
				p->getComponent<TransformComponent>().position.y += 5;
			}


		}
		if (!p->getComponent<TransformComponent>().anyCollision) {
			p->getComponent<TransformComponent>().canMoveRight = true;
			p->getComponent<TransformComponent>().canMoveLeft = true;
			//cout << "Nie ma kolizji" << endl;

		}



		if (!p->getComponent<TransformComponent>().onGround) {
			p->getComponent<TransformComponent>().velocity.y += 0.6f;
			if (p->getComponent<TransformComponent>().velocity.y > 5.0f) p->getComponent<TransformComponent>().velocity.y = 5.0f;
		}
		else {
			p->getComponent<TransformComponent>().velocity.y = 0;
			p->getComponent<TransformComponent>().isJumping = false;
			if (!p->getComponent<TransformComponent>().canMove) {
				p->getComponent<TransformComponent>().canMove = true;
				p->getComponent<TransformComponent>().xdir = 0;
			}

		}


	}
	if (godMode > 0) godMode--;
		for (auto& t : traps)
		{
			if (Collision::AABB(t->getComponent<ColliderComponent>().collider, player.getComponent<ColliderComponent>().collider))
			{
				if(godMode==0)
				if (t->getComponent<TrapComponent>().catched(player.getComponent<TransformComponent>(), playerHead.getComponent<TransformComponent>())) {
					HPSystem::HPDown(player.getComponent<HPComponent>(), 30);
					godMode = 30;
				}
			}
			for (auto& p : projectiles) {
				if (Collision::AABB(t->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider)) {
					t->getComponent<TrapComponent>().catched();
				}
			}
		}

		for (auto& p : projectiles) {
			if (Collision::AABB(p->getComponent<ColliderComponent>().collider, player.getComponent<ColliderComponent>().collider)) {
				if (p->getComponent<TrapComponent>().catched(player.getComponent<TransformComponent>(), playerHead.getComponent<TransformComponent>())){
					HPSystem::HPDown(player.getComponent<HPComponent>(), 20);
					p->destroy();
				}
			}
		}
	
	

	for (auto& p : projectiles)
	{

		for (auto& t : tiles) {
			if (t->hasComponent<ColliderComponent>()) {
				if (Collision::AABB(t->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
				{
					p->destroy();
					if (t->hasComponent<HPComponent>()) {
						HPSystem::HPDown(t->getComponent<HPComponent>(), 10);
						if (t->getComponent<HPComponent>().hp <= 0) {
							t->getComponent<ColliderComponent>().collider.x = -100;
							t->getComponent<ColliderComponent>().collider.y = -100;
							t->getComponent<ColliderComponent>().collider.h = 0;
							t->getComponent<ColliderComponent>().collider.w = 0;

						}
					}

				}
			}
		}
	}


	camera.x = player.getComponent<TransformComponent>().position.x - 800.0f;
	camera.y = player.getComponent<TransformComponent>().position.y - 320.0f;

	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.x + camera.w > mapSize.w) camera.x = mapSize.w - camera.w;
	if (camera.y + camera.h > mapSize.h+96) camera.y = mapSize.h - camera.h+96;

	////////////Ustawianie prostok¹ta oznaczaj¹cego aktualn¹ broñ///////////////
	switch (player.getComponent<GunComponent>().actualGun)
	{
	case 0:
		actualGun.getComponent<TransformComponent>().position.x = camera.x + 696;
		player.getComponent<GunComponent>().setTex("rock");
		break;

	case 1:
		actualGun.getComponent<TransformComponent>().position.x = camera.x + 748;
		player.getComponent<GunComponent>().setTex("pistol");
		break;

	case 2:
		actualGun.getComponent<TransformComponent>().position.x = camera.x + 798;
		player.getComponent<GunComponent>().setTex("AK");
		break;

	default:
		actualGun.getComponent<TransformComponent>().position.x = -50;
		player.getComponent<GunComponent>().setTex("blank");
		break;
	}
	////////////////////////////////////////////////////////////////////////


	
	////////////Wyœwietlanie informacji o stanie gracza/////////////////////
	string labelText = "HP: " + to_string(player.getComponent<HPComponent>().hp) + '/' + to_string(player.getComponent<HPComponent>().maxHp) + "   " + "Armor: " + to_string(player.getComponent<HPComponent>().armor) + '/' + to_string(player.getComponent<HPComponent>().maxArmor);

	label.getComponent<UILabel>().SetLabelText(labelText, "HPFont");

	rockCap.getComponent<UILabel>().SetLabelText(to_string(player.getComponent<GunComponent>().gunCount[0]), "AmmoFont");

	string pistolLabel = to_string(player.getComponent<GunComponent>().gunAmmo[1]) + '/' + to_string(player.getComponent<GunComponent>().ammo[1]);
	pistolAmmo.getComponent<UILabel>().SetLabelText(pistolLabel, "AmmoFont");

	string AKLabel = to_string(player.getComponent<GunComponent>().gunAmmo[2]) + '/' + to_string(player.getComponent<GunComponent>().ammo[2]);
	AKAmmo.getComponent<UILabel>().SetLabelText(AKLabel, "AmmoFont");
	//////////////////////////////////////////////////////////////////

	/////////Wyœwietlanie ikon broni w dolnym pasku//////////
	if (player.getComponent<GunComponent>().gunCount[0] == 0)
	{
		rockIcon.getComponent<SpriteComponent>().setTex("rockblack");
	}
	else rockIcon.getComponent<SpriteComponent>().setTex("rock2");

	if (player.getComponent<GunComponent>().gunCount[1] == 0)
	{
		pistolIcon.getComponent<SpriteComponent>().setTex("pistolblack");
	}
	else pistolIcon.getComponent<SpriteComponent>().setTex("pistol2");

	if (player.getComponent<GunComponent>().gunCount[2] == 0)
	{
		AKIcon.getComponent<SpriteComponent>().setTex("AKblack");
	}
	else AKIcon.getComponent<SpriteComponent>().setTex("AK2");
	///////////////////////////////////////////////////////////


	if (player.getComponent<TransformComponent>().velocity.x != 0 && player.getComponent<TransformComponent>().velocity.y == 0 && time % 25 == 0) Game::assets.playSFX("walk", 0, 4);

	for (auto& p : pickups)
	{
		if (Collision::AABB(p->getComponent<ColliderComponent>().collider, player.getComponent<ColliderComponent>().collider))
		{
			cout << "Picked id: " << p->getComponent<PickupComponent>().id << endl;
			if (p->getComponent<PickupComponent>().id == 1) {
				HPSystem::HPUp(player.getComponent<HPComponent>());
			}
			else if (p->getComponent<PickupComponent>().id == 2) {
				HPSystem::ArmUp(player.getComponent<HPComponent>());
			}
			else if (p->getComponent<PickupComponent>().id == 6)
			{
				Game::won = true;
			}
			else {
				player.getComponent<GunComponent>().pickup(p->getComponent<PickupComponent>().id);
			}
			p->destroy();
		}
		if(p->getComponent<PickupComponent>().id != 6)
			if (time % 24 < 12)
				p->getComponent<TransformComponent>().position.y++;
			else p->getComponent<TransformComponent>().position.y--;
	}

	for (auto c : characters)
	{
		if (c->hasComponent<AIComponent>()) {
			for (auto t : tiles) {
				if (Collision::ColRight(c->getComponent<ColliderComponent>().collider, t->getComponent<ColliderComponent>().collider) && c->getComponent<TransformComponent>().onGround) {
					if (c->getComponent<AIComponent>().triggered > 0 && !c->getComponent<TransformComponent>().isJumping)
					{
						c->getComponent<TransformComponent>().isJumping = true;
						c->getComponent<TransformComponent>().velocity.y = -3;
						cout << "skacze" << endl;
					}else	c->getComponent<AIComponent>().swap(1);
				}
				if (Collision::ColLeft(c->getComponent<ColliderComponent>().collider, t->getComponent<ColliderComponent>().collider) && c->getComponent<TransformComponent>().onGround) {
					
					if (c->getComponent<AIComponent>().triggered > 0 && !c->getComponent<TransformComponent>().isJumping)
					{
						c->getComponent<TransformComponent>().isJumping = true;
						c->getComponent<TransformComponent>().velocity.y = -2;
					}
					else	c->getComponent<AIComponent>().swap(0);
				}

			}
		}
	}


	for (auto& e : enemies)
	{
		if (e->getComponent<TransformComponent>().position.x < Game::camera.x + Game::camera.w + 64 &&
			e->getComponent<TransformComponent>().position.x > Game::camera.x - 64 &&
			e->getComponent<TransformComponent>().position.y < Game::camera.y + Game::camera.h + 64 &&
			e->getComponent<TransformComponent>().position.y > Game::camera.y - 64)
		{
			if (e->getComponent<AIComponent>().triggered != 1 && e->hasComponent<GunComponent>()) e->getComponent<GunComponent>().enemyGameTime = Game::time;
			e->getComponent<AIComponent>().goToRock = false;
			if (e->getComponent<AIComponent>().triggered == 0) {
				if (Collision::AABB(player.getComponent<ColliderComponent>().collider, e->getComponent<AIComponent>().FOV)) {
					e->getComponent<AIComponent>().triggered = 1;
				}
				if (e->getComponent<TransformComponent>().xdir == 0 && e->getComponent<AIComponent>().id == "enemy" && e->getComponent<AIComponent>().triggered == 0) {
					e->getComponent<GunComponent>().angle = 0;
					e->getComponent<AIComponent>().swap(rand() % 2);
				}
			}
		
			if (e->getComponent<AIComponent>().triggered == 1 && (player.getComponent<TransformComponent>().position.y > e->getComponent<TransformComponent>().position.y + 192 || player.getComponent<TransformComponent>().position.y < e->getComponent<TransformComponent>().position.y - 192)) {
				e->getComponent<AIComponent>().triggered = 0;
			}

			for (auto& p : projectiles) {
				if (p->getComponent<TransformComponent>().position.x < Game::camera.x + Game::camera.w + 64 &&
					p->getComponent<TransformComponent>().position.x > Game::camera.x - 64 &&
					p->getComponent<TransformComponent>().position.y < Game::camera.y + Game::camera.h + 64 &&
					p->getComponent<TransformComponent>().position.y > Game::camera.y - 64)
				{
					if (Collision::AABB(e->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider)) {
						if (p->getComponent<TrapComponent>().id >= 98) {
							HPSystem::HPDown(e->getComponent<HPComponent>(), 10);
							p->destroy();
						}
					}

					if (p->getComponent<TrapComponent>().id == 100 && (e->getComponent<AIComponent>().triggered == 1 || e->getComponent<AIComponent>().triggered == 2) &&e->getComponent<AIComponent>().id == "dog") {
						e->getComponent<AIComponent>().goToRock = true;
						e->getComponent<AIComponent>().triggered = 2;
						if (e->getComponent<TransformComponent>().position.x > p->getComponent<TransformComponent>().position.x) {
							e->getComponent<TransformComponent>().xdir = -2;
						}
						else if (e->getComponent<TransformComponent>().position.x < p->getComponent<TransformComponent>().position.x) {
							e->getComponent<TransformComponent>().xdir = 2;

						}

					}
				}
			}
			if (!e->getComponent<AIComponent>().goToRock && e->getComponent<AIComponent>().triggered==2) e->getComponent<AIComponent>().triggered = 0;
			if (e->getComponent<HPComponent>().hp <= 0) e->destroy();

			if (e->getComponent<AIComponent>().triggered == 1) {
				if (e->getComponent<AIComponent>().id == "dog") {
					if (e->getComponent<TransformComponent>().position.x > player.getComponent<TransformComponent>().position.x) {
						e->getComponent<TransformComponent>().xdir = -2;
					}
					else if (e->getComponent<TransformComponent>().position.x < player.getComponent<TransformComponent>().position.x) {
						e->getComponent<TransformComponent>().xdir = 2;
					}
				}
				else {
					if (Collision::AABB(e->getComponent<AIComponent>().FOV, player.getComponent<ColliderComponent>().collider)) {
						e->getComponent<TransformComponent>().xdir = 0;
						e->getComponent<GunComponent>().enemyGunRotation(e->getComponent<TransformComponent>().position.x, e->getComponent<TransformComponent>().position.y, player.getComponent<TransformComponent>().position.x, player.getComponent<TransformComponent>().position.y);
					}
					else {
						if (e->getComponent<TransformComponent>().position.x > player.getComponent<TransformComponent>().position.x) {
							e->getComponent<TransformComponent>().xdir = -1.5;
						}
						else if (e->getComponent<TransformComponent>().position.x < player.getComponent<TransformComponent>().position.x) {
							e->getComponent<TransformComponent>().xdir = 1.5;
						}
					}
				}
			}
		}
	}
		




	//Ustalanie pozycji GUI i otoczenia
	background.getComponent<TransformComponent>().position.x = camera.x;
	background.getComponent<TransformComponent>().position.y = camera.y;

	UIBar.getComponent<TransformComponent>().position.x = camera.x;
	UIBar.getComponent<TransformComponent>().position.y = camera.y + 896;


	rockIcon.getComponent<TransformComponent>().position.x = camera.x + 700;
	rockIcon.getComponent<TransformComponent>().position.y = camera.y + 900;

	pistolIcon.getComponent<TransformComponent>().position.x = camera.x + 750;
	pistolIcon.getComponent<TransformComponent>().position.y = camera.y + 900;

	AKIcon.getComponent<TransformComponent>().position.x = camera.x + 800;
	AKIcon.getComponent<TransformComponent>().position.y = camera.y + 900;

	actualGun.getComponent<TransformComponent>().position.y = camera.y + 900;




	player.getComponent<TransformComponent>().relPos.x = player.getComponent<TransformComponent>().position.x - camera.x + 14;
	player.getComponent<TransformComponent>().relPos.y = player.getComponent<TransformComponent>().position.y - camera.y + 18;

	playerHead.getComponent<TransformComponent>().relPos.x = playerHead.getComponent<TransformComponent>().position.x - camera.x + 8;
	playerHead.getComponent<TransformComponent>().relPos.y = playerHead.getComponent<TransformComponent>().position.y - camera.y + 8;

	if(player.getComponent<HPComponent>().hp==0) Game::died=true;

}
void Game::render()
{
	background.draw();

	for (auto& t : traps)
	{
		Vector2D pos = t->getComponent<TransformComponent>().position;
		if (pos.x < Game::camera.x + camera.w + BLOCK_WIDTH * SCALE &&
			pos.x > Game::camera.x - BLOCK_WIDTH * SCALE &&
			pos.y < Game::camera.y + camera.h + BLOCK_HEIGHT * SCALE &&
			pos.y > Game::camera.y - BLOCK_HEIGHT * SCALE)
		{

			t->draw();
		}
	}

	for (auto& t : tiles)
	{
		Vector2D pos = t->getComponent<TransformComponent>().position;
		if (pos.x < Game::camera.x + camera.w + BLOCK_WIDTH * SCALE &&
			pos.x > Game::camera.x - BLOCK_WIDTH * SCALE &&
			pos.y < Game::camera.y + camera.h + BLOCK_HEIGHT * SCALE &&
			pos.y > Game::camera.y - BLOCK_HEIGHT * SCALE)
		{

			if (t->hasComponent<HPComponent>())
			{
				if (t->getComponent<HPComponent>().hp > 0)
					t->draw();
			}
			else
				t->draw();
		}
	}



	playerHead.getComponent<TransformComponent>().position.x = player.getComponent<TransformComponent>().position.x + 8;
	playerHead.getComponent<TransformComponent>().position.y = player.getComponent<TransformComponent>().position.y;
	playerHead.getComponent<TransformComponent>().velocity.y = player.getComponent<TransformComponent>().velocity.y;
	playerHead.getComponent<TransformComponent>().xdir = player.getComponent<TransformComponent>().xdir;


	for (auto& p : characters)
	{
		Vector2D pos = p->getComponent<TransformComponent>().position;
		if (pos.x < Game::camera.x + camera.w + BLOCK_WIDTH*SCALE &&
			pos.x > Game::camera.x - BLOCK_WIDTH * SCALE &&
			pos.y < Game::camera.y + camera.h + BLOCK_HEIGHT*SCALE &&
			pos.y > Game::camera.y - BLOCK_HEIGHT * SCALE)
		{

			p->draw();
		}
	}


	//player.draw();
	playerHead.draw();


	for (auto p : pickups) {
		Vector2D pos = p->getComponent<TransformComponent>().position;
		if (pos.x < Game::camera.x + camera.w + BLOCK_WIDTH * SCALE &&
			pos.x > Game::camera.x - BLOCK_WIDTH * SCALE &&
			pos.y < Game::camera.y + camera.h + BLOCK_HEIGHT * SCALE &&
			pos.y > Game::camera.y - BLOCK_HEIGHT * SCALE)
		{

			p->draw();
		}

	}


	for (auto& p : projectiles)
	{
		Vector2D pos = p->getComponent<TransformComponent>().position;
		if (pos.x < Game::camera.x + camera.w + BLOCK_WIDTH * SCALE &&
			pos.x > Game::camera.x - BLOCK_WIDTH * SCALE &&
			pos.y < Game::camera.y + camera.h + BLOCK_HEIGHT * SCALE &&
			pos.y > Game::camera.y - BLOCK_HEIGHT * SCALE)
		{

			p->draw();
		}

	}

	UIBar.draw();


	for (auto& u : UI)
	{
			u->draw();
	}
	actualGun.draw();
	portal.draw();
	manager.refresh();
	SDL_RenderPresent(renderer);
}

void Game::pauseRender(std::string text)
{

	//SDL_RenderClear(renderer);
	background.draw();

	if (Game::pause) resumeTitle.getComponent<UILabel>().SetLabelText("Pause", "TitleFont");
	else if (Game::died) resumeTitle.getComponent<UILabel>().SetLabelText("You Died!", "TitleFont");
	else if (Game::won) resumeTitle.getComponent<UILabel>().SetLabelText("You Won!", "TitleFont");


	if (Game::won || Game::died) {
		if (Game::pauseGui == 0)
			resumeButton.getComponent<UILabel>().ChangeColor(lightGrey);
		else resumeButton.getComponent<UILabel>().ChangeColor(grey);
	}
	else {
		if (Game::pauseGui == 0)
			resumeButton.getComponent<UILabel>().ChangeColor(white);
		else resumeButton.getComponent<UILabel>().ChangeColor(black);
	}

	if (Game::pauseGui == 1)
		exitButton.getComponent<UILabel>().ChangeColor(white);
	else exitButton.getComponent<UILabel>().ChangeColor(black);

	resumeTitle.draw();
	resumeButton.draw();
	exitButton.draw();

	SDL_RenderPresent(renderer);
}

void Game::exit()
{
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
}


void Game::clean()
{
	for (auto& t : tiles)
	{
		t->destroy();
	}
	for (auto& e : enemies)
	{
		e->destroy();
	}
	for (auto& p : pck)
	{
		p->destroy();
	}
	for (auto& t : tiles)
	{
		t->destroy();
	}
	for (auto& p : projectiles)
	{
		p->destroy();
	}
	for (auto& t : traps)
	{
		t->destroy();
	}


	manager.refresh();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	
	cout << "Game cleaned." << endl;
}

bool Game::running() {
	return isRunning;
}