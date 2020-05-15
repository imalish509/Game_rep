#include "HelloWorldScene.h"
#include "MyGameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	return HelloWorld::create();
}

bool HelloWorld::init()
{
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float xx = visibleSize.width;
	float yy = visibleSize.height;

	auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	auto menu1 = Menu::create(closeItem, NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1, 1);

	auto label = Label::createWithTTF("MY GAME", "fonts/Marker Felt.ttf", 40);

	label->setPosition(Vec2(origin.x + xx / 2,
		origin.y + yy - label->getContentSize().height));

	this->addChild(label, 1);

	auto menuItem4 = MenuItemImage::create("Play1.png", "Play.jpg", CC_CALLBACK_1(HelloWorld::ImageButton, this));
	menuItem4->setPosition(Point(xx / 2, (yy / 5) * 2));

	auto *menu = Menu::create(menuItem4, NULL);
	menu->setScale(0.4, 0.4);
	menu->setPosition(Point(0, 0));
	this->addChild(menu);

	auto sprite = Sprite::create("HelloWorld.png");
	sprite->setPosition(Vec2(30, origin.y + visibleSize.height - 30));
	sprite->setScale(0.3, 0.3);
	this->addChild(sprite);

	return true;
}

void HelloWorld::ImageButton(cocos2d::Ref* pSender)
{
	CCLOG("ImageButton");
	auto scene = MyGameScene::createScene();
	Director::getInstance()->pushScene(scene);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

}