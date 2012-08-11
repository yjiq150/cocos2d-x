/*
 *  VVBasicCell.cpp
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 8..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#include "VVBasicCell.h"
VVBasicCell::VVBasicCell()
{
}
bool VVBasicCell::init()
{
	if ( !VVTableViewCell::init() )
		return false;
	
	label = NULL;
	
	return true;
}

void VVBasicCell::setLabel(CCNode* aLabel)
{
	label = aLabel;
	label->retain();
	addChild(label,5);

}
CCNode* VVBasicCell::getLabel()
{
	return label;
}

CCLayerColor* VVBasicCell::getBG()
{
	return bg;
}

VVBasicCell::~VVBasicCell()
{
	CC_SAFE_RELEASE(label);
	//CCLog("Basiccell died");
}

void VVBasicCell::reset()
{
	VVTableViewCell::reset();
	if (label) 
	{
		label->removeFromParentAndCleanup(true);
		label->release();
		label = NULL;
	}
}