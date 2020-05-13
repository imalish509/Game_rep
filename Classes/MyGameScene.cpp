#include "MyGameScene.h"

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

	auto director = Director::getInstance();
	level->getMap()->setScale(SCALE_FACTOR);

	this->addChild(level->getMap());

	player = Player::create();
	this->addChild(player);
	
	Point point = Point(10, 2);
	player->setPosition(level->tileCoordinateToPosition(point));

	score = player->getPositionX();

	Point origin = Director::getInstance()->getVisibleOrigin();
	Size wsize = Director::getInstance()->getVisibleSize();

	cameraTarget = Sprite::create();
	cameraTarget->setPositionX(player->getPositionX());
	cameraTarget->setPositionY(wsize.height / 2 + origin.y);

	this->addChild(cameraTarget);
	
	this->schedule(schedule_selector(MyGameScene::updatePlayer));
	
	camera = Follow::create(cameraTarget, Rect::ZERO);

	loadEnemies();

	this->runAction(camera);
	
	auto pos = cameraTarget->getPosition();

	m_scoreLabel = Label::createWithSystemFont("Score = ", "Arial", 20);
	m_score = Label::createWithSystemFont("0", "Arial", 20);
	m_labelLives = Label::createWithSystemFont("Lives = ", "Arial", 20);
	m_label = Label::createWithSystemFont("0", "Arial", 20);

	this->addChild(m_score);
	this->addChild(m_scoreLabel);
	this->addChild(m_label);
	this->addChild(m_labelLives);

	return true;
}

void MyGameScene::loadEnemies()
{
	enemy1 = Sprite::create("94.png");
	enemy1->setPosition(level->tileCoordinateToPosition(Point(33, 2)));
	enemy1->setAnchorPoint(Point::ZERO);
	enemy1->setScale(ENEMY_SCALE_FACTOR);
	enemy1->setFlippedX(true);

	enemyList.push_back(enemy1);
	this->addChild(enemy1);

	Sprite *enemy2 = Sprite::create("94.png");
	enemy2->setPosition(level->tileCoordinateToPosition(Point(44, 2)));
	enemy2->setAnchorPoint(Point::ZERO);
	enemy2->setScale(ENEMY_SCALE_FACTOR);
	enemy2->setFlippedX(true);

	enemyList.push_back(enemy2);
	this->addChild(enemy2);

	Sprite *enemy3 = Sprite::create("94.png");
	enemy3->setPosition(level->tileCoordinateToPosition(Point(55, 2)));
	enemy3->setAnchorPoint(Point::ZERO);
	enemy3->setScale(ENEMY_SCALE_FACTOR);
	enemy3->setFlippedX(true);

	enemyList.push_back(enemy3);
	this->addChild(enemy3);

	Sprite *enemy4 = Sprite::create("94.png");
	enemy4->setPosition(level->tileCoordinateToPosition(Point(100, 2)));
	enemy4->setAnchorPoint(Point::ZERO);
	enemy4->setScale(ENEMY_SCALE_FACTOR);
	enemy4->setFlippedX(true);

	enemyList.push_back(enemy4);
	this->addChild(enemy4);

	Sprite *enemy5 = Sprite::create("94.png");
	enemy5->setPosition(level->tileCoordinateToPosition(Point(100, 6)));
	enemy5->setAnchorPoint(Point::ZERO);
	enemy5->setScale(ENEMY_SCALE_FACTOR);
	enemy5->setFlippedX(true);

	enemyList.push_back(enemy5);
	this->addChild(enemy5);
	this->schedule(schedule_selector(MyGameScene::fireCreate), 1);

}

