/*
 *  VVTargetedTableView.cpp
 *  Collins_Dictionary_iphone
 *
 *  Created by YoungJae Kwon (yjiq150@gmail.com) on 11/23/11.
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#include "VVTargetedTableView.h"

VVTargetedTableView* VVTargetedTableView::viewWithDataSource( VVTableViewDataSource* dataSource, CCSize size )
{
    return viewWithDataSource(dataSource ,size, NULL, 0);
}

VVTargetedTableView* VVTargetedTableView::viewWithDataSource( VVTableViewDataSource* dataSource, CCSize size ,CCNode* container )
{
    return viewWithDataSource(dataSource, size, container, 0);
}

VVTargetedTableView* VVTargetedTableView::viewWithDataSource( VVTableViewDataSource* dataSource, CCSize size , int nPriority )
{
    return viewWithDataSource(dataSource ,size, NULL, nPriority);
}

VVTargetedTableView* VVTargetedTableView::viewWithDataSource( VVTableViewDataSource* dataSource, CCSize size , CCNode* container, int nPriority )
{
    VVTargetedTableView *table = new VVTargetedTableView();
	table->setTouchPriority(nPriority);
	if ( table && table->initWithViewSize(size, container) )
	{
		table->autorelease();		
		table->setDataSource( dataSource );
		table->_updateContentSize();
		return table;
	}
    CC_SAFE_DELETE(table);
	return table;
}

#pragma mark -
#pragma mark implement CCTargetedTouchDelegate

void VVTargetedTableView::registerWithTouchDispatcher()
{
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,m_iTouchPriority, true);
}

// CCLayer의 CCAssert구문 회피용 더미함수
bool VVTargetedTableView::ccTouchBegan(CCTouch* touch, CCEvent*event)
{
	return false;
}

// targeted multitouch를 입력받아서 다른 노드로 터치 전파를 막은 후에
// 터치가 전달되길 원하는 tableView의 터치함수를 직접 실행한다.
bool VVTargetedTableView::ccTargetedTouchesBegan(CCSet* touches, CCEvent*event)
{	
	CCTouch* touch = (CCTouch*)touches->anyObject();
	CCRect rect = CCRectMake(0, 0,this->getViewSize().width,this->getViewSize().height);
	CCPoint pointInTable = this->convertTouchToNodeSpace(touch);
	
	if ( CCRect::CCRectContainsPoint(rect, pointInTable) )
	{
		this->ccTouchesBegan(touches, event);
		return true;
	}
	else 
		return false;	
}

void VVTargetedTableView::ccTargetedTouchesMoved(CCSet *touches, CCEvent *event)
{
	this->ccTouchesMoved(touches, event);
}
void VVTargetedTableView::ccTargetedTouchesEnded(CCSet *touches, CCEvent *event)
{
	this->ccTouchesEnded(touches, event);
}
void VVTargetedTableView::ccTargetedTouchesCancelled(CCSet *touches, CCEvent *event) 
{
	this->ccTouchesCancelled(touches, event);
}
