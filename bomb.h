#pragma once

class BombBehaviourComponent : public Component
{
public:

	void Update(float dt)
	{
		go->verticalPosition += BOMB_SPEED * dt;

		if (go->verticalPosition >480) 
			go->enabled = false;
	}
};


class Bomb : public GameObject
{

public:

	virtual void Init(double xPos, double yPos)
	{
		SDL_Log("Bomb::Init");
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
			SDL_Log("Bomb::Hit");
		}
	}
};