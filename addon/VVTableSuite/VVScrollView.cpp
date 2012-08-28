//
//  VVScrollView.cpp
//  CocosX_test
//
//  Created by YoungJae Kwon on 11. 5. 29..
//  Copyright 2011 __MyCompanyName_. All rights reserved.
//

#include "VVScrollView.h"
#include "CCGeometry.h"
#include "CCNode.h"
#include "CCEGLView.h"

#define SCROLL_DEACCEL_RATE  0.95f
#define SCROLL_DEACCEL_DIST  1.0f
#define BOUNCE_DURATION      0.35f
#define INSET_RATIO          0.7f

#define PAGING_CENTER_RATIO 0.4f // 페이징 할때 뷰 크기의 40 퍼센트 이상 넘어가면 다음페이지로

#define MAX_TOTAL_SCREEN 600000
// 주의사항!!!!!!!!
// Added by Kwon YoungJae 
// clipping코드를 제외한 부분은
// 레티나 디스플레이용으로 수정되어있지 않음! , 
// 따라서 아이폰4용으로 작성시 inset, 스크롤 속도 등을 CC_CONTENT_SCALE_FACTOR()를 사용해서 처리해줄것

using namespace cocos2d;

VVScrollView* VVScrollView::scrollViewWithViewSize(CCSize size)
{
	VVScrollView* scrView = new VVScrollView();
	if (scrView && scrView->initWithViewSize(size))
	{
		scrView->autorelease();
		return scrView;
	}
	CC_SAFE_DELETE(scrView);
	return NULL;
}
VVScrollView* VVScrollView::scrollViewWithViewSize(CCSize size, CCNode* container)
{
	VVScrollView* scrView = new VVScrollView();
	if (scrView && scrView->initWithViewSize(size,container))
	{
		scrView->autorelease();
		return scrView;
	}
	CC_SAFE_DELETE(scrView);
	return NULL;
}
bool VVScrollView::init()
{
// 호출하면 에러 container_가 초기화 되지 않은상태	
// 호출안해도 무관해서 막아놓음
//	if ( !CCLayer::init() )
//		return false;
	
	container_ = NULL;
	
	return true;
}

bool VVScrollView::initWithViewSize(CCSize size)
{
    return initWithViewSize(size, NULL);
}

bool VVScrollView::initWithViewSize(CCSize size, CCNode *container)
{
	if ( !VVScrollView::init() )
		return false;
	
	// 외부에서 컨테이너 갖고 셋업했을경우
	if (container != NULL) 
		setContainer(container_);// isRelativeAnchor = NO
	else 	// 외부컨테이너가 없을경우 생성
		setContainer( CCLayer::node() );

	viewSize = size;
	
	setIsTouchEnabled(true);
	touchPool = new CCMutableArray<CCTouch*>();
	
	// VVScrollView의 디폴트 셋팅
	isPagingEnabled		= false;
	isDeaccelPagingEnabled = false;
	numberOfEmptyPages = 0;
	
	pagingDirection = VVScrollViewPagingDirectionNone;
	minTouchLengthToSlide = 1;
	minTouchLengthToChangePage = 100;
	currentPage = 1;
	maxPage = MAX_TOTAL_SCREEN; // 임시로 값 셋팅 정확히 할려면 직접 설정할것
	pagingSize = size;
	
	
	isEnabled           = true;
	delegate            = NULL;
	bounces             = true;
	clipToBounds        = true;
	minScale = 1;
	maxScale = 1;
	
	isMovePageToOngoing = false;
	
	direction = VVScrollViewDirectionBoth;
	
	
	container_->setContentSize( CCSizeZero );
	container_->setPosition( ccp(0.0f, 0.0f) );
	addChild(container_, VVScrollView_Container);

	// 움직이지 않는 fixed형 배경
	// Default값은 invisible;
	background_ = new CCLayerColor();
	background_->initWithColorWidthHeight(ccc4(255, 255, 255, 255),size.width,size.height);
	background_->setIsVisible(false);
	addChild(background_ ,VVScrollView_BG); // -1 : 맨아래에 삽입
	background_->release();
	
	// 추후 테이블 위에 fixed형 무언가를 배치하고 싶을경우 getForeground를 해서 추가
	foreground_ = CCNode::node();
	addChild(foreground_ ,VVScrollView_Foreground); // -1 : 맨아래에 삽입

	
    return true;
}
/* 아직 포팅 안함
-(BOOL)isNodeVisible:(CCNode *)node {
    const CCPoint offset = [self contentOffset];
    const CCSize  size   = [self viewSize];
    const float   scale  = [self zoomScale];
    
    CCRect viewRect;
    
    viewRect = CCRectMake(-offset.x/scale, -offset.y/scale, size.width/scale, size.height/scale); 
    
    return CCRectIntersectsRect(viewRect, [node boundingBox]);
}
 
-(void)pause:(id)sender {
    id child;
    [container_ pauseSchedulerAndActions];
    CCARRAY_FOREACH(container_.children, child) {
        if ([child respondsToSelector:@selector(pause:)]) {
            [child performSelector:@selector(pause:) withObject:sender];
        }
    }
}
-(void)resume:(id)sender {
    id child;
    CCARRAY_FOREACH(container_.children, child) {
        if ([child respondsToSelector:@selector(resume:)]) {
            [child performSelector:@selector(resume:) withObject:sender];
        }
    }
    [container_ resumeSchedulerAndActions];
}

-(void)setIsTouchEnabled:(BOOL)e {
    [super setIsTouchEnabled:e];
    if (!e) {
        isDragging_ = NO;
        touchMoved_ = NO;
        [touches_ removeAllObjects];
    }
}
*/

