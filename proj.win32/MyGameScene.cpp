#include "LoseScene.h"
#include "MyGameScene.h"
#include "HelloWorldScene.h"
#include "EndScene.h"

Scene* MyGameScene::createScene()
{
	auto scene = Scene::create();

	auto layer = MyGameScene::create();

	scene->addChild(layer);

	return scene;
}

bool MyGameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	level = new Level();
	level->loadMap("level.tmx");
	level->retain();

	auto director = Director::getInstance();
	level->getMap()->setScale(SCALE_FACTOR);

	this->addChild(level->getMap());

	player = Player::create();
	player->retain();

	Point point = Point(10, 2);
	player->setPosition(level->tileCoordinateToPosition(point));

	Point origin = Director::getInstance()->getVisibleOrigin();
	Size wsize = Director::getInstance()->getVisibleSize();
	Point *center = new Point(wsize.width / 2 + origin.x, wsize.height / 2 + origin.y);

	cameraTarget = Sprite::create();
	cameraTarget->setPositionX(player->getPositionX());
	cameraTarget->setPositionY(wsize.height / 2 + origin.y);
	cameraTarget->retain();

	this->addChild(player);
	this->schedule(schedule_selector(MyGameScene::updateScene));
	this->addChild(cameraTarget);

	//deathCheck(label);

	rectWithBorder = DrawNode::create();
	Vec2 vertices[] =
	{
		Vec2(0, player->player_size.height),
		Vec2(player->player_size.width, player->player_size.height),
		Vec2(player->player_size.width, 0),
		Vec2(0,0)
	};
	rectWithBorder->drawPolygon(vertices, 4, Color4F(0.0f, 0.3f, 0.3f, 1), 0, Color4F(0.0f, 0.2f, 0.0f, 1));

	addChild(rectWithBorder);

	camera = Follow::create(cameraTarget, Rect::ZERO);
	camera->retain();
	loadEnemies();

	this->runAction(camera);
	return true;
}

void MyGameScene::loadEnemies() {

	Sprite *enemy1 = Sprite::create("94.png");
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

void MyGameScene::updateScene(float delta) {

	cameraTarget->setPositionX(player->getPositionX());

	this->updatePlayer(delta);

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
		}
	}

	player->updateState(interval);
	player->velocity_x = 0;
	this->errorUp(interval);
}

//void MyGameScene::deathCheck(Label *label)
//{
//	char *k;
//	char buf[20];
//	k = itoa(lives, buf, 10);
//	label = Label::createWithTTF(k, "fonts/Marker Felt.ttf", 40);
//	label->setPosition(Point(cameraTarget->getPositionX(), cameraTarget->getPositionY()));
//	this->addChild(label);
//}

void MyGameScene::errorUp(float count)
{
	Point errPoint;
	errPoint = level->positionToTileCoordinate(Point(player->getPositionX(), player->getPositionY()));
		if (errPoint.x < 1 || errPoint.y < 1)
		{
			Point p = level->tileCoordinateToPosition(Point(10, 2));
			player->setPosition(p);
			lives--;
			//this->deathCheck(label);
		}
		if (errPoint.x > level->map->getMapSize().width - 5.0f)
		{
			auto scene = EndScene::createScene();
			Director::getInstance()->pushScene(scene);
		}
		if (lives == 0)
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
	lives = 3;
}
MyGameScene::~MyGameScene(void)
{
}
