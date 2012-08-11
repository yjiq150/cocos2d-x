/*
 *  VVNavTable.cpp
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "VVNavTable.h"
#include "VVActionSet.h"


USING_NS_CC;

VVNavTable::VVNavTable()
{
	arrowMinus = NULL;
	arrowPlus = NULL;
    selectedIdx = 0;
	//navDelegate = NULL;
}

bool VVNavTable::initWithViewSizeAndDir(CCSize size, VVScrollViewDirection dir)
{
	if ( !VVTableView::initWithViewSize(size,NULL) )
		return false;
	
	setDirection(dir);
	setViewSize(size);
	
	// tableView Delegate
	setDelegate(this); // tableCellTouched를 연결
	setDataSource(this); // 이 클래스에서 cellData를 생성해서 넘겨주기위해 연결
    if (dir == VVScrollViewDirectionVertical)
    {
        setVerticalFillOrder(VVTableViewFillTopDown); // 아래에서 위로채울지, 위에서 아래로 채울지 정렬
        setUIViewOffsetInVerticalDirection(0); // UIView 좌표계로 오프셋값을 설정
    }
    else
    {
        //MOD: BottomUp하나마나 수평스크롤의 경우는 기본값으로 채워도 정상적인 방향이므로 내부 구현이 안되어있음.
        setVerticalFillOrder(VVTableViewFillBottomUp);
        setUIViewOffsetInHorizontalDirection(0); // UIView 좌표계로 오프셋값을 설정
    }
    
	setPosition(ccp(0,0));

		
	return true;
}


void VVNavTable::loadItems()
{

}


#pragma mark -
#pragma mark TableView Delegate
void VVNavTable::tableCellTouched(VVTableView * table, VVTableViewCell * cell)
{
	CCLog("VVNavTable tableCellTouched: OVERIDE ME!!!");
}

void VVNavTable::tableCellRollOverOn(VVTableView * table, VVTableViewCell * cell)
{
	CCLog("VVNavTable tableCellRollOverOn: OVERIDE ME!!!");
}
void VVNavTable::tableCellRollOverOff(VVTableView * table, VVTableViewCell * cell)
{
	CCLog("VVNavTable tableCellRollOverOff: OVERIDE ME!!!");
}
void VVNavTable::tableViewDidReachStart(VVTableView * table)
{
	if (arrowMinus && arrowPlus)
	{
		arrowPlus->setIsVisible(true);
		arrowMinus->setIsVisible(false);
	}
}
void VVNavTable::tableViewDidScroll(VVTableView* table)
{
	if (arrowMinus && arrowPlus)
	{
		arrowPlus->setIsVisible(true);
		arrowMinus->setIsVisible(true);
	}
}
void VVNavTable::tableViewDidReachEnd(VVTableView * table)
{
	if (arrowMinus && arrowPlus)
	{
		arrowPlus->setIsVisible(false);
		arrowMinus->setIsVisible(true);
	}
}


#pragma mark -
#pragma mark TableView DataSource

VVTableViewCell* VVNavTable::tableCellAtIndex(VVTableView *table, unsigned int idx )
{
	CCLog("VVNavTable tableCellAtIndex: OVERIDE ME!!!");
	return NULL;
}

unsigned int VVNavTable::numberOfCellsInTableView(VVTableView * table)
{
    // 검색된 셀의 수가 테이블 뷰보다 작아서 오프셋이 맞지 않는것을 고려한 코드
	int cellheight = cellSizeForTable(table).height;
	int tableHeight = table->getViewSize().height;
	// 테이블 뷰의 크기보다 셀의높이 합이 작으면 테이블 뷰 셀의 갯수를 뷰사이즈만큼 더 크게 부풀려서 리턴
	if( idxList.size() * cellSizeForTable(table).height <  tableHeight)
		return tableHeight / cellheight + 1;
	else
		return idxList.size();
}

CCSize VVNavTable::cellSizeForTable(VVTableView *table)
{
	CCLog("VVNavTable cellSizeForTable set to (width,100) by default: OVERIDE ME!!!");
	return CCSizeMake(table->getViewSize().width, 100);
}