VVScrollViewPagingDirection VVScrollView::getPagingDirection()
{
	return pagingDirection;
}

// 1. PagingDirection을 정하기 전에 setContentSize가 먼저 호출되거나
// 2. PagingDirection을 정한 후 setContentSize를 호출해주어야 한다.
void VVScrollView::setPagingDirection(VVScrollViewPagingDirection aDir)
{
	pagingDirection = aDir;
	if (pagingDirection == VVScrollViewPagingDirectionHorizontal) 
	{
		minTouchLengthToChangePage = viewSize.width*PAGING_CENTER_RATIO;
		maxPage = ceil( (getContentSize().width)/pagingSize.width);
	}
	else if(pagingDirection == VVScrollViewPagingDirectionVertical) 
	{
		minTouchLengthToChangePage = viewSize.height*PAGING_CENTER_RATIO;
		maxPage = ceil( (getContentSize().height )/pagingSize.height);

	}
	else
	{
		minTouchLengthToChangePage = 0;
	}
	
}
// 스크롤뷰 배경 색상 설정
void VVScrollView::setBackGroundColor(ccColor3B color)
{
	// 안보이는 상태에서 색상설정할경우 visivility reset
	if( background_->getIsVisible() == false )
	{
		background_->setIsVisible(true);
	}
	background_->setColor(color);
}
ccColor3B VVScrollView::getBackGroundColor()
{
	return background_->getColor();
}


void VVScrollView::setContentOffset(CCPoint offset)
{
	setContentOffset(offset, false);
}

void VVScrollView::setContentOffset(CCPoint offset, bool animated)
{
    if (animated) //animate scrolling
	{ 
		setContentOffset(offset, BOUNCE_DURATION);
    } 
	else 
	{ 
		if (!bounces) 
		{
            const CCPoint minOffset = minContainerOffset();
            const CCPoint maxOffset = maxContainerOffset();
            
            offset.x = MAX(minOffset.x, MIN(maxOffset.x, offset.x));
            offset.y = MAX(minOffset.y, MIN(maxOffset.y, offset.y));
        }
		//set the container_ position directly
        container_->setPosition(offset);
		if (delegate) {
			delegate->scrollViewDidScroll(this);
		}
		
		// added by YoungJae Kwon
		// 액셀러레이팅 스크롤 해제
		//stoppedAnimatedScroll(container_);
    }
}
void VVScrollView::stopDeaccelrerateScrolling()
{
	this->unschedule(schedule_selector(VVScrollView::deaccelerateScrolling));
}
void VVScrollView::setContentOffset(CCPoint offset, ccTime aniDuration) 
{
    CCFiniteTimeAction *scroll, *expire;
    scroll = CCEaseExponentialOut::actionWithAction( CCMoveTo::actionWithDuration(aniDuration, offset) );
	expire = CCCallFunc::actionWithTarget( this, callfunc_selector(VVScrollView::stoppedAnimatedScroll) );
	container_->runAction(CCSequence::actions(scroll,expire,NULL));
	this->schedule(schedule_selector(VVScrollView::performedAnimatedScroll));
}

// vertical방향 스크롤시 좌표계를 UIView와 동일하게 컨버팅해주는 함수
void VVScrollView::setUIViewOffsetInVerticalDirection(int offsetY)
{
	setContentOffset(ccp(container_->getPosition().x , offsetY - this->getContentSize().height* getZoomScale() + viewSize.height), false);
}

void VVScrollView::setUIViewOffsetInHorizontalDirection(int offsetX)
{
	setContentOffset(ccp(offsetX , container_->getPosition().y), false);
}

CCPoint VVScrollView::getConvertedOffsetInVerticalDirection()
{
	// 좌표변환공식: 컨텐츠 높이 - 현재오프셋 - viewSize 높이
	return ccp(container_->getPosition().x , this->getContentSize().height* getZoomScale() + container_->getPosition().y  - viewSize.height);
}

CCPoint VVScrollView::getConvertedOffsetInHorizontalDirection()
{
    // 좌표 변환 필요 없음
	return container_->getPosition();
}

