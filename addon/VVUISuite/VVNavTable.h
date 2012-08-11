/*
 *  VVNavTable.h
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __VVNavTable_H__
#define __VVNavTable_H__

#include "cocos2d.h"
#include "VVScrollView.h"
#include "CCSymbol.h"
#include "VVBasicCell.h"
#include "VVTableView.h"

class VVNavTable : public VVTableView, public VVTableViewDelegate, public VVTableViewDataSource
{
public:
	VVNavTable();
	virtual bool initWithViewSizeAndDir(CCSize size, VVScrollViewDirection dir);
	
	virtual void loadItems();

	
	// VVTableViewDataSource delegations
	virtual VVTableViewCell* tableCellAtIndex(VVTableView *table, unsigned int idx );
	virtual unsigned int numberOfCellsInTableView(VVTableView * table);
	virtual void tableCellTouched(VVTableView * table, VVTableViewCell * cell);
	virtual CCSize cellSizeForTable(VVTableView *table);
	
	// VVTableViewDelegate Delegation
	virtual void tableCellRollOverOn(VVTableView * table, VVTableViewCell * cell);	
	virtual void tableCellRollOverOff(VVTableView * table, VVTableViewCell * cell);
	virtual void tableViewDidReachStart(VVTableView * table);
	virtual void tableViewDidReachEnd(VVTableView * table);
	virtual void tableViewDidScroll(VVTableView* table);

	
	CC_SYNTHESIZE(CCSprite*, arrowPlus,PlusArrow);
	CC_SYNTHESIZE(CCSprite*, arrowMinus,MinusArrow);
	CC_SYNTHESIZE(unsigned int, selectedIdx,SelectedIdx);
	
protected:
	vector<unsigned int>idxList;
};

#endif
