#pragma once

bool change_direction = false;

class Game : public GameObject
{
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/

	AvancezLib* engine;

	ObjectPool<Rocket> rockets_pool;	// used to instantiate rockets

	Player * player;

	ObjectPool<Bomb> bombs_pool;

	ObjectPool<Alien> alien_pool;

	Sprite * life_sprite;
	bool game_over = false;

	unsigned int score = 0;

public:

	virtual void Create(AvancezLib* engine)
	{
		SDL_Log("Game::Create");

		this->engine = engine;



		player = new Player();
		PlayerBehaviourComponent * player_behaviour = new PlayerBehaviourComponent();
		player_behaviour->Create(engine, player, &game_objects, &rockets_pool);
		RenderComponent * player_render = new RenderComponent();
		player_render->Create(engine, player, &game_objects, "data/player.bmp");

		player->Create();
		player->AddComponent(player_behaviour);
		player->AddComponent(player_render);
		player->AddReceiver(this);
		game_objects.insert(player);

		rockets_pool.Create(30);
		for (auto rocket = rockets_pool.pool.begin(); rocket != rockets_pool.pool.end(); rocket++)
		{
			RocketBehaviourComponent * behaviour = new RocketBehaviourComponent();
			behaviour->Create(engine, *rocket, &game_objects);
			RenderComponent * render = new RenderComponent();
			render->Create(engine, *rocket, &game_objects, "data/rocket.bmp");
			(*rocket)->Create();
			(*rocket)->AddComponent(behaviour);
			(*rocket)->AddComponent(render);
		}

		

		alien_pool.Create(55);
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 11; j++) {
				Alien* alien = new Alien();
				AlienBehaviourComponent* alien_behaviour = new AlienBehaviourComponent();
				alien_behaviour->Create(engine, alien, &game_objects, &bombs_pool);
				RenderComponent* alien_render = new RenderComponent();
				alien_render->Create(engine, alien, &game_objects, "data/enemy_0.bmp");
				alien->Create();
				alien->AddComponent(alien_behaviour);
				alien->AddComponent(alien_render);
				alien->AddReceiver(this);
				game_objects.insert(alien);
				alien->Init(j * 35, i * 35);

				CollideComponent* alienCollide = new CollideComponent();
				alienCollide->Create(engine, alien, &game_objects, (ObjectPool<GameObject>*) & rockets_pool);
				alien->AddComponent(alienCollide);
			}
		}

		

		bombs_pool.Create(30);
		for (auto bomb = bombs_pool.pool.begin(); bomb != bombs_pool.pool.end(); bomb++)
		{
			BombBehaviourComponent* behaviour = new BombBehaviourComponent();
			behaviour->Create(engine, *bomb, &game_objects);
			RenderComponent* render = new RenderComponent();
			render->Create(engine, *bomb, &game_objects, "data/bomb.bmp");
			(*bomb)->Create();
			(*bomb)->AddComponent(behaviour);
			(*bomb)->AddComponent(render);
		}

		CollideComponent* playerCollide = new CollideComponent();
		playerCollide->Create(engine, player, &game_objects, (ObjectPool<GameObject>*) &bombs_pool);
		player->AddComponent(playerCollide);
		

		life_sprite = engine->createSprite("data/player.bmp");
	}

	virtual void Init()
	{
		player->Init();

		enabled = true;
	}

	virtual void Update(float dt)
	{
		AvancezLib::KeyStatus keys;
		engine->getKeyStatus(keys);
		if (keys.esc) {
			Destroy();
			engine->quit();
		}

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Update(dt);

	}

	virtual void Draw()
	{
		//... Draw user interface elements here

		engine->swapBuffers();
		engine->clearWindow();
	}

	virtual void Receive(Message m)
	{
		if (m == GAME_OVER)
		{

		}

		if (m == ALIEN_HIT) 
		{

		}
	}

	virtual void Destroy()
	{
		SDL_Log("Game::Destroy");

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Destroy();

		life_sprite->destroy();
	
		rockets_pool.Destroy();
		delete player;
	}
};