CCPoint VVScrollView::getContentOffset()
{
    return container_->getPosition();
}

void VVScrollView::setZoomScale(float s)
{
    if (container_->getScale() != s) 
	{
        CCPoint oldCenter, newCenter;
        CCPoint center;
        
        if (touchLength == 0.0f) 
		{
            center = ccp(viewSize.width*0.5f, viewSize.height*0.5f);
            center = convertToWorldSpace(center);
        } 
		else 
		{
            center = touchPoint;
        }
        
        oldCenter = container_->convertToNodeSpace(center);
        container_->setScale( MAX(minScale, MIN(maxScale, s)) );
        newCenter = container_->convertToWorldSpace(oldCenter);
        
        const CCPoint offset = ccpSub(center, newCenter);
        delegate->scrollViewDidZoom(this);
		
		computeInsets();
        setContentOffset(ccpAdd(container_->getPosition(),offset));
    }
}

float VVScrollView::getZoomScale() {
    return container_->getScale();
}
void VVScrollView::setZoomScale(float s, bool animated )
{
    if (animated) 
        setZoomScale(s, BOUNCE_DURATION);
    else 
        setZoomScale(s);
    
}
void VVScrollView::setZoomScale(float s, ccTime dt)
{
	/* MOD: 스케일을 액션으로바꾸는법? 더 부드러울려나... 잘모르겠음 나중에 잘 동작하면 삭제할것
    if (dt > 0) {
        if (container_->getScale() != s) 
		{
            CCActionTween *scaleAction;
            scaleAction = CCActionTween::actionWithDuration(dt,"scale",container_->getScale(), s);
            this->runAction(scaleAction);
			//container_->setScale(s);
        }
    } 
	else 
     */
    setZoomScale(s);
}


void VVScrollView::setViewSize(CCSize size)
{
    if (!CCSize::CCSizeEqualToSize(viewSize, size))
	{
        viewSize = size;
		background_->setContentSize(size);
        computeInsets();
    }
}
CCSize VVScrollView::getViewSize()
{
	return viewSize;
}

void VVScrollView::computeInsets()
{
	maxInset_ = maxContainerOffset();
	maxInset_ = ccp(maxInset_.x + viewSize.width * INSET_RATIO,
					maxInset_.y + viewSize.height * INSET_RATIO);
	minInset_ = minContainerOffset();
	minInset_ = ccp(minInset_.x - viewSize.width * INSET_RATIO,
					minInset_.y - viewSize.height * INSET_RATIO);
}

void VVScrollView::relocateContainer(bool animated)
{
    CCPoint oldPoint, min, max;
    float newX, newY;
    
    min = minContainerOffset();
    max = maxContainerOffset();
    
    oldPoint = container_->getPosition();
	newX     = oldPoint.x;
    newY     = oldPoint.y;
    if (direction == VVScrollViewDirectionBoth || direction == VVScrollViewDirectionHorizontal) {
        newX     = MIN(newX, max.x);
        newX     = MAX(newX, min.x);
    }
    if (direction == VVScrollViewDirectionBoth || direction == VVScrollViewDirectionVertical) {
        newY     = MIN(newY, max.y);
        newY     = MAX(newY, min.y);
    }
	
    if (newY != oldPoint.y || newX != oldPoint.x) 
	{
		setContentOffset(ccp(newX,newY), animated);
    }
}
CCPoint VVScrollView::maxContainerOffset()
{
    return ccp(0.0f, 0.0f);
}
CCPoint VVScrollView::minContainerOffset()
{
	
	float minX = viewSize.width - container_->getContentSize().width*container_->getScaleX();
	float minY = viewSize.height - container_->getContentSize().height*container_->getScaleY();
	// Modified by YoungJae Kwon
	// 컨테이너안의 총 객체들크기(컨텐츠사이즈)가 viewSize보다 작을경우 min값이 양수가 나오는데 이것을 다 0으로 줄여버릴것
	// 그렇지않으면 바운더리 체크가 안된다.
	if( minX > 0 )
		minX = 0;
	if (minY > 0)
		minY = 0;
	
    return ccp(minX,minY);

}


