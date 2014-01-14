#include "TestScene.h"
#include "jansson.h"
#include <fstream>
#include <iostream>

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

CCScene* TestScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    TestScene *layer = TestScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TestScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage * pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(TestScene::menuCloseCallback));
    
	pCloseItem -> setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
                                origin.y + pCloseItem->getContentSize().height/2));

    
    
    CCMenuItemImage * pNetBtn = CCMenuItemImage::create("CloseNormal.png",
                                                        "CloseSelected.png",
                                                        this,
                                                        menu_selector(TestScene::menuNetCallback));
    pNetBtn -> setPosition(ccp(240, 160));
    
    // create menu, it's an autorelease object
    CCMenu * pMenu = CCMenu::create(pNetBtn, pCloseItem, NULL);
    pMenu -> setPosition(CCPointZero);
    this -> addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Test Network Func", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);
    
    return true;
}


void TestScene::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void TestScene::menuNetCallback(cocos2d::CCObject * pSender) {
    cocos2d::extension::CCHttpRequest * request = new cocos2d::extension::CCHttpRequest();
//    request->setUrl("http://m.weather.com.cn/data/101180101.html");
//    request->setUrl("http://www.bjtime.cn");
    request->setUrl("http://www.time.ac.cn/timeflash.asp?user=flash");
    request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpGet);
    std::vector<std::string> headers;
//    headers.push_back("Content-Type: application/json; charset=utf-8");
    request->setHeaders(headers);
    const char* postData = "";
    request->setRequestData(postData ,strlen(postData));
    request->setResponseCallback(this, callfuncND_selector(TestScene::onHttpRequestCompleted));
    request->setTag("Post_My_Data");
    cocos2d::extension::CCHttpClient::getInstance()->send(request);
    request->release();
}

void TestScene::onHttpRequestCompleted(cocos2d::CCNode *sender, void *data) {
//    CCMessageBox("TestScene::onHttpRequestCompleted","Alert");
    
    cocos2d::extension::CCHttpResponse * response = (cocos2d::extension::CCHttpResponse*)data;
    if (response == NULL) {
        return;
    }
    if (0 != strlen(response->getHttpRequest()->getTag())) {
        CCLog("%s completed", response->getHttpRequest()->getTag());
    }
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString ,"Http status code:%d ,tag = %s" ,statusCode ,response->getHttpRequest()->getTag());
    CCLog("response code:%d" ,statusCode);
    if (!response->isSucceed())
    {
        CCLog("response failed");
        CCLog("error buffer:%s" ,response->getErrorBuffer());
    }
    std::vector<char> *buffer = response->getResponseData();

    std::stringstream ss;
    
    printf("Http response,dump data:");
    std::string result = "";
    
    string writable_path = CCFileUtils::sharedFileUtils() -> getWritablePath();
    string file_full_path = writable_path + "net_data.xml";
    
//    ofstream out(file_full_path.c_str());
    
//    if (out.is_open()) {
        for (unsigned int i = 0; i < buffer->size(); i ++) {
            //        printf("%c" ,(*buffer)[i]);
            ss << (*buffer)[i];
//            out << (*buffer)[i];
        }
//    }
//out.close();
    
    CCLOG("\n");
    result = ss.str();
    ss.str("");
    
    
//    json_error_t error;
//    json_t * root = json_loads("{\"key\":\"value\"}", 0, &error);
//    if (!root) {
//        CCLOG("error: on line %d: %s\n", error.line, error.text);
//    }
//    ;
//    CCLOG("json_is_object %s", json_is_object(root)?"yes":"no");
//    json_t * inner = json_object_get(root, "key");
//    CCLOG("json_is_string %s", json_is_string(inner)?"yes":"no");
//    CCLOG("inner: %s", json_string_value(inner));
}
