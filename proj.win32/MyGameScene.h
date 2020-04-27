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

	//Label *label;

	vector<Sprite*> enemyList;

	boolean collidesX;

	float stutteringFix;
	int lives;
	//void deathCheck(Label *label);

	Follow *camera;

	DrawNode *rectWithBorder;

	vector<EventKeyboard::KeyCode> heldKeys;

	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	void updateScene(float interval);
	void updatePlayer(float interval);
	void loadEnemies();
	void errorUp(float count);

	static Scene* createScene();

	virtual bool init();

	void menuCloseCallback(Ref* pSender);

	CREATE_FUNC(MyGameScene);

	MyGameScene(void);
	virtual ~MyGameScene(void);
};

#endif // __MYGAMESCENE_H__

