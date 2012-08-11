/*
 *  VVTargetedTableView.h
 *  Collins_Dictionary_iphone
 *
 *  Created by YoungJae Kwon (yjiq150@gmail.com) on 11/23/11.
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#ifndef __VVTargetedTableView_H__
#define __VVTargetedTableView_H__

#include "VVTableView.h"

class VVTargetedTableView : public VVTableView {
	
	
	CC_SYNTHESIZE(int, m_iTouchPriority, TouchPriority);
	
public:
	
	// initialize
	static VVTargetedTableView* viewWithDataSource( VVTableViewDataSource* dataSource, CCSize size );
	static VVTargetedTableView* viewWithDataSource( VVTableViewDataSource* dataSource, CCSize size ,CCNode* container );
	static VVTargetedTableView* viewWithDataSource( VVTableViewDataSource* dataSource, CCSize size , int nPriority );
	static VVTargetedTableView* viewWithDataSource( VVTableViewDataSource* dataSource, CCSize size ,CCNode* container , int nPriority );
	
	// implement CCTargetedTouchDelegate
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent*event);
	virtual bool ccTargetedTouchesBegan(CCSet* touches, CCEvent*event);
	virtual void ccTargetedTouchesMoved(CCSet *touches, CCEvent *event);
	virtual void ccTargetedTouchesEnded(CCSet *touches, CCEvent *event);
	virtual void ccTargetedTouchesCancelled(CCSet *touches, CCEvent *event);
};

#endif //__VVTargetedTableView_H__