void VVScrollView::deaccelerateScrolling(ccTime dt)
{
    if (isDragging)
	{
		this->unschedule(schedule_selector(VVScrollView::deaccelerateScrolling));
        return;
    }
    
    float newX, newY;
    CCPoint maxInset, minInset;
    
    container_->setPosition( ccpAdd(container_->getPosition(), scrollDistance) );
    
    if (bounces) 
	{
        maxInset = maxInset_;
		minInset = minInset_;
    }
	else 
	{
        maxInset = maxContainerOffset();
        minInset = minContainerOffset();
    }
    
    //check to see if offset lies within the inset bounds
    newX     = MIN(container_->getPosition().x, maxInset.x);
    newX     = MAX(newX, minInset.x);
    newY     = MIN(container_->getPosition().y, maxInset.y);
    newY     = MAX(newY, minInset.y);

	// added by YoungJae Kwon
	// 바운더리 바깥으로 벗어났을때는 스크롤 속도 급감하게 계수 0.5 곱해줌
	
// 스크롤 바운더리보는  디버깅 코드	
//	bool a1 =  newY > maxContainerOffset().y;
//	bool a2 =  newY < minContainerOffset().y;
//	bool a3 =  newX > maxContainerOffset().x;
//	bool a4 =  newX < minContainerOffset().x;
//	
//	CCLog("%d,%d,%d,%d",a1,a2,a3,a4);
//	
	if (newY > maxContainerOffset().y || newY < minContainerOffset().y ||
		newX > maxContainerOffset().x || newX < minContainerOffset().x)
	{
		scrollDistance = ccpSub(scrollDistance, ccp(newX - container_->getPosition().x, newY - container_->getPosition().y));
		scrollDistance = ccpMult(scrollDistance, SCROLL_DEACCEL_RATE*0.5);

	}
	else {
		scrollDistance = ccpSub(scrollDistance, ccp(newX - container_->getPosition().x, newY - container_->getPosition().y));
		scrollDistance = ccpMult(scrollDistance, SCROLL_DEACCEL_RATE);

	}
	// 스크롤되는 디스턴스 거리정보를 넘겨줌
	if ( delegate && delegate->scrollViewDeaccelerating(scrollDistance) )
	{
		this->unschedule(schedule_selector(VVScrollView::deaccelerateScrolling));
		return;
	}

    setContentOffset( ccp(newX,newY) );

	
	// 디엑셀 정지시키는 조건 두가지
	//1.키네틱 스크롤링이 거리가 1이하로 줄어들경우
	//2.범위가 바운더리 바깥으로 벗어날경우     
	if (ccpLengthSQ(scrollDistance) <= SCROLL_DEACCEL_DIST*SCROLL_DEACCEL_DIST ||
        newX== maxInset.x || newX == minInset.x ||
        newY == maxInset.y || newY == minInset.y)
	{

		this->unschedule(schedule_selector(VVScrollView::deaccelerateScrolling));
		
		if (isDeaccelPagingEnabled)
		{
			movePageTo( getClosestPageFromContentOffset( getContentOffset() ) );
		}
		else 
		{
			relocateContainer(true);
		}

		
    }
	
	
}

void VVScrollView::stoppedAnimatedScroll(CCNode *node)
{
	this->unschedule(schedule_selector(VVScrollView::performedAnimatedScroll));
	
	// added by YougnJae Kwon
	if( (isPagingEnabled || isDeaccelPagingEnabled) )
	{
		//MOD: 페이징 오동작시 체크!
		// NavDays의 month이동시 페이지가 동일해서 업데이트 안되는것 수정위함
		//if(scrAniDirection != VVScrollViewPagingToStay) 
		{
			CCLOG("ScrollView::stoppedAnimatedScroll, page changed");
			if (delegate) {
				delegate->scrollViewDidPageChange(this,scrAniDirection);
			}
			isMovePageToOngoing = false;
		}
	}
	
}

void VVScrollView::performedAnimatedScroll(ccTime dt) 
{
    if (isDragging)
	{
        this->unschedule(schedule_selector(VVScrollView::performedAnimatedScroll));
		// 페이징 애니메이션 도중 사용자의 드래깅에 의해 중간에 끊어먹는 경우
		// added by YougnJae Kwon
		//if( scrAniDirection != VVScrollViewPagingToStay)
		{
			CCLOG("ScrollView::performedAnimatedScroll, drag & page changed");
			if (delegate) {
				delegate->scrollViewDidPageChange(this,scrAniDirection);
			}
			isMovePageToOngoing = false;
		}
        return;
    }
	if (delegate) {
		delegate->scrollViewDidScroll(this);
	}
}


const CCSize& VVScrollView::getContentSize()
{
	return container_->getContentSize();
}

void VVScrollView::setContentSize(const CCSize& size)
{
	if (isPagingEnabled || isDeaccelPagingEnabled)
	{
		if (pagingDirection == VVScrollViewPagingDirectionHorizontal) 
		{
			maxPage = ceil( (size.width)/pagingSize.width );
		}
		else if(pagingDirection == VVScrollViewPagingDirectionVertical) 
		{
			maxPage = ceil( (size.height)/pagingSize.height);
		}
		
	}
	container_->setContentSize(size);
	computeInsets();
}
    
/**
 * make sure all children go to the container_
 */
