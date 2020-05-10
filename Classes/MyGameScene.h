#ifndef __MYGAMESCENE_H__
#define __MYGAMESCENE_H__
#include "cocos2d.h"
#include "Level.h"
#include "Player.h"
#include "Keyboard.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;
USING_NS_CC;

class MyGameScene : public Layer
{
public:

	//FIELDS
	Level  *level;

	Player *player;

	Sprite *player_sprite;
	Sprite *cameraTarget;

	Animate *walkRight;
	Animate *jumping;
	Animate *falling;

	vector<Sprite*> enemyList;

	bool collidesX;

	float stutteringFix;


	Follow *camera;

	vector<EventKeyboard::KeyCode> heldKeys;
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	void updatePlayer(float interval);

	void loadEnemies();

	void errorUp();

	void labels();

	static Scene* createScene();

	void shoot();

	virtual bool init();

	void menuCloseCallback(Ref* pSender);

	CREATE_FUNC(MyGameScene);

	MyGameScene(void);
	virtual ~MyGameScene(void);

private:

	Sprite *enemy1;
	AccelDeccelAmplitude* amp;
	AccelAmplitude* cam;
	Spawn * spawn;
	BezierBy bez;
	ActionEase* act;
	FiniteTimeAction* fineact;
	Place *place;
	TargetedAction* join;
	Speed *speed;
	Sprite *fire;
	int score;
	int lives;
	Label *m_label = nullptr;
	Label *m_labelLives = nullptr;
	Label *m_scoreLabel = nullptr;
	Label *m_score = nullptr;
};

#endif // __MYGAMESCENE_H__

