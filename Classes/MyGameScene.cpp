#include "LoseScene.h"
#include "MyGameScene.h"
#include "HelloWorldScene.h"
#include "EndScene.h"
#include <iostream>
#include <stdio.h>

Scene* MyGameScene::createScene()
{
	auto scene = Scene::create();

	auto layer = MyGameScene::create();

	scene->addChild(layer);

	return scene;
}

bool MyGameScene::init()
{


	level = new Level();
	level->loadMap("level.tmx");
	level->retain();

	auto director = Director::getInstance();
	level->getMap()->setScale(SCALE_FACTOR);

	this->addChild(level->getMap());

	player = Player::create();
	player->retain();
	this->addChild(player);

	Point point = Point(10, 2);
	player->setPosition(level->tileCoordinateToPosition(point));

	score = player->getPositionX();

	Point origin = Director::getInstance()->getVisibleOrigin();
	Size wsize = Director::getInstance()->getVisibleSize();

	cameraTarget = Sprite::create();
	cameraTarget->setPositionX(player->getPositionX());
	cameraTarget->setPositionY(wsize.height / 2 + origin.y);
	cameraTarget->retain();
	this->addChild(cameraTarget);
	
	this->schedule(schedule_selector(MyGameScene::updatePlayer));

	camera = Follow::create(cameraTarget, Rect::ZERO);
	camera->retain();
	loadEnemies();

	this->runAction(camera->clone());

	auto vsize = Director::getInstance()->getVisibleSize();

	auto pos = cameraTarget->getPosition();
	auto pointLabel = Point(vsize.width / 2.4f + origin.x, vsize.height / 2.3f + origin.y);

	m_scoreLabel = Label::createWithSystemFont("Score = ", "Arial", 20);
	m_score = Label::createWithSystemFont("0", "Arial", 20);
	m_labelLives = Label::createWithSystemFont("Lives = ", "Arial", 20);
	m_label = Label::createWithSystemFont("0", "Arial", 20);

	//m_scoreLabel->setPosition(Vec2(pos.x - pointLabel.x, pos.y + pointLabel.y - m_scoreLabel->getContentSize().height));
	//m_score->setPosition(Vec2(m_scoreLabel->getPositionX() + m_scoreLabel->getContentSize().width / 1.6f, m_scoreLabel->getPositionY()));
	//m_labelLives->setPosition(Vec2(pos.x - pointLabel.x, pos.y + pointLabel.y));
	//m_label->setPosition(Vec2(m_labelLives->getPositionX() + m_labelLives->getContentSize().width / 1.6f, m_labelLives->getPositionY()));

	m_label->setPosition(level->tileCoordinateToPosition(point));
	m_labelLives->setPosition(level->tileCoordinateToPosition(point));
	m_score->setPosition(level->tileCoordinateToPosition(point));
	m_scoreLabel->setPosition(level->tileCoordinateToPosition(point));

	this->addChild(m_score);
	this->addChild(m_scoreLabel);
	this->addChild(m_label);
	this->addChild(m_labelLives);

	shoot();

	return true;
}