void VVScrollView::addChild(CCNode* node)
{
	CCLayer::addChild(node);
}
void VVScrollView::addChild(CCNode* node, int z)
{
    CCLayer::addChild(node, z);
}
void VVScrollView::addChild(CCNode* node, int z, int aTag)
{
	// Modified By Kwon Young Jae
	// 앵커포인트를 강제 지정하는 부분 삭제해버림
	//  node.isRelativeAnchorPoint = YES;
	//  node.anchorPoint           = ccp(0.0f, 0.0f);

	if (node == container_ || node == background_ || node == foreground_)
	{
		CCLayer::addChild(node, z, aTag);
	}
	else	
		container_->addChild(node, z, aTag);
}

// Added By Kwon Young Jae
// addchild를 오버라이드 해서 컨테이너에 넣도록 했으면 getchild도 오버라이드가 필요

CCNode* VVScrollView::getChildByTag(int tag)
{
	return container_->getChildByTag(tag);
}
/**
 * clip this view so that outside of the visible bounds can be hidden.
 */
void VVScrollView::beforeDraw()
{
    if (clipToBounds) 
	{


		
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		//	클리핑 플레인 사용하는 코드 : OPENGL_ES 저버전 호환성을 위해 코드 주석 유지
		//MOD: iOS에서는 이게 더 잘먹는듯?
        GLfloat planeTop[]    = {0.0f, -1.0f, 0.0f, viewSize.height*CC_CONTENT_SCALE_FACTOR()};
        GLfloat planeBottom[] = {0.0f, 1.0f, 0.0f, 0.0f};
        GLfloat planeLeft[]   = {1.0f, 0.0f, 0.0f, 0.0f};
        GLfloat planeRight[]  = {-1.0f, 0.0f, 0.0f, viewSize.width*CC_CONTENT_SCALE_FACTOR()};
        
        glClipPlanef(GL_CLIP_PLANE0, planeTop);
        glClipPlanef(GL_CLIP_PLANE1, planeBottom);
		glClipPlanef(GL_CLIP_PLANE2, planeLeft);
        glClipPlanef(GL_CLIP_PLANE3, planeRight);
        glEnable(GL_CLIP_PLANE0);
        glEnable(GL_CLIP_PLANE1);
        glEnable(GL_CLIP_PLANE2);
        glEnable(GL_CLIP_PLANE3);
		
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	

        // android에서 GL_CLIP_PLANE이 동시에 1개이상 사용이 잘 안되는듯 싶다. 따라서 glScissor를 사용할것.
		glEnable(GL_SCISSOR_TEST);
        
        // 안드로이드는 항상 1이라 필요없다.
		//const CGFloat s = CC_CONTENT_SCALE_FACTOR();
       
        // 대신 view->create(w,h); 함수를 위해 스크린 스케일이 및 원점조정 필요
        const CGFloat s =  CCEGLView::sharedOpenGLView().getScreenScaleFactor();
        const CCPoint ori = CCEGLView::sharedOpenGLView().getViewPort().origin;

        
		// glScissor는 스크린기준으로 잘라내기 때문에 포인트를 화면좌표계로 변경해야한다
		CCPoint screenPos = this->getParent()->convertToWorldSpace( m_tPosition );
		glScissor( ori.x + screenPos.x*s, ori.y + screenPos.y*s, viewSize.width*s, viewSize.height*s);
    
		 
#endif 

	

    }
}
/**
 * retract what's done in beforeDraw so that there's no side effect to
 * other nodes.
 */
void VVScrollView::afterDraw()
{
    if (clipToBounds) 
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		//	클리핑 플레인 사용하는 코드 : OPENGL_ES 저버전 호환성을 위해 코드 주석 유지		
		glDisable(GL_CLIP_PLANE0);
		glDisable(GL_CLIP_PLANE1);
		glDisable(GL_CLIP_PLANE2);
		glDisable(GL_CLIP_PLANE3);
		
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	
		glDisable(GL_SCISSOR_TEST);
#endif 
				
    }
}

