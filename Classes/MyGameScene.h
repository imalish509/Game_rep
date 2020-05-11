#ifndef __MYGAMESCENE_H__
#define __MYGAMESCENE_H__
#include "cocos2d.h"
#include "Level.h"
#include "LoseScene.h"
#include "MyGameScene.h"
#include "HelloWorldScene.h"
#include "EndScene.h"
#include "Player.h"
#include "Keyboard.h"
#include <algorithm>
#include <iostream>
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
	vector<Sprite*> fireList;

	bool collidesX;

	float stutteringFix;

	Follow *camera;

	vector<EventKeyboard::KeyCode> heldKeys;
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	void updatePlayer(float interval);

	void loadEnemies();

	void errorUp();
	void playerUp(float time);
	void labels();

	static Scene* createScene();

	void fireCreate(float delay);
	void fireCheck();

	virtual bool init();

	void menuCloseCallback(Ref* pSender);

	CREATE_FUNC(MyGameScene);

	MyGameScene(void);
	virtual ~MyGameScene(void);

private:

	Point firepos;
	Sprite *enemy1;
	Sprite *fire = nullptr;
	int score;
	int lives;
	Label *m_label = nullptr;
	Label *m_labelLives = nullptr;
	Label *m_scoreLabel = nullptr;
	Label *m_score = nullptr;
};

#endif // __MYGAMESCENE_H__

