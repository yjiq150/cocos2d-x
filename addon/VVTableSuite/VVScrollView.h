//
//  SWScrollView.h
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
//  VVScrollView.h
//  CocosX_test
//
//  Created by YoungJae Kwon on 11. 5. 29..
//  Copyright 2011 Beyondi Soft All rights reserved.
//

#ifndef __VVScrollView_H__
#define __VVScrollView_H__

#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

typedef enum {
    VVScrollView_BG,
    VVScrollView_Container,
	VVScrollView_Foreground,
} VVScrollViewLayerLevel;

typedef enum {
    VVScrollViewDirectionHorizontal,
    VVScrollViewDirectionVertical,
    VVScrollViewDirectionBoth
} VVScrollViewDirection;

typedef enum {
    VVScrollViewPagingDirectionHorizontal,
    VVScrollViewPagingDirectionVertical,
    VVScrollViewPagingDirectionNone
} VVScrollViewPagingDirection;

// 페이징이 왼쪽(아래쪽)으로되는지 오른쪽(위쪽)으로 되는지 체크
typedef enum {
    VVScrollViewPagingToMinus,
    VVScrollViewPagingToPlus,
    VVScrollViewPagingToStay	
} VVScrollViewPagingTo;

typedef enum 
{
	VVScrollViewStateIdle,
	VVScrollViewStateSliding,
	VVScrollViewStateZooming
} VVScrollViewScrollState;

class VVScrollView;

class VVScrollViewDelegate
{
public:
	// true를 리턴하면 Deaccelerating을 취소!
	virtual bool scrollViewDeaccelerating(CCPoint scrollDistance) 
	{
		return false;
	}
	virtual void scrollViewDidScroll(VVScrollView* aScrollView) 
	{
		return;
	}
	virtual void scrollViewDidZoom(VVScrollView *aScrollView)
	{
		return;
	}
	virtual void scrollViewDidPageChange(VVScrollView *aScrollView, VVScrollViewPagingTo dir)
	{
		return;
	}
	
};


// 프로퍼티 접근자들이 CC_Synthesize로 되어있으므로 virtual 상속해서 동적바인딩을 사용한다면 주의할것.
// CC_Property는 virtual키워드가 있으나 CC_Synth는 단순inline임
class VVScrollView : public CCLayer
{
	
public:
	virtual bool init();  
	virtual ~VVScrollView();
	virtual void onExit();
	
	void ccTouchesBegan(CCSet* touches, CCEvent* event);
	void ccTouchesMoved(CCSet* touches, CCEvent* event);
	void ccTouchesEnded(CCSet* touches, CCEvent* event);
	void ccTouchesCancelled(CCSet* touches, CCEvent* event);
	
	CC_SYNTHESIZE(float , maxScale, MaxScale);
	CC_SYNTHESIZE(float , minScale, MinScale);
	
	/**
	 * Determines whether this view is enabled for user interaction.
	 * It controlls all table cells too.
	 */
	CC_SYNTHESIZE(bool, isEnabled, IsEnabled);
	/**
	 * scroll view delegate to respond to scroll event
	 delegate 
	 */

	CC_SYNTHESIZE(VVScrollViewDelegate*, delegate, Delegate);

	/**
	 * If YES, the view is being dragged.
	 */
	CC_SYNTHESIZE_READONLY(bool, isDragging, IsDragging);
	/**
	 * Determines whether the scroll view is allowed to bounce or not.
	 */
	CC_SYNTHESIZE(bool, bounces, Bounces);
	/**
	 * direction allowed to scroll. VVScrollViewDirectionBoth by default.
	 */
	CC_SYNTHESIZE(VVScrollViewDirection, direction,Direction);
	
	/**
     * Determines whether it clips its children or not.
     */
	/**
	 * If YES, it clips its children to the visible bounds (view size)
	 * it is YES by default.
	 */
	CC_SYNTHESIZE(bool, clipToBounds, ClipToBounds);
	
	
	/**
     * Container holds scroll view contents
     */
	CC_SYNTHESIZE_RETAIN(CCNode*, container_, Container);
	   
	/**
	 * Returns an autoreleased scroll view object.
	 *
	 * @param size view size
	 * @return autoreleased scroll view object
	 */

	static VVScrollView* scrollViewWithViewSize(CCSize size);
	static VVScrollView* scrollViewWithViewSize(CCSize size, CCNode* container);
	/**
	 * Returns a scroll view object
	 *
	 * @param size view size
	 * @return scroll view object
	 */
	virtual bool initWithViewSize(CCSize size);
	virtual bool initWithViewSize(CCSize size, CCNode *container);
	/**
	 * Sets a new content offset. It ignores max/min offset. It just sets what's given. (just like UIKit's UIScrollView)
	 *
	 * @param offset new offset
	 * @param If YES, the view scrolls to the new offset
	 */
	void setContentOffset(CCPoint offset);
	void setContentOffset(CCPoint offset, bool animated);
	const CCSize& getContentSize();
	void setContentSize(const CCSize& size);
	
	void setViewSize(CCSize size);
	CCSize getViewSize();
	
