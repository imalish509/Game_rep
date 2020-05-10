#ifndef __LOSESCENE_H__
#define __LOSESCENE_H__
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

#include "cocos2d.h"

class LoseScene : public Scene
{
public:
	static Scene* createScene();

	virtual bool init();

	void menuCloseCallback(Ref* pSender);

	CREATE_FUNC(LoseScene);

	void ImageButton(Ref* pSender);

};

#endif // __LOSESCENE_H__
