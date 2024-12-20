/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "MainScene.h"

using namespace ax;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Content/' in front of filenames in "
        "MainScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = _director->getVisibleSize();
    auto origin = _director->getVisibleOrigin();
    auto safeArea = _director->getSafeAreaRect();
    auto safeOrigin = safeArea.origin;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
        AX_CALLBACK_1(MainScene::menuCloseCallback, this));

    if (closeItem == nullptr || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = safeOrigin.x + safeArea.size.width - closeItem->getContentSize().width / 2;
        float y = safeOrigin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(
            Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    _rt = RenderTexture::create(visibleSize.width, visibleSize.height);
    _rt->setPositionNormalized(Vec2(0.5f, 0.5f));

    _dummyNode = Node::create(); // Do not add this to the scene. It's stored as a RefPtr, so it's automatically retained/released

    auto sprite = Sprite::create("HelloWorld.png");

    sprite->setPosition(visibleSize / 2);

    auto* movingSprite = Sprite::create("CloseSelected.png");
    movingSprite->setPosition(Vec2(visibleSize.x / 2 - 100, visibleSize.y / 2 - 85));
    movingSprite->setAnchorPoint(Vec2(0, 0));
    _dummyNode->addChild(movingSprite, 1);

    auto moveAction     = MoveBy::create(5, Vec2(100, 0));
    auto sequenceAction = Sequence::create(moveAction, moveAction->reverse(), nullptr);
    auto repeatAction   = RepeatForever::create(sequenceAction);
    movingSprite->runAction(repeatAction);

    _dummyNode->addChild(sprite);

    _normalView = Sprite::createWithTexture(_rt->getSprite()->getTexture(), _rt->getSprite()->getTextureRect());
    _normalView->setFlippedY(true);
    _normalView->setPositionNormalized(Vec2(0.5f, 0.6f));

    _flippedView = Sprite::createWithTexture(_rt->getSprite()->getTexture(), _rt->getSprite()->getTextureRect());
    _flippedView->setPositionNormalized(Vec2(0.5f, 0.32f));

    addChild(_normalView);
    addChild(_flippedView);

    return true;
}

void MainScene::onEnter()
{
    Scene::onEnter();
    _dummyNode->onEnter(); // Required since this node will not be part of the scene, but still needs to be activated
}

void MainScene::onExit()
{
    _dummyNode->onExit(); // Required since this node will not be part of the scene, but still needs to be de-activated
    Scene::onExit();
}


void MainScene::update(float delta)
{
    _rt->beginWithClear(0.f, 0.f, 0.f, 0);
    _dummyNode->visit();
    _rt->end();
}

void MainScene::menuCloseCallback(ax::Object* sender)
{
    // Close the axmol game scene and quit the application
    _director->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use
     * _director->end() as given above,instead trigger a custom event created in RootViewController.mm
     * as below*/

     // EventCustom customEndEvent("game_scene_close_event");
     //_eventDispatcher->dispatchEvent(&customEndEvent);
}