	CCPoint getContentOffset();
	/**
	 * Returns the current container's minimum offset. You may want this while you animate scrolling by yourself
	 */
	CCPoint minContainerOffset();
	/**
	 * Returns the current container's maximum offset. You may want this while you animate scrolling by yourself
	 */
	CCPoint maxContainerOffset();	
	
	//차일드 추가시  container로 넣기위해 오버라이드
	void addChild(CCNode* node);
	void addChild(CCNode* node, int z);
	void addChild(CCNode* node, int z, int aTag);
	CCNode* getChildByTag(int tag);
	
	// added by YoungJae Kwon
	// vertical방향 스크롤시 좌표계를 UIView와 동일하게 컨버팅해주는 함수 (horizontal 아직 지원안함.)
    void setUIViewOffsetInVerticalDirection(int offsetY);
    void setUIViewOffsetInHorizontalDirection(int offsetX);
	CCPoint getConvertedOffsetInVerticalDirection();
    CCPoint getConvertedOffsetInHorizontalDirection();

	
protected:

	void beforeDraw();
	void afterDraw();

	void relocateContainer(bool animated);
	void deaccelerateScrolling(ccTime dt);
	void stoppedAnimatedScroll(CCNode *node);
	void performedAnimatedScroll(ccTime dt);
	/**
	 * Determiens whether user touch is moved after begin phase.
     */
    bool touchMoved;
    /**
     * max inset point to limit scrolling by touch
     */
    CCPoint maxInset_;
    /**
     * min inset point to limit scrolling by touch
     */
    CCPoint minInset_;
    
    /**
     * scroll speed
     */
    CCPoint scrollDistance;
    /**
     * Touch point
     */
    CCPoint touchPoint;
	
	// Internal state of scrollLayer (scrolling or idle).
	int scrollState;
	
    /**
	 * ScrollView size which is different from contentSize. This size determines visible 
	 * bounding box.
	 */
	/**
     * size to clip. CCNode boundingBox uses contentSize directly.
     * It's semantically different what it actually means to common scroll views.
     * Hence, this scroll view will use a separate size property.
     */
	CCSize viewSize;
	
	 /**
	 * Content offset. Note that left-bottom point is the origin
	 */
	CCPoint contentOffset;
	
	
	
public:
	
	// isPaging과 isDeaccelPaging은 동시에 true가 될 수 없다.
	// 페이지의 시작은 1부터 시작
	
	// 페이징 관련 (한번에 한페이지 이동이 max)
	CC_SYNTHESIZE(bool ,isPagingEnabled,IsPagingEnabled);
	// deaccelerating하다가 가장 근처에있는 페이지로 이동
	CC_SYNTHESIZE(bool ,isDeaccelPagingEnabled,IsDeaccelPagingEnabled);

	// 스크롤뷰의 Inset개념(빈페이지를 채워넣어서 첫 페이지가 스크롤뷰 중앙에서부터 시작하도록)
	CC_SYNTHESIZE(int, numberOfEmptyPages, NumberOfEmptyPages);
	
	CC_PROPERTY(VVScrollViewPagingDirection ,pagingDirection,PagingDirection);
	CC_SYNTHESIZE(int ,minTouchLengthToSlide,MinTouchLengthToSlide);
	CC_SYNTHESIZE(int ,minTouchLengthToChangePage,MinTouchLengthToChangePage);	
	CC_SYNTHESIZE(int ,maxPage,MaxPage);	
	CC_SYNTHESIZE(int ,currentPage,CurrentPage);	
	CC_SYNTHESIZE(CCSize ,pagingSize,PagingSize);	
	
	void movePageTo(int page);
	// 컨텐트 오프셋 <-> page 번호간 전환, page번호는 첫페이지가 1페이지.. 아마도(?)
	virtual CCPoint getContentOffsetFromPage(int page);

	// 줌인/아웃 관련
	void setContentOffset(CCPoint offset, ccTime aniDuration);
	void setZoomScale(float s);
	void setZoomScale(float s, bool animated );
	void setZoomScale(float s, ccTime dt);
	float getZoomScale();
	
	// 배경설정
	void setBackGroundColor(ccColor3B color);
	ccColor3B getBackGroundColor();

	void stopDeaccelrerateScrolling();
	
	
	CC_SYNTHESIZE(CCLayerColor*, background_, Background);
	CC_SYNTHESIZE(CCNode*, foreground_, Foreground);				  
protected:
	void swipeGestureFinder();
	VVScrollViewPagingTo scrAniDirection;
	
	CCMutableArray<CCTouch*> *touchPool;
	float touchLength;
	bool isValidTouch;
	bool isSwipeGesture;
	bool isSwipeRegistered;
	
	// movePageTo가 호출되어서 이동하는 애니메이션이 진행중임을 나타냄
	bool isMovePageToOngoing;

	
	virtual int getClosestPageFromContentOffset(CCPoint offset);
	
	void computeInsets();


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )	
	// 안드로이드용 터치체크 코드
	CCPoint initialTouchLocation;
#endif
	

};

#endif // __VVScrollView_H__