void VVScrollView::ccTouchesBegan(CCSet* touches, CCEvent* event)
{	
	if (!m_bIsVisible || !isEnabled || isMovePageToOngoing) 
		return;

	// 스와이프 제스쳐 초기값은 무조것 false
	isSwipeRegistered = false;
	isSwipeGesture = false;
	// 기본적으로 터치의 유효성은 false로 셋팅 후 제대로된 터치일경우 true로 전환
	isValidTouch = false;
	
    CCRect frame;
    frame = CCRectMake(this->getPosition().x, this->getPosition().y, viewSize.width, viewSize.height);
    //dispatcher does not know about clipping. reject touches outside visible bounds.
	// 이미 터치중인 손가락 + 새로 눌러진 손가락의 최대 수는 2
	if (touches->count() + touchPool->count() > 2  )
	{
		isValidTouch = false;
		return;
	}
	
	CCSetIterator it;
    for( it = touches->begin(); it != touches->end(); it++)
    {
		//MOD: 원래코드 이상해서 부모에서 터치좌표계변환하는걸로 수정됨
//		if (!CCRect::CCRectContainsPoint(frame, convertToWorldSpaceAR( convertTouchToNodeSpaceAR((CCTouch*)*it))) )
		if (!CCRect::CCRectContainsPoint(frame, getParent()->convertTouchToNodeSpace((CCTouch*)*it)) )
		{
            touchPoint = ccp(-1.0f, -1.0f); 
            isDragging = false;
			isValidTouch = false;
            return;
        }
		//유효터치를 새로운 터치풀에 저장
		touchPool->addObject((CCTouch*)*it);
    }
	

	// touch의 유효성 검사가 끝난 후 validation을 셋팅
	isValidTouch = true;
    if ( touchPool->count() == 1) // scrolling
	{ 
		touchPoint = convertTouchToNodeSpace((CCTouch*)touches->anyObject());
		touchMoved     = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )	
		CCTouch *touch = (CCTouch*)touches->anyObject();
		initialTouchLocation = touch->locationInView(); // 안드로이드용 터치무브 체크
#endif
		
		
		isDragging     = true; //dragging started
		scrollDistance = ccp(0,0);
		touchLength = 0;
		scrollState = VVScrollViewStateIdle;
    } 
	else if (touchPool->count() == 2)
	{ 
		CCTouch *t1 = touchPool->getObjectAtIndex(0);
		CCTouch *t2 = touchPool->getObjectAtIndex(1);
		touchPoint = ccpMidpoint(convertTouchToNodeSpace(t1),convertTouchToNodeSpace( t2 ));
		touchLength = ccpDistance(container_->convertTouchToNodeSpace(t1), container_->convertTouchToNodeSpace(t2));
		isDragging  = true;
		scrollState = VVScrollViewStateZooming;
    } 
	else if (touchPool->count() == 0)
	{
		isValidTouch = false;
	}

}

