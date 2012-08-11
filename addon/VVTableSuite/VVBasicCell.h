/*
 *  VVBasicCell.h
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 8..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#ifndef __VVBasicCell_H__
#define __VVBasicCell_H__

#include "VVTableViewCell.h"


class VVBasicCell: public VVTableViewCell
{
public:
	VVBasicCell();
	virtual bool init();
	~VVBasicCell();
	
	virtual void reset();
	void setLabel(CCNode* aLabel);
	CCNode* getLabel();

	CCLayerColor* getBG();
	
protected:
	CCNode* label;

};



#endif