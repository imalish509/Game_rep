#ifndef __ENDSCENE_H__
#define __ENDSCENE_H__
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

#include "cocos2d.h"

class EndScene : public Scene
{
public:
	static Scene* createScene();

	virtual bool init();

	void menuCloseCallback(Ref* pSender);

	CREATE_FUNC(EndScene);

	void ImageButton(Ref* pSender);
	
private:

	Label* m_label = nullptr;
};

#endif // __ENDSCENE_H__
