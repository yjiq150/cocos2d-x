/*
 *  VVTableViewCell.cpp
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 7..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#include "VVTableViewCell.h"
VVTableViewCell::VVTableViewCell()
{
}
bool VVTableViewCell::init()
{
    idx = CC_NOT_FOUND;
	
	bg = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 255), 0, 0);
	addChild(bg, 1);
	bg->setIsVisible(false);
	
	wrapper = CCNode::node();
	addChild(wrapper, 2);
	
	return true;
}
VVTableViewCell::~VVTableViewCell()
{
}

//커스텀 셀을 만들때 addChild함수 대신 addChildToCell을 이용해야 reset이 정상작동한다.
void VVTableViewCell::addChildToCell(CCNode* node)
{
	wrapper->addChild(node);
}
CCNode* VVTableViewCell::getChildByTagFromCell(int aTag)
{
	return wrapper->getChildByTag(aTag);
}
void VVTableViewCell::reset()
{
    idx = CC_NOT_FOUND;
	wrapper->removeAllChildrenWithCleanup(true);
}

CCLayerColor* VVTableViewCell::getBG()
{
	return bg;
}
