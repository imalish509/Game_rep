#ifndef __LEVEL_H__
#define __LEVEL_H__


#include "cocos2d.h"
#include <vector>

using namespace std;
using namespace cocos2d;

class Level : public Object
{
public:

	TMXTiledMap *map;
	void loadMap(const char *name);
	TMXTiledMap *getMap();
	vector<Rect> getHazardsX(Point point, int direction);
	vector<Rect> getHazardsY(Point point, int direction);
	
	Point tileCoordinateToPosition(Point point);
	Point positionToTileCoordinate(Point point);
	vector<Rect> getCollisionTilesY(Point point, int direction);
	vector<Rect> getCollisionTilesX(Point point, int direction);
	Level(void);
	virtual ~Level(void);
};
#endif // __LEVEL_H__