void MyGameScene::fireCreate(float delay)
{
	if (fire)
	{
		firepos = level->positionToTileCoordinate(Vec2(fire->getPosition()));
		if (firepos.x < 20.0f)
		{
			fire->retain();
			fire->removeFromParent();
		}
	}

	auto placeb = level->positionToTileCoordinate(Vec2(level->tileCoordinateToPosition(Vec2(10, 2.5f))));
	auto enemypos = level->positionToTileCoordinate(Vec2(enemy1->getPositionX(), 2.5f));

	auto contsize = enemy1->getContentSize() / 2;

	auto action = MoveTo::create(enemypos.x / SPEED, level->tileCoordinateToPosition(Vec2(10, 2.6f)));

	fire = Sprite::create("fire.png");
	fire->setPosition(enemy1->getPositionX(), enemy1->getPositionY() + contsize.height);
	fire->setAnchorPoint(Point::ZERO);
	fire->runAction(action);

	fireList.push_back(fire);

	this->addChild(fire);

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
	vector<Rect> tilebonus;
	tiles.clear();
	tilebonus.clear();

	tmp = level->positionToTileCoordinate(Point(player->getPositionX() + player->player_size.width * 0.5f,
		player->getPositionY() + player->player_size.height * 0.5f));

	if (player->velocity_x > 0) {
		tiles = level->getCollisionTilesX(tmp, 1);
	}
	else {
		tiles = level->getCollisionTilesX(tmp, -1);
	}
	if (player->velocity_x > 0) {
		tilebonus = level->getBonusX(tmp, 1);
	}
	else {
		tilebonus = level->getBonusX(tmp, -1);
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
	for (Rect tile : tilebonus) {
		if (player_rect.intersectsRect(tile)) {
			player->velocity_x = 0;
			break;
		}
	}

	tiles.clear();
	tilebonus.clear();

	if (player->velocity_y > 0) {
		tiles = level->getCollisionTilesY(tmp, 1);
	}
	else if (player->velocity_y < 0) {
		tiles = level->getCollisionTilesY(tmp, -1);
	}
	if (player->velocity_y > 0) {
		tilebonus = level->getBonusY(tmp, 1);
	}
	else if (player->velocity_y < 0) {
		tilebonus = level->getBonusY(tmp, -1);
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

	for (Rect tile : tilebonus) {

		if (tile.intersectsRect(player_rect)) {
			if (player->velocity_y > 0) {

				player->setPositionY(player->getPositionY() - player->velocity_y);
				this->schedule(schedule_selector(MyGameScene::playerUp));
				this->unschedule(schedule_selector(MyGameScene::updatePlayer));
				lives--;
				
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

	for (Sprite* tile : fireList)
	{
		if (fire)
		{
			if (tile->getBoundingBox().intersectsRect(player_rect))
			{
				Point p = level->tileCoordinateToPosition(Point(10, 2));
				player->setPosition(p);
				lives--;
				score = 0;
			}
		}
	}

	player->updateState(interval);
	player->velocity_x = 0;
	labels();
	cameraTarget->setPositionX(player->getPositionX());
}

void MyGameScene::playerUp(float time)
{
	player->setColor(Color3B::RED);

	if (std::find(heldKeys.begin(), heldKeys.end(), RIGHT_ARROW) != heldKeys.end()) {

		player->velocity_x = PLAYER_BONUS_VELOCITY;

		player->facing_right = true;
	}

	if (std::find(heldKeys.begin(), heldKeys.end(), LEFT_ARROW) != heldKeys.end()) {

		player->velocity_x = -PLAYER_BONUS_VELOCITY;
		player->facing_right = false;
	}

	this->bonusUpdatePlayer(time);

}

void MyGameScene::bonusUpdatePlayer(float interval) {

	if (std::find(heldKeys.begin(), heldKeys.end(), SPACEBAR) != heldKeys.end()) {

		if (player->grounded && player->velocity_y <= 0) {

			player->velocity_y = PLAYER_JUMP_VELOCITY + 2.0f;
			player->jumping = true;
			player->grounded = false;
		}
	}

	if (std::find(heldKeys.begin(), heldKeys.end(), RIGHT_ARROW) != heldKeys.end()) {

		player->velocity_x = PLAYER_MAX_VELOCITY + 5.0f;

		player->facing_right = true;
	}

	if (std::find(heldKeys.begin(), heldKeys.end(), LEFT_ARROW) != heldKeys.end()) {

		player->velocity_x = -PLAYER_MAX_VELOCITY - 5.0f;
		player->facing_right = false;
	}

	player->velocity_y -= GRAVITY;

	Rect player_rect = player->getBoundingBox();

	Point tmp;
	vector<Rect> tiles;
	vector<Rect> hazards;
	vector<Rect> tilebonus;
	tiles.clear();
	tilebonus.clear();

	tmp = level->positionToTileCoordinate(Point(player->getPositionX() + player->player_size.width * 0.5f,
		player->getPositionY() + player->player_size.height * 0.5f));

	if (player->velocity_x > 0) {
		tiles = level->getCollisionTilesX(tmp, 1);
	}
	else {
		tiles = level->getCollisionTilesX(tmp, -1);
	}
	if (player->velocity_x > 0) {
		tilebonus = level->getBonusX(tmp, 1);
	}
	else {
		tilebonus = level->getBonusX(tmp, -1);
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
	for (Rect tile : tilebonus) {
		if (player_rect.intersectsRect(tile)) {
			player->velocity_x = 0;
			break;
		}
	}

	tiles.clear();
	tilebonus.clear();

	if (player->velocity_y > 0) {
		tiles = level->getCollisionTilesY(tmp, 1);
	}
	else if (player->velocity_y < 0) {
		tiles = level->getCollisionTilesY(tmp, -1);
	}
	if (player->velocity_y > 0) {
		tilebonus = level->getBonusY(tmp, 1);
	}
	else if (player->velocity_y < 0) {
		tilebonus = level->getBonusY(tmp, -1);
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

	for (Rect tile : tilebonus) {

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
	Point origin = Director::getInstance()->getVisibleOrigin();
	Size wsize = Director::getInstance()->getVisibleSize();

	auto posx = player->getPositionX();
	auto posy = wsize.height / 1.1f;

	m_label->setString(to_string(lives));
	m_score->setString(to_string(score));

	m_label->setPosition(posx - wsize.width / 2.4f + m_labelLives->getContentSize().width, posy);
	m_labelLives->setPosition(posx - wsize.width / 2.4f, posy);
	m_score->setPosition(posx - wsize.width / 2.4f + m_scoreLabel->getContentSize().width, posy - m_label->getContentSize().height);
	m_scoreLabel->setPosition(posx - wsize.width / 2.4f, posy - m_label->getContentSize().height);

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
	lives = 20.0f;
	score = 0;
}
MyGameScene::~MyGameScene(void)
{
}
