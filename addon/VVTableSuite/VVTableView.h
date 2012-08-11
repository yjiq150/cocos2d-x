//
//  SWTableView.h
//  SWGameLib
//
//
//  Copyright (c) 2010 Sangwoo Im
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//  
//  Created by Sangwoo Im on 6/3/10.
//  Copyright 2010 Sangwoo Im. All rights reserved.



/*
 *  VVTableView.h
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 7..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#ifndef __VVTableView_H__
#define __VVTableView_H__

#include "cocos2d.h"
#include "VVScrollView.h"
#include "VVTableViewCell.h"

using namespace cocos2d;



typedef enum {
    VVTableViewFillTopDown,
    VVTableViewFillBottomUp
} VVTableViewVerticalFillOrder;

class VVTableView;

class VVTableViewDelegate : public VVScrollViewDelegate
{
public:
	// cell내부에서 touch up Inside 일 경우 호출
	virtual void tableCellTouched(VVTableView* table, VVTableViewCell * cell){};

	// cell위에 터치가 올라갈경우 롤오버 효과를 주기위해 호출
	virtual void tableCellRollOverOn(VVTableView* table, VVTableViewCell * cell){};	
	virtual void tableCellRollOverOff(VVTableView* table, VVTableViewCell * cell){};

	// 테이블 뷰가 스크롤되어 경계에 도달했을때 호출
	virtual void tableViewDidReachStart(VVTableView* table){};
	virtual void tableViewDidScroll(VVTableView* table){};
	virtual void tableViewDidReachEnd(VVTableView* table){};
}; 



class VVTableViewDataSource
{
public:
	virtual CCSize cellSizeForTable(VVTableView *table) = 0;
	virtual VVTableViewCell * tableCellAtIndex(VVTableView *table, unsigned int idx) = 0;
	virtual unsigned int numberOfCellsInTableView(VVTableView* table) = 0;
};


// 프로퍼티 접근자들이 CC_Synthesize로 되어있으므로 virtual 상속해서 동적바인딩을 사용한다면 주의할것.
class VVTableView : public VVScrollView, public VVScrollViewDelegate
{
	
public:
	virtual bool init();  
	VVTableView();
	virtual ~VVTableView();
	//virtual void onExit();
	
	// ScrollView delegations
	void ccTouchesBegan(CCSet* touches, CCEvent* event);
	void ccTouchesMoved(CCSet* touches, CCEvent* event);
	void ccTouchesEnded(CCSet* touches, CCEvent* event);
	//void ccTouchesCancelled(CCSet* touches, CCEvent* event);
	void scrollViewDidScroll(VVScrollView* view);
	
	//MOD: 수정할것
//	NSMutableIndexSet *indices_;

    CCMutableArray<VVTableViewCell*> *cellsUsed_;
    CCMutableArray<VVTableViewCell*> *cellsFreed_;
	
	CC_SYNTHESIZE(VVTableViewDataSource*,dataSource_,DataSource);
	CC_SYNTHESIZE(VVTableViewDelegate*,tDelegate_, Delegate);

	CC_PROPERTY(VVTableViewVerticalFillOrder, vordering_, VerticalFillOrder);

	
	// 생성 및 초기화 함수
	static VVTableView* viewWithDataSource( VVTableViewDataSource* dataSource, CCSize size );
	static VVTableView* viewWithDataSource( VVTableViewDataSource* dataSource, CCSize size ,CCNode* container );
	bool initWithViewSize(CCSize size, CCNode* container);
	
	void updateCellAtIndex(unsigned int idx);
	void insertCellAtIndex(unsigned int idx);
	void removeCellAtIndex(unsigned int idx);
	void reloadData();
	void reloadDataWithAnimation();
	
	VVTableViewCell* dequeueCell();
	VVTableViewCell* cellAtIndex(unsigned int idx);
	
	// protected? or Private?
protected:
	int __indexFromOffset(CCPoint offset);
	unsigned int _indexFromOffset(CCPoint offset);
	CCPoint __offsetFromIndex(int index);
	CCPoint _offsetFromIndex(unsigned int index);
	void _updateContentSize();
	
	VVTableViewCell* _cellWithIndex(unsigned int cellIndex);
	void _moveCellOutOfSight(VVTableViewCell* cell);
	void _setIndexForCell(unsigned int index, VVTableViewCell* cell);
	void _addCellIfNecessary(VVTableViewCell* cell);
	
	VVTableViewCell* selectedCell;
	
	void reloadAniDone();
};

#endif