void VVScrollView::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
	if (!m_bIsVisible || !isEnabled || !isValidTouch || isMovePageToOngoing || touchPool->count() == 0) {
		return;
	}
    
	
	CCTouch *touch = (CCTouch*)touches->anyObject();
	CCPoint touchLocation = touch->locationInView();
	CCPoint prevLocation = touch->previousLocationInView();
	
	// 터치수에 관계없이 드래그로 스크롤링
	if (isDragging)
	{
        // 페이징이 켜져있을경우 키네틱 스크롤 사용 안함
		if (isPagingEnabled) 
		{
			CCPoint newTouchPoint = convertTouchToNodeSpace( touchPool->getObjectAtIndex(0) );
			// If finger is dragged for more distance then minimum - start sliding and cancel pressed buttons.
			// Of course only if we not already in sliding mode
			if ( (scrollState != VVScrollViewStateSliding) 
				&& (ccpDistance(touchPoint,newTouchPoint) >= minTouchLengthToSlide) )
			{
				scrollState = VVScrollViewStateSliding;				
				touchPoint = newTouchPoint;	// Avoid jerk after state change.
			}
			
			
			if (scrollState == VVScrollViewStateSliding)
			{
				CCPoint newOffset;
				if( pagingDirection == VVScrollViewPagingDirectionHorizontal )
					newOffset = ccp( (-(currentPage-1)*pagingSize.width)+(newTouchPoint.x-touchPoint.x) , getContentOffset().y);
				else if ( pagingDirection == VVScrollViewPagingDirectionVertical )
					newOffset = ccp( getContentOffset().x, -(getContentSize().height - getViewSize().height)+((currentPage-1)*pagingSize.height)+(newTouchPoint.y-touchPoint.y) );
				else
					CCAssert(0,"If Paging is enabled, then setPageDirection ");
				
				setContentOffset( newOffset ) ;
			}
			
			// 0.3초안에 빠르게 페이지 넘기는 방법 
			if ( !isSwipeRegistered) 
			{
				isSwipeRegistered = true;
				isSwipeGesture = true;
				this->schedule( schedule_selector(VVScrollView::swipeGestureFinder),0.3 );
			}
			
		}
		else 
		{
			
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
				touchMoved = true;			
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )	
            float maxMoveSensitivity = 5;
			if (touchLocation.x == initialTouchLocation.x && touchLocation.y == initialTouchLocation.y)
				touchMoved = false;
            else if(touchLocation.x <= initialTouchLocation.x + maxMoveSensitivity &&
                    touchLocation.x >= initialTouchLocation.x - maxMoveSensitivity &&
                    touchLocation.y <= initialTouchLocation.y + maxMoveSensitivity &&
                    touchLocation.y >= initialTouchLocation.y - maxMoveSensitivity)
            {
                touchMoved = false;
            }
			else
				touchMoved = true;
            
#endif
			
			CCPoint moveDistance, newPoint, maxInset, minInset;
			CCRect  frame;
			float newX, newY;
			
			frame        = CCRectMake(m_tPosition.x, m_tPosition.y, viewSize.width, viewSize.height);
			newPoint     = convertTouchToNodeSpace( touchPool->getObjectAtIndex(0) );
			moveDistance = ccpSub(newPoint, touchPoint);
			touchPoint  = newPoint;
			
			CCPoint touchPointInTermsOfParent = this->getParent()->convertTouchToNodeSpace( touchPool->getObjectAtIndex(0) );
			//if ( CCRect::CCRectContainsPoint(frame, convertToWorldSpace(newPoint)) ) 
			if ( CCRect::CCRectContainsPoint(frame, touchPointInTermsOfParent) ) 			
			{
				switch (direction) 
				{
					case VVScrollViewDirectionVertical:
						moveDistance = ccp(0.0f, moveDistance.y);
						break;
					case VVScrollViewDirectionHorizontal:
						moveDistance = ccp(moveDistance.x, 0.0f);
						break;
					default:
						break;
				}

				container_->setPosition( ccpAdd(container_->getPosition(), moveDistance) );

				if (bounces) 
				{
					maxInset = maxInset_;
					minInset = minInset_;
				} 
				else 
				{
					maxInset = maxContainerOffset();
					minInset = minContainerOffset();
				}
			   
				//check to see if offset lies within the inset bounds
				newX     = MIN(container_->getPosition().x, maxInset.x);
				newX     = MAX(newX, minInset.x);
				newY     = MIN(container_->getPosition().y, maxInset.y);
				newY     = MAX(newY, minInset.y);

				scrollDistance = ccpSub(moveDistance, ccp(newX - container_->getPosition().x, newY - container_->getPosition().y));
				setContentOffset(ccp(newX, newY));
			}
		}
	}

	// 투터치 상태로 움직임 = 확대
	//MOD: paging이 꺼져있을때만 투터치 moving 켜지게?
	//if ( touchPool->count() == 2 && isDragging && !isPagingEnabled)
	if ( touchPool->count() == 2 && isDragging)
	{
		touchMoved = true;
		CCLOG("2 touch moving");
		CCTouch *t1 = touchPool->getObjectAtIndex(0);
		CCTouch *t2 = touchPool->getObjectAtIndex(1);
		
		const float len = ccpDistance(container_->convertTouchToNodeSpace(t1), container_->convertTouchToNodeSpace(t2));
		setZoomScale(container_->getScale()*len/touchLength);
	}
	
}
void VVScrollView::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
 
    if (!m_bIsVisible || !isEnabled || isMovePageToOngoing || touchPool->count() == 0) 
	{
        return;
    }

	CCSetIterator it;
	int containCnt = 0;
    for( it = touches->begin(); it != touches->end(); it++)
    {  
		CCTouch* touch = (CCTouch*)*it;
		if ( touchPool->containsObject( touch ) )
		{
			containCnt++;
			if (touchMoved) {
				this->schedule( schedule_selector(VVScrollView::deaccelerateScrolling) );			
			}
			else 
			{
				// 디엑셀 스크롤 도중 사용자가 손으로 잡았다가 떼버리면 페이징이 안먹고 정지하는데
				// 이 상태에서 가장 가까운 페이지로 자동이동 하도록 작성
				if(isDeaccelPagingEnabled)
					movePageTo(getClosestPageFromContentOffset(this->getContentOffset()));
			}

			// MOD: 확대동작 잘 안하면 수정할것
			// 두손가락 올려놓은 상태에서 동시에떼면 TouchesEnded 한번만 호출 (CCSet* touches에는 터치 2개들어있음)
			// 하나씩 시간차두고 떼면 각각에대해 TouchesEnded 호출(CCSet* touches에는 터치 하나만 들어있음)
			//touchPool->removeObject( touch );
			touchPool->removeAllObjects();
		}
	}
	if (containCnt == 0) 
	{
		// 무효터치이므로 아무 동작 안한다
		return;
	}

	// 일반 페이징이 켜져있는경우 터치떼는 순간 기준으로 디엑셀 취소하고 한 페이지만 이동
	if (isPagingEnabled)
	{
		if (isSwipeGesture == true) 
		{
			CCPoint newPoint = convertTouchToNodeSpace((CCTouch*)touches->anyObject());	
			if (pagingDirection == VVScrollViewPagingDirectionHorizontal)
			{
				if ( (newPoint.x - touchPoint.x) < 0  && (currentPage+1) <= maxPage )
					movePageTo(currentPage+1);		
				else if ( (newPoint.x - touchPoint.x) > 0 && (currentPage-1) > 0 )
					movePageTo(currentPage-1);		
				else
					movePageTo(currentPage);
			}
			else if (pagingDirection == VVScrollViewPagingDirectionVertical)
			{
				if ( (newPoint.y - touchPoint.y) > 0 && (currentPage+1) <= maxPage )
					movePageTo(currentPage+1);		
				else if ( (newPoint.y - touchPoint.y) < 0 && (currentPage-1) > 0 )
					movePageTo(currentPage-1);		
				else
					movePageTo(currentPage);
			}
			else 
				CCAssert(0, "SetPagingDirection required");
			
		}
		else
		{
			CCPoint newPoint = convertTouchToNodeSpace((CCTouch*)touches->anyObject());
			
			if (pagingDirection == VVScrollViewPagingDirectionHorizontal)
			{
				if ( (newPoint.x - touchPoint.x) < -minTouchLengthToChangePage && (currentPage+1) <= maxPage )
					movePageTo(currentPage+1);		
				else if ( (newPoint.x - touchPoint.x) > minTouchLengthToChangePage && (currentPage-1) > 0 )
					movePageTo(currentPage-1);		
				else
					movePageTo(currentPage);
			}
			else if (pagingDirection == VVScrollViewPagingDirectionVertical)
			{
				if ( (newPoint.y - touchPoint.y) > -minTouchLengthToChangePage && (currentPage+1) <= maxPage )
					movePageTo(currentPage+1);		
				else if ( (newPoint.y - touchPoint.y) < minTouchLengthToChangePage && (currentPage-1) > 0 )
					movePageTo(currentPage-1);		
				else
					movePageTo(currentPage);
			}
			else 
				CCAssert(0, "SetPagingDirection required");
			
		}

	}

	
	if (touchPool->count() == 0) 
	{
		isDragging = false;
		touchMoved = false;
	}
	//isValidTouch = false;
}
void VVScrollView::ccTouchesCancelled(CCSet* touches, CCEvent* event)
{
    if (!m_bIsVisible || !isEnabled) 
	{
        return;
    }
    this->ccTouchesEnded(touches, event);
}

// 0.3초이상이 지나면 이 함수가 호출되면서 SwipeGesture가 아님을 판명
// 0.3초 이내라면 이 함수가 호출되지 않을것이고 isSwipeGesture값이 true로 유지됨
void VVScrollView::swipeGestureFinder()
{
	isSwipeGesture = false;
	this->unschedule(schedule_selector(VVScrollView::swipeGestureFinder));
}
void VVScrollView::movePageTo(int page)
{
	CCAssert(isPagingEnabled || isDeaccelPagingEnabled, "Do not call movePageTo when paging is disabled");
	
	// movePageTo 실행중인동안 터치 안받음
	isMovePageToOngoing = true;
	
	if (currentPage < page) 
		scrAniDirection = VVScrollViewPagingToPlus;
	else if (currentPage > page)
		scrAniDirection = VVScrollViewPagingToMinus;
	else
		scrAniDirection = VVScrollViewPagingToStay;
	
	// 미리 페이지 변수 바꾸고, 스크롤 애니메이션 끝나는 시점에서 페이이지바뀌었다고 호출할것
	currentPage = page;
	this->setContentOffset( getContentOffsetFromPage(page),(ccTime)0.5 );
}

CCPoint VVScrollView::getContentOffsetFromPage(int page)
{
	if (pagingDirection == VVScrollViewPagingDirectionVertical)
	{
		return ccp( getContentOffset().x, -(getContentSize().height - getViewSize().height)+((page-1)*pagingSize.height) );
	}
	else if (pagingDirection == VVScrollViewPagingDirectionHorizontal)
	{
		return ccp(-((page-1)*pagingSize.width),	getContentOffset().y );
	}
	else
	{
		CCAssert(0,"PagingDirection should be set");
	}

	
}

// 스크롤 셀렉터가 필요할 경우 이 함수를 오버라이드 할것.
// 콜린스사전의 NavDays.cpp에 샘플 있음
int VVScrollView::getClosestPageFromContentOffset(CCPoint offset)
{
	int page;
	if (pagingDirection == VVScrollViewPagingDirectionVertical)
	{
		page = round( (offset.y + (getContentSize().height - getViewSize().height) ) / pagingSize.height) + 1;
	}
	else if (pagingDirection == VVScrollViewPagingDirectionHorizontal)
	{
		page = round( -offset.x / pagingSize.width ) + 1;
	}
	else
	{
		CCAssert(0,"PagingDir should be set");
	}
	
	// 페이지 바운더리가 넘어간 경우 다시 돌려놓기
	if( page < 1 - numberOfEmptyPages)
	{
		page = 1 - numberOfEmptyPages;
	}
	else if (page > maxPage - numberOfEmptyPages)
	{
		page = maxPage - numberOfEmptyPages;
	}
	

	
	return page;
}

void VVScrollView::onExit()
{
	CCLayer::onExit(); // super클래스 호출
}

VVScrollView::~VVScrollView()
{
	this->unschedule(schedule_selector(VVScrollView::deaccelerateScrolling));
	this->unschedule(schedule_selector(VVScrollView::performedAnimatedScroll));
	CC_SAFE_RELEASE( touchPool );
	CC_SAFE_RELEASE( container_ );
}