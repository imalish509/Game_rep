#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class Player : public Sprite
{

public:
	enum State {
		Standing, Walking, Jumping
	};

	State state;

	float velocity_x;
	float velocity_y;

	int direction;
	int facing_left;
	int facing_right;
	int facing_up;
	int facing_down;
	int facing_down_left;
	int facing_down_right;
	int facing_up_left;
	int facing_up_right;
	bool jumping;
	bool facingRight;
	bool grounded;
	float stateTime;

	Size player_size;

	Point position;
	Point velocity;

	Animate *walk;

	Rect getCollisionBox();
	Rect getUpperCollisionBox();

	static Player* create();

	void updateState(float delta);
	void setupAnimation(const char* name);

	Player(void);
	virtual ~Player(void);
};