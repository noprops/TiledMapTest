#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    _tiledMap = TMXTiledMap::create("dungeon1.tmx");
    
    _HX = origin.x;
    _HY = origin.y;
    _LX = origin.x - (_tiledMap->getContentSize().width - visibleSize.width);
    _LY = origin.y - (_tiledMap->getContentSize().height - visibleSize.height);
    
    _tiledMap->setAnchorPoint(Vec2::ZERO);
    _tiledMap->setPosition(_HX, _LY);
    addChild(_tiledMap);
    
    addLabels();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void HelloWorld::addLabels()
{
    Node* debugLayer = Node::create();
    _tiledMap->addChild(debugLayer, 100);
    
    Size mapSize = _tiledMap->getMapSize();
    Size tileSize = _tiledMap->getTileSize() / Director::getInstance()->getContentScaleFactor();
    CCLOG("mapSize = (%.0f,%.0f) tileSize = (%.0f,%.0f)",mapSize.width, mapSize.height, tileSize.width, tileSize.height);
    for (int x = 0; x < mapSize.width; x++) {
        for (int y = 0; y < mapSize.height; y++) {
            //Label* label = Label::createWithBMFont("font.fnt", StringUtils::format("(%d,%d)",x,y));
            Label* label = Label::createWithSystemFont(StringUtils::format("(%d,%d)",x,y), "Helvetica", 5);
            label->setAnchorPoint(Vec2(.5f,.5f));
            Vec2 p = Vec2(x * tileSize.width, (mapSize.height - y - 1) * tileSize.height) + Vec2(tileSize.width / 2, tileSize.height / 2);
            label->setPosition(p);
            label->setTextColor(Color4B::WHITE);
            debugLayer->addChild(label);
        }
    }
}

#pragma mark - touch

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    _touchStartPosition = convertTouchToNodeSpace(touch);
    _touchStartTiledMapPosition = _tiledMap->getPosition();
    return true;
}
void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
}
void HelloWorld::onTouchCancelled(Touch* touch, Event* event)
{
}
void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
    Vec2 move = convertTouchToNodeSpace(touch) - _touchStartPosition;
    _tiledMap->setPosition(clampf(_touchStartTiledMapPosition.x + move.x,
                                  _LX,
                                  _HX),
                           clampf(_touchStartTiledMapPosition.y + move.y,
                                  _LY,
                                  _HY));
}
