/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "EAGLView.h"
#include "CCDirectorCaller.h"
#include "CCEGLView_ios.h"
#include "CCSet.h"
#include "CCTouch.h"
#include "CCTouchDispatcher.h"
#include "CCDirector.h"


namespace cocos2d {

CCEGLView::CCEGLView()
: m_pDelegate(0),
m_bNotHVGA(false), 
m_fScreenScaleFactor(1.0)  
{

}

CCEGLView::~CCEGLView()
{

}
    void CCEGLView::setFrameWidthAndHeight(int width, int height)
    {
        m_sSizeInPixel.width = width;
        m_sSizeInPixel.height = height;
    }  

    void CCEGLView::create(int width, int height)
    {
        if (width == 0 || height == 0)
        {
            return;
        }
        
        m_sSizeInPoint.width = width;
        m_sSizeInPoint.height = height;
        
        // calculate the factor and the rect of viewport	
        m_fScreenScaleFactor =  MIN((float)m_sSizeInPixel.width / m_sSizeInPoint.width, 
                                    (float)m_sSizeInPixel.height / m_sSizeInPoint.height);
        int viewPortW = (int)(m_sSizeInPoint.width * m_fScreenScaleFactor);
        int viewPortH = (int)(m_sSizeInPoint.height * m_fScreenScaleFactor);
        m_rcViewPort.origin.x = (m_sSizeInPixel.width - viewPortW) / 2;
        m_rcViewPort.origin.y = (m_sSizeInPixel.height - viewPortH) / 2;
        m_rcViewPort.size.width = viewPortW;
        m_rcViewPort.size.height = viewPortH;
        
        m_bNotHVGA = true;
        
    }
    
cocos2d::CCSize  CCEGLView::getSize()
{
    if (m_bNotHVGA)
    {
        CCSize size(m_sSizeInPoint.width, m_sSizeInPoint.height);
		return size;
	}

	cocos2d::CCSize size([[EAGLView sharedEGLView] getWidth], [[EAGLView sharedEGLView] getHeight]);
	return size;
}

bool CCEGLView::isIpad()
{
    return UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad;
}

bool CCEGLView::isOpenGLReady()
{
    return [EAGLView sharedEGLView] != NULL;
}
    
    bool CCEGLView::canSetContentScaleFactor()
    {
        //return true;
		//modified by LEETAEHO 11.07.12 : for use iphone4 image to iphone3
//        if( [[UIScreen mainScreen] bounds].size.width <= 480 ) // 패드는 그대로, 폰만 custom scaling
//            return true;
        return true; // 폰 패드 모두 custom scaling 가능해짐.
#ifdef PD_IPHONE
		return true;
#else
       return [[EAGLView sharedEGLView] respondsToSelector:@selector(setContentScaleFactor:)]
               && [[UIScreen mainScreen] scale] != 1.0;
#endif
    }
    
    void CCEGLView::setContentScaleFactor(float contentScaleFactor)
    {
        UIView * view = [EAGLView sharedEGLView];
        view.contentScaleFactor = contentScaleFactor;
        [view setNeedsLayout];

        //으악
        CCLOG("iOS Content scale: %f",contentScaleFactor);
    }

void CCEGLView::release()
{
	[CCDirectorCaller destroy];
	
	// destroy EAGLView
	[[EAGLView sharedEGLView] removeFromSuperview];
}

void CCEGLView::setTouchDelegate(EGLTouchDelegate * pDelegate)
{
    m_pDelegate = pDelegate;
}

void CCEGLView::swapBuffers()
{
	[[EAGLView sharedEGLView] swapBuffers];
}
	
void CCEGLView::touchesBegan(CCSet *set)
{
	if (m_pDelegate) {
		m_pDelegate->touchesBegan(set, NULL);
	}
}

void CCEGLView::touchesMoved(CCSet *set)
{
	if (m_pDelegate) {
		m_pDelegate->touchesMoved(set, NULL);
	}
}

void CCEGLView::touchesEnded(CCSet *set)
{
	if (m_pDelegate) {
		m_pDelegate->touchesEnded(set, NULL);
	}
}

void CCEGLView::touchesCancelled(CCSet *set)
{
	if (m_pDelegate) {
		m_pDelegate->touchesCancelled(set, NULL);
	}
}

void CCEGLView::setViewPortInPoints(float x, float y, float w, float h)
{
    if (m_bNotHVGA)
    {
        float factor = m_fScreenScaleFactor / CC_CONTENT_SCALE_FACTOR();
        glViewport((GLint)(x * factor) + m_rcViewPort.origin.x,
                   (GLint)(y * factor) + m_rcViewPort.origin.y,
                   (GLint)(w * factor),
                   (GLint)(h * factor));
    }
    else
    glViewport((GLint)x, (GLint)y, (GLint)w, (GLint)h);
}

void CCEGLView::setScissorInPoints(float x, float y, float w, float h)
{
    glScissor((GLint)x, (GLint)y, (GLint)w, (GLint)h);
}

void CCEGLView::setIMEKeyboardState(bool bOpen)
{
    if (bOpen)
    {
        [[EAGLView sharedEGLView] becomeFirstResponder];
    }
    else
    {
        [[EAGLView sharedEGLView] resignFirstResponder];
    }
}

CCEGLView& CCEGLView::sharedOpenGLView()
{
    static CCEGLView instance;
    return instance;
}

float CCEGLView::getMainScreenScale()
{
    return [[UIScreen mainScreen] scale];
}

} // end of namespace cocos2d;
