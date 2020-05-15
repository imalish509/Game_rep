#include "cocos2d.h"

class BaseBullet
{
public:
	BaseBullet() = default;
	virtual ~BaseBullet() = default;

	virtual bool create(const std::string& fileName);
	virtual void init(const float lifeTime, const cocos2d::Vec2& initPoint, const cocos2d::Vec2& finishPoint, const std::function<void()>& endCallBack);

private:
	cocos2d::Sprite* m_bullet = nullptr;
	float m_lifeTime = 0.f;
};
