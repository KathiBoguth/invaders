#pragma once

#include <cstdlib>
#include "time.h"


class AlienBehaviourComponent : public Component
{
	bool dirRight = true;

	float time_last_bomb;

	ObjectPool<Bomb>* bombs_pool;

public:

	virtual ~AlienBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Bomb>* bombs_pool)
	{
		Component::Create(engine, go, game_objects);
		this->bombs_pool = bombs_pool;
		srand(time(NULL));

	}

	void Update(float dt)
	{
		if (dirRight) {
			go->horizontalPosition += ALIEN_SPEED * dt;
			if (go->horizontalPosition > 640-35) //width: 640, height: 480
			{
				go->verticalPosition += 35;
				dirRight = false;
			}
		}
		else {
			go->horizontalPosition -= ALIEN_SPEED * dt;
			if (go->horizontalPosition < 0) 
			{
				go->verticalPosition += 35;
				dirRight = true;
			}
		}

		if (CanFire()) {
			Bomb* bomb = bombs_pool->FirstAvailable();
			if (bomb != NULL)
			{
				bomb->Init(go->horizontalPosition, go->verticalPosition);
				game_objects->insert(bomb);
			}
		}

	}

	bool CanFire()
	{
		if ((engine->getElapsedTime() - time_last_bomb) < BOMB_TIME_INTERVAL / game_speed || rand() > 1 )
			return false;

		time_last_bomb = engine->getElapsedTime();

		SDL_Log("bomb!");
		return true;
	}
};


// rockets are shot by the player towards the aliens
class Alien : public GameObject
{

public:

	virtual void Init(double xPos, double yPos)
	{
		SDL_Log("Alien::Init");
		GameObject::Init();

		horizontalPosition = xPos;
		verticalPosition = yPos;
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			SDL_Log("Alien::Hit");
		}
	}
};