void MyGameScene::loadEnemies()
{

	enemy1 = Sprite::create("94.png");
	enemy1->setPosition(level->tileCoordinateToPosition(Point(33, 2)));
	enemy1->setAnchorPoint(Point::ZERO);
	enemy1->setScale(ENEMY_SCALE_FACTOR);
	enemy1->setFlippedX(true);
	enemy1->retain();

	enemyList.push_back(enemy1);
	this->addChild(enemy1);

	Sprite *enemy2 = Sprite::create("94.png");
	enemy2->setPosition(level->tileCoordinateToPosition(Point(44, 2)));
	enemy2->setAnchorPoint(Point::ZERO);
	enemy2->setScale(ENEMY_SCALE_FACTOR);
	enemy2->setFlippedX(true);
	enemy2->retain();

	enemyList.push_back(enemy2);
	this->addChild(enemy2);

	Sprite *enemy3 = Sprite::create("94.png");
	enemy3->setPosition(level->tileCoordinateToPosition(Point(55, 2)));
	enemy3->setAnchorPoint(Point::ZERO);
	enemy3->setScale(ENEMY_SCALE_FACTOR);
	enemy3->setFlippedX(true);
	enemy3->retain();

	enemyList.push_back(enemy3);
	this->addChild(enemy3);

	Sprite *enemy4 = Sprite::create("94.png");
	enemy4->setPosition(level->tileCoordinateToPosition(Point(100, 2)));
	enemy4->setAnchorPoint(Point::ZERO);
	enemy4->setScale(ENEMY_SCALE_FACTOR);
	enemy4->setFlippedX(true);
	enemy4->retain();

	enemyList.push_back(enemy4);
	this->addChild(enemy4);

	Sprite *enemy5 = Sprite::create("94.png");
	enemy5->setPosition(level->tileCoordinateToPosition(Point(100, 6)));
	enemy5->setAnchorPoint(Point::ZERO);
	enemy5->setScale(ENEMY_SCALE_FACTOR);
	enemy5->setFlippedX(true);
	enemy5->retain();

	enemyList.push_back(enemy5);
	this->addChild(enemy5);
}

void MyGameScene::shoot()
{
	auto contsize = enemy1->getContentSize() / 2;
	auto sprite = Sprite::create("fire.png");
	sprite->setPosition(enemy1->getPositionX(), enemy1->getPositionY() + contsize.height);
	sprite->setAnchorPoint(Point::ZERO);
	auto action = MoveTo::create(4, level->tileCoordinateToPosition(Vec2(10, 2.5f)));
	auto placeb = place->create(Vec2(level->tileCoordinateToPosition(Vec2(10, 2))));
	auto target = TargetedAction::create(sprite, placeb);
	auto bot = Speed::create(target, 1.4f);
	sprite->runAction(action);
	this->addChild(sprite);
}

void MyGameScene::updatePlayer(float interval) {


	if (std::find(heldKeys.begin(), heldKeys.end(), SPACEBAR) != heldKeys.end()) {

		if (player->grounded && player->velocity_y <= 0) {

			player->velocity_y = PLAYER_JUMP_VELOCITY;
			player->jumping = true;
			player->grounded = false;
		}
	}

	if (std::find(heldKeys.begin(), heldKeys.end(), RIGHT_ARROW) != heldKeys.end()) {

		player->velocity_x = PLAYER_MAX_VELOCITY;

		player->facing_right = true;
	}

	if (std::find(heldKeys.begin(), heldKeys.end(), LEFT_ARROW) != heldKeys.end()) {

		player->velocity_x = -PLAYER_MAX_VELOCITY;
		player->facing_right = false;
	}

	player->velocity_y -= GRAVITY;

	Rect player_rect = player->getBoundingBox();

	Point tmp;
    vector<Rect> tiles;
	vector<Rect> hazards;
	tiles.clear();

	tmp = level->positionToTileCoordinate(Point(player->getPositionX() + player->player_size.width * 0.5f,
		player->getPositionY() + player->player_size.height * 0.5f));

	if (player->velocity_x > 0) {
		tiles = level->getCollisionTilesX(tmp, 1);
	}
	else {
		tiles = level->getCollisionTilesX(tmp, -1);
	}
	player_rect.setRect(
		player->getBoundingBox().getMinX() + player->velocity_x,
		player->getBoundingBox().getMinY() + 1.0f,
		player->player_size.width,
		player->player_size.height
	);

	for (Rect tile : tiles) {
		if (player_rect.intersectsRect(tile)) {
			player->velocity_x = 0;
			break;
		}
	}

	tiles.clear();

	if (player->velocity_y > 0) {
		tiles = level->getCollisionTilesY(tmp, 1);
	}
	else if (player->velocity_y < 0) {
		tiles = level->getCollisionTilesY(tmp, -1);
	}

	player_rect.setRect(
		player->getBoundingBox().getMinX(),
		player->getBoundingBox().getMinY(),
		player->player_size.width,
		player->player_size.height
	);

	for (Rect tile : tiles) {

		if (tile.intersectsRect(player_rect)) {
			if (player->velocity_y > 0) {

				player->setPositionY(player->getPositionY() - player->velocity_y);

			}
			else {

				player->setPositionY(tile.getMaxY());
				player->grounded = true;
				player->jumping = false;

			}
			player->velocity_y = 0;
			break;

		}
		player->grounded = false;
	}

	for (Sprite *tile : enemyList) {

		if (tile->getBoundingBox().intersectsRect(player_rect)) {

			Point p = level->tileCoordinateToPosition(Point(10, 2));
			player->setPosition(p);
			lives--;
			score = player->getPositionX();
		}
	}

	if (player->velocity_x > 0) {
		hazards = level->getHazardsX(tmp, 1);
	}
	else {
		hazards = level->getHazardsX(tmp, -1);
	}

	if (player->velocity_y > 0) {
		hazards = level->getHazardsY(tmp, 1);
	}
	else if (player->velocity_y < 0) {
		hazards = level->getHazardsY(tmp, -1);
	}

	player_rect.setRect(
		player->getBoundingBox().getMinX() + player->velocity_x,
		player->getBoundingBox().getMinY() + 1.0f,
		player->player_size.width,
		player->player_size.height
	);

	for (Rect tile : hazards) {
		if (player_rect.intersectsRect(tile)) {
			Point p = level->tileCoordinateToPosition(Point(10, 2));
			player->setPosition(p);
			lives--;
			score = 0;
		}
	}
	if (player->velocity_x > 0 || player->velocity_x < 0)
	{
		score = player->getPositionX();
	}

	player->updateState(interval);
	player->velocity_x = 0;
	labels();
	cameraTarget->setPositionX(player->getPositionX());
}

void MyGameScene::labels()
{
	//if(m_label)
	//	{
	//	m_label->removeFromParentAndCleanup(true);
	//	m_labelLives->removeFromParentAndCleanup(true);
	//	m_score->removeFromParentAndCleanup(true);
	//	m_scoreLabel->removeFromParentAndCleanup(true);
	//	}

	Point point = player->getPosition();

	m_label->setString(to_string(lives));
	m_score->setString(to_string(score));

	m_label->setPosition(point);
	m_labelLives->setPosition(point);
	m_score->setPosition(point);
	m_scoreLabel->setPosition(point);

	errorUp();

}

void MyGameScene::errorUp()
{
	Point errPoint;
	errPoint = level->positionToTileCoordinate(Point(player->getPositionX(), player->getPositionY()));
		if (errPoint.x < 1 || errPoint.y < 1)
		{
			Point p = level->tileCoordinateToPosition(Point(10, 2));
			player->setPosition(p);
			lives--;
			score = player->getPositionX();
		}
		if (errPoint.x > level->map->getMapSize().width - 5.0f)
		{
			auto scene = EndScene::createScene();
			Director::getInstance()->pushScene(scene);
		}
		if (lives <= 0)
		{
			auto scene = LoseScene::createScene();
			Director::getInstance()->pushScene(scene);
		}
}

void MyGameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{

	if (std::find(heldKeys.begin(), heldKeys.end(), keyCode) == heldKeys.end()) {
		heldKeys.push_back(keyCode);
	}

}

void MyGameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), keyCode), heldKeys.end());
}

void MyGameScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
}

MyGameScene::MyGameScene(void)
{
	setKeyboardEnabled(true);
	collidesX = false;
	lives = 3.0f;
	score = 0;
}
MyGameScene::~MyGameScene(void)
{
}
