#include "BaseBullet.h"

using namespace cocos2d;

bool BaseBullet::create(const std::string & fileName)
{
	m_bullet = Sprite::create(fileName);
	return m_bullet != nullptr;
}

void BaseBullet::init(const float lifeTime, const cocos2d::Vec2& initPoint, const cocos2d::Vec2& finishPoint, const std::function<void()>& endCallBack)
{
	auto pAction = Sequence::create(
		MoveTo::create(lifeTime, finishPoint)
		, CallFunc::create(endCallBack)
		, nullptr);

	m_bullet->setPosition(initPoint);
	m_bullet->runAction(pAction);
}
