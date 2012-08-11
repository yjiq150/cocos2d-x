
/*
 *  VVTableView.cpp
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 7..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#include "VVTableView.h"
#include "VVTableViewCell.h"


using namespace cocos2d;

VVTableView* VVTableView::viewWithDataSource( VVTableViewDataSource* dataSource, CCSize size )
{
    return viewWithDataSource(dataSource ,size, NULL);
}

VVTableView* VVTableView::viewWithDataSource( VVTableViewDataSource* dataSource, CCSize size ,CCNode* container )
{
    VVTableView *table = new VVTableView();
	if ( table && table->initWithViewSize(size, container) )
	{
		table->autorelease();		
		table->setDataSource( dataSource );
		table->_updateContentSize();
		return table;
	}
    CC_SAFE_DELETE(table);
	return NULL;
}

bool VVTableView::init()
{
	//호출하면 에러 container가 초기화 되지 않은상태	
	//호출안해도 무관해서 막아놓음
	//	if ( !CCLayer::init() )
	//		return false;
	
	return true;
}


bool VVTableView::initWithViewSize(CCSize size, CCNode* container)
{
	if ( !VVScrollView::initWithViewSize(size,container) )
		return false;
	
	cellsUsed_      = new CCMutableArray<VVTableViewCell*>();
	cellsFreed_     = new CCMutableArray<VVTableViewCell*>();
	//indices_        = new CCMutableIndexSet;
	
	tDelegate_      = NULL;
	vordering_      = VVTableViewFillBottomUp;
	setDirection( VVScrollViewDirectionVertical );
	VVScrollView::setDelegate( this );
	
    return true;
}

VVTableView::VVTableView()
{
	selectedCell = NULL;
}


VVTableView::~VVTableView()
{
	//CC_SAFE_RELEASE(indices_);	
	CC_SAFE_RELEASE(cellsUsed_);
	CC_SAFE_RELEASE(cellsFreed_);	
}

#pragma mark - 
#pragma mark property

void VVTableView::setVerticalFillOrder(VVTableViewVerticalFillOrder fillOrder)
{
    if (vordering_ != fillOrder) 
	{
        vordering_ = fillOrder;
        if (cellsUsed_->count() > 0) 
		{
            reloadData();
        }
    }
}
VVTableViewVerticalFillOrder VVTableView::getVerticalFillOrder()
{
	return vordering_;
}

#pragma mark -
#pragma mark public
void VVTableView::reloadAniDone()
{
	reloadData();
	
	CCEaseExponentialIn* act = CCEaseExponentialIn::actionWithAction(CCFadeOut::actionWithDuration(0.25));
	CCLayerColor* layer = (CCLayerColor*)this->getForeground()->getChildByTag(1000);
	layer->runAction(act);	
}
//주의: 뒷배경이 흰색일 경우에만 사용가능.
// 그냥 포어그라운드에 흰 바탕 컬러 오버레이로 fade in/out 효과주는것
void VVTableView::reloadDataWithAnimation()
{

	CCLayerColor* layer = (CCLayerColor*)this->getForeground()->getChildByTag(1000);
	if ( layer == NULL )
	{
		layer = CCLayerColor::layerWithColor( ccc4(255,255,255,0) );
		this->getForeground()->addChild(layer,9999,1000);
	}
		
	CCEaseExponentialOut* act = CCEaseExponentialOut::actionWithAction(CCFadeIn::actionWithDuration(0.25));
	CCCallFunc* call = CCCallFunc::actionWithTarget(this, callfunc_selector(VVTableView::reloadAniDone));
	CCAction* seq = CCSequence::actions(act,call,NULL);
	layer->runAction(seq);	
}

void VVTableView::reloadData()
{
	// 디엑셀 스크롤링을 중지
	stopDeaccelrerateScrolling();
	
	//NSAutoreleasePool *pool;
	//pool = [NSAutoreleasePool new];
	
	CCMutableArray<VVTableViewCell*>::CCMutableArrayIterator it;
	VVTableViewCell *cell;
	for (it = cellsUsed_->begin(); it != cellsUsed_->end(); it++) 
	{
		cell = *it;
		if ( !cell ) 
			break;
		
		cellsFreed_->addObject(cell);
        cell->reset();
        if (cell->getParent() == container_)
			container_->removeChild(cell,true);
	}
   
	//[indices_ removeAllIndexes];
    cellsUsed_->release();
    cellsUsed_ = new CCMutableArray<VVTableViewCell*>();
    
    _updateContentSize();
	if (dataSource_->numberOfCellsInTableView(this) > 0) {
		this->scrollViewDidScroll(this);
	}
	
	
	// added by YoungJae Kwon
	// reload 할 경우에 focus를 맨 첫줄로 다시 돌려놓음
	if (vordering_ == VVTableViewFillTopDown && direction == VVScrollViewDirectionVertical)
    {
		setUIViewOffsetInVerticalDirection(0);
    }
	else if (direction == VVScrollViewDirectionHorizontal )
	{
        setUIViewOffsetInHorizontalDirection(0);
	}
	
  //[pool drain];
}

VVTableViewCell* VVTableView::cellAtIndex(unsigned int idx)
{
    return _cellWithIndex(idx);
}

void VVTableView::updateCellAtIndex(unsigned int idx)
{
    if (idx == CC_NOT_FOUND || idx > dataSource_->numberOfCellsInTableView(this)-1 ) 
        return;
 
    VVTableViewCell   *cell;
    
//	CCLog("used cellcount:%d ",cellsUsed_->count() );
//	CCLog("free cellcount:%d ",cellsFreed_->count() );
    cell = _cellWithIndex(idx);
    if (cell) 
        _moveCellOutOfSight(cell);
	
    cell = dataSource_->tableCellAtIndex(this, idx);

	// added by YoungJae Kwon
	if (cell == NULL )
		return;
	
	_setIndexForCell(idx, cell);
    _addCellIfNecessary(cell);
}
void VVTableView::insertCellAtIndex( unsigned int idx )
{
    if (idx == CC_NOT_FOUND || idx > dataSource_->numberOfCellsInTableView(this)-1 )
	    return;
    
	VVTableViewCell   *cell;
    int         newIdx;

//  cell = cellsUsed_->objectWithObjectID(idx);
    cell = cellsUsed_->getObjectAtIndex(idx);
    if (cell)
	{		
       // newIdx = [cellsUsed_ indexOfSortedObject:cell];
		newIdx = cell->getIdx();
        for (int i = newIdx; i < cellsUsed_->count() ; i++) 
		{
            cell = cellsUsed_->getObjectAtIndex(i);
            _setIndexForCell(cell->getIdx()+1, cell);
        }
    }
   // [indices_ shiftIndexesStartingAtIndex:idx by:1];
    
    //insert a new cell
    cell = dataSource_->tableCellAtIndex(this,idx);
	// added by YoungJae Kwon 0930
	if (cell == NULL )
		return;
	
	_setIndexForCell(idx, cell);
	_addCellIfNecessary(cell);
    
    _updateContentSize();
}
void VVTableView::removeCellAtIndex(unsigned int idx)
{
    if (idx == CC_NOT_FOUND || idx > dataSource_->numberOfCellsInTableView(this)-1 ) 
        return;
    
    VVTableViewCell   *cell;
    int newIdx;
    
    cell = _cellWithIndex(idx);
    if (!cell) 
        return;
    
	//newIdx = cellsUsed_->indexOfSortedObject(cell);
	newIdx = idx;
    
    //remove first
    _moveCellOutOfSight(cell);
    
    //[indices_ shiftIndexesStartingAtIndex:idx+1 by:-1];
	//MOD: 여기 등호로바꿔야될듯?? -> 등로호 바껏음
    for (int i = cellsUsed_->count()-1; i >= newIdx; i--) 
	{
		cell = cellsUsed_->getObjectAtIndex(i);
		_setIndexForCell(cell->getIdx()-1, cell);
    }
}
VVTableViewCell* VVTableView::dequeueCell()
{
    VVTableViewCell *cell;
    
    if ( cellsFreed_->count() == 0 )
        cell = NULL;
	else 
	{
        cell = cellsFreed_->getObjectAtIndex(0);
		cell->retain();
		cell->autorelease();
        cellsFreed_->removeObjectAtIndex(0);
    }
    return cell;
}
#pragma mark -
#pragma mark private


void VVTableView::_addCellIfNecessary(VVTableViewCell* cell)
{
    if ( cell->getParent() != container_ )
        container_->addChild(cell);
    
	//CCLog("cell add by necessiry,%d",cell->getIdx());
  //  cellsUsed_->insertSortedObject:cell];
	
	if (cellsUsed_->count() == 0) 
	{
		cellsUsed_->addObject(cell);
	}
	else
	{
		VVTableViewCell* lastCell = cellsUsed_->getLastObject();
		// 뒤에 삽입할지 앞에삽입할지 크기보고 결정
		if ( lastCell->getIdx() < cell->getIdx() )
			cellsUsed_->addObject(cell);
		else 
			cellsUsed_->insertObjectAtIndex(cell, 0);
	
	}
   // [indices_ addIndex:cell.idx];
}
void VVTableView::_updateContentSize()
{
    CCSize     size, cellSize;
    unsigned int cellCount;
    
    cellSize  = dataSource_->cellSizeForTable(this);
    cellCount = dataSource_->numberOfCellsInTableView(this);
    
    switch ( getDirection() ) {
        case VVScrollViewDirectionHorizontal:
            size = CCSizeMake(cellCount * cellSize.width, cellSize.height);
            break;
        default:
            size = CCSizeMake(cellSize.width, cellCount * cellSize.height);
            break;
    }
	setContentSize(size);
}
CCPoint VVTableView::_offsetFromIndex(unsigned int index)
{
    CCPoint offset = __offsetFromIndex(index);
    
    const CCSize cellSize = dataSource_->cellSizeForTable(this);
    if (vordering_ == VVTableViewFillTopDown) 
        offset.y = container_->getContentSize().height - offset.y - cellSize.height;
    
    return offset;
}
CCPoint VVTableView::__offsetFromIndex(int index)
{
    CCPoint offset;
    CCSize  cellSize;
    
    cellSize = dataSource_->cellSizeForTable(this);
    switch ( getDirection() ) {
        case VVScrollViewDirectionHorizontal:
            offset = ccp(cellSize.width * index, 0.0f);
            break;
        default:
            offset = ccp(0.0f, cellSize.height * index);
            break;
    }
    
    return offset;
}
unsigned int VVTableView::_indexFromOffset(CCPoint offset)
{
    int index;
    const int maxIdx = dataSource_->numberOfCellsInTableView(this)-1;
    
    const CCSize cellSize = dataSource_->cellSizeForTable(this);
    if (vordering_ == VVTableViewFillTopDown) 
        offset.y = container_->getContentSize().height - offset.y - cellSize.height;
    
    index = MAX(0, __indexFromOffset(offset) );
    index = MIN(index, maxIdx);
    return index;
}
int VVTableView::__indexFromOffset(CCPoint offset) 
{
    int  index;
    CCSize     cellSize;
    
    cellSize = dataSource_->cellSizeForTable(this);
    
    switch ( getDirection() )
	{
        case VVScrollViewDirectionHorizontal:
            index = offset.x / cellSize.width;
            break;
        default:
            index = offset.y / cellSize.height;
            break;
    }
    
    return index;
}
VVTableViewCell* VVTableView::_cellWithIndex(unsigned int cellIndex) 
{
    VVTableViewCell *found;
    found = NULL;
    
    //if ([indices_ containsIndex:cellIndex]) 
      //  found = (VVTableViewCell *)[cellsUsed_ objectWithObjectID:cellIndex];
	
	CCMutableArray<VVTableViewCell*>::CCMutableArrayIterator it;
	VVTableViewCell *cell;
	for (it = cellsUsed_->begin(); it !=  cellsUsed_->end(); it++) 
	{
		cell = *it;
		if (!cell) 
			break;
		
		if( cell->getIdx() == cellIndex)
		{
			found = cell;
			break;
		}
	}
	
    return found;
}

void VVTableView::_moveCellOutOfSight(VVTableViewCell * cell) 
{
	cellsFreed_->addObject(cell);
   // [cellsUsed_ removeSortedObject:cell];
	cellsUsed_->removeObject(cell);
	//CCLog("cell:%d Used->freed",cell->getIdx());

   // [indices_ removeIndex:cell.idx];
    cell->reset();
	
    if (cell->getParent() == container_) 
        container_->removeChild(cell,true);
    
}
		 
void VVTableView::_setIndexForCell(unsigned int index, VVTableViewCell *cell )
{
    cell->setAnchorPoint( ccp(0.0f, 0.0f) );
    cell->setPosition( _offsetFromIndex(index) );
    cell->setIdx( index );
}

#pragma mark -
#pragma mark scrollView

void VVTableView::scrollViewDidScroll(VVScrollView* aView)
{
    unsigned int        startIdx, endIdx, idx, maxIdx;
    CCPoint           offset;
	//NSAutoreleasePool *pool;
    
    maxIdx   = dataSource_->numberOfCellsInTableView(this);
    
    if (maxIdx == 0) 
        return; // early termination
    
	//pool     = [NSAutoreleasePool new];
    offset   = ccpMult( getContentOffset(), -1);
    maxIdx   = MAX(maxIdx - 1, 0);
    
    const CCSize cellSize = dataSource_->cellSizeForTable(this);
    
    if (vordering_ == VVTableViewFillTopDown) 
        offset.y = offset.y + viewSize.height/container_->getScaleY() - cellSize.height;
    
    startIdx = _indexFromOffset(offset);

    if (vordering_ == VVTableViewFillTopDown)
        offset.y -= viewSize.height / container_->getScaleY();
    else 
        offset.y += viewSize.height / container_->getScaleY();
    
    offset.x += viewSize.width / container_->getScaleX();
    
    endIdx   = _indexFromOffset(offset);   
    
    
    if ( cellsUsed_->count() > 0 )
	{
        idx = cellsUsed_->getObjectAtIndex(0)->getIdx();
        while(idx < startIdx) 
		{
            VVTableViewCell *cell = cellsUsed_->getObjectAtIndex(0);
            _moveCellOutOfSight(cell);
            if ( cellsUsed_->count() > 0 ) 
                idx = cellsUsed_->getObjectAtIndex(0)->getIdx();     
            else 
                break;
            
        }
    }
    if ( cellsUsed_->count() > 0 )
	{
        idx = cellsUsed_->getLastObject()->getIdx();
        while(idx <= maxIdx && idx > endIdx) 
		{
            VVTableViewCell *cell = cellsUsed_->getLastObject();
            _moveCellOutOfSight(cell);
            if ( cellsUsed_->count() > 0 ) 
                 idx = cellsUsed_->getLastObject()->getIdx();  
            else 
                break;
            
        }
    }
    
	bool isUpdateRequired;
    for (unsigned int i = startIdx; i <= endIdx; i++) 
	{
		//if ([indices_ containsIndex:i])
  //          continue;
		isUpdateRequired = true;
		// CCMutableArray의 동작특성 : 기본capacity가 4일경우 내부에 오브젝트가 1개밖에 없어도 cellsUsed_->end()는 4번째끝을 가리킨다!
		CCMutableArray<VVTableViewCell*>::CCMutableArrayIterator it;
		VVTableViewCell *cell;
		for (it = cellsUsed_->begin(); it !=  cellsUsed_->end(); it++) 
		{
			cell = *it;
			if (!cell) 
				break;
			
			if( cell->getIdx() == i)
			{
				isUpdateRequired = false;
				break;
			}
		}
		if (isUpdateRequired) 
			updateCellAtIndex(i);
	}
   // [pool drain];
	
	
	// TableView didReachStart, didReachEnd 델리게이션 위한 코드
	if( direction == VVScrollViewDirectionHorizontal)
	{
		if( aView->getContentOffset().x >= 0 )
			tDelegate_->tableViewDidReachStart(this);

		else if ( aView->getContentOffset().x <= -(aView->getContentSize().width - aView->getViewSize().width) )
			tDelegate_->tableViewDidReachEnd(this);
		
		else
			tDelegate_->tableViewDidScroll(this);
	}
	else if ( direction == VVScrollViewDirectionVertical ) 
	{
		if( aView->getContentOffset().y >= 0 )
			tDelegate_->tableViewDidReachEnd(this);
		else if ( aView->getContentOffset().y <= -(aView->getContentSize().height - aView->getViewSize().height) )
			tDelegate_->tableViewDidReachStart(this);
		else 
			tDelegate_->tableViewDidScroll(this);
	}
}

#pragma mark -
#pragma mark Touch events

void VVTableView::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
	 VVScrollView::ccTouchesBegan(touches,event);
	
	if ( !m_bIsVisible)
        return;
	
	//한손가락 터치시 셀 배경 색상 rollover
    if ( touchPool->count() == 1 ) 
	{
        unsigned int        index;
        VVTableViewCell   *cell;
        CCPoint           point;
        
        point = container_->convertTouchToNodeSpace( touchPool->getObjectAtIndex(0) );
        if (vordering_ == VVTableViewFillTopDown) 
		{
            CCSize cellSize = dataSource_->cellSizeForTable(this);
            point.y -= cellSize.height;
        }
        index = _indexFromOffset(point);
        cell  = _cellWithIndex(index);
        
        if (cell) 
		{
			cell->getBG()->setIsVisible(true);
			tDelegate_->tableCellRollOverOn(this, cell);
			selectedCell = cell;
		}
		
	}
	
	
	
}

void VVTableView::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
	if (selectedCell) 
	{
		selectedCell->getBG()->setIsVisible(false);
		tDelegate_->tableCellRollOverOff(this, selectedCell);
		selectedCell = NULL;
	}
	 VVScrollView::ccTouchesMoved(touches,event);
}
void VVTableView::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    if ( !m_bIsVisible)
        return;

	if (selectedCell) 
	{
		selectedCell->getBG()->setIsVisible(false);
		selectedCell = NULL;
	}
	
    if ( touchPool->count() == 1 && !touchMoved) 
	{
        unsigned int        index;
        VVTableViewCell   *cell;
        CCPoint           point;
        
        point = container_->convertTouchToNodeSpace( touchPool->getObjectAtIndex(0) );
        if (vordering_ == VVTableViewFillTopDown) 
		{
            CCSize cellSize = dataSource_->cellSizeForTable(this);
            point.y -= cellSize.height;
        }
        index = _indexFromOffset(point);
        cell  = _cellWithIndex(index);
        
        if (cell) 
			tDelegate_->tableCellTouched(this, cell);
    }
    VVScrollView::ccTouchesEnded(touches,event);
}

