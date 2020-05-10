#include "EndScene.h"
#include "MyGameScene.h"
USING_NS_CC;

Scene* EndScene::createScene()
{
	return EndScene::create();
}

bool EndScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float xx = visibleSize.width;
	float yy = visibleSize.height;

	auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(EndScene::menuCloseCallback, this));
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	auto menu1 = Menu::create(closeItem, NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1, 1);

	m_label = Label::createWithTTF("YOU WIN", "fonts/Marker Felt.ttf", 40);
	m_label->setPosition(Vec2(origin.x + xx / 2,
		origin.y + yy - m_label->getContentSize().height));

	this->addChild(m_label, 1);

	auto menuItem4 = MenuItemImage::create("Play1.png", "Play.jpg", CC_CALLBACK_1(EndScene::ImageButton, this));
	menuItem4->setPosition(Point(xx / 2, (yy / 5) * 2));

	auto *menu = Menu::create(menuItem4, NULL);
	menu->setScale(0.3, 0.3);
	menu->setPosition(Point(0, 0));
	this->addChild(menu);

	return true;
}

void EndScene::ImageButton(Ref* pSender)
{
	CCLOG("ImageButton");
	auto scene = MyGameScene::createScene();
	Director::getInstance()->pushScene(scene);
}

void EndScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}