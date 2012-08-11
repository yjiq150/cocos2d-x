//
//  SWTableViewCell.h
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
//

/*
 *  VVTableViewCell.h
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 7..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */
#ifndef __VVTableViewCell_H__
#define __VVTableViewCell_H__

#include "cocos2d.h"

#define CC_NOT_FOUND 5000000 // 셀이 5천만이 넘지는 않을거라 가정 ( unsigned int의 맥시멈값으로 고쳐잡아도 무관)


class VVTableViewCell: public CCNode
{
public:
	VVTableViewCell();
	virtual bool init();
	~VVTableViewCell();
	
	//셀을 재활용하기 위해서 필요. 객체는 그대로 유지하고 내용만 reset된다.
	virtual void reset();

	virtual CCLayerColor* getBG();
	void addChildToCell(CCNode* node);
	CCNode* getChildByTagFromCell(int aTag);
protected:	
	CC_SYNTHESIZE(unsigned int,idx,Idx);
	CCLayerColor* bg;

	//reset할때 임의 추가노드 한번에 삭제 위해 필요
	CCNode* wrapper;
};



#endif