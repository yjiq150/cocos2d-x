/*
 *  VVPageFactory.h
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 3..
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __VVPageFactory_H__
#define __VVPageFactory_H__

/*
#include "Page00.h"
#include "PagePainter.h"
#include "PageIntro.h"
#include "PageTableViewTest.h"
#include "Sticker00.h"
 */

class VVPageBase;
// 사용법 주의!!!!
// VVPageFactory의 내용은 라이브러리가 아니므로 라이브러리 폴더에 있는것을 사용하고싶은 프로젝트내로 
// copy해 넣은 후 그 안에서 필요한 페이지 클래스들을 아래 예시와 같이 추가해서 사용할것. (단지 사용예시를 위해 여기에 추가해 놓았음)

// C++에서는 dynamic class loading이 가능하긴 하지만 dlopen과 같은 라이브러리 없이는
// 불가능할 뿐더러 사용한다고 해도 매우 복잡하므로 아래와같은 간단한 Factory코드로 대체했음

class VVPageFactory
{
public:
	VVPageFactory(std::string &aPageClassName)
	{
		pageClassName = aPageClassName;
	}
	~VVPageFactory()
	{
	}

	VVPageBase * createPage()
	{
		/*
		VVPageBase * page = NULL;
		if (pageClassName == "Page00") 
			page = new Page00();
		else if (pageClassName == "PagePainter")
			page = new PagePainter();
		else if (pageClassName == "PageIntro")
			page = new PageIntro();
		else if (pageClassName == "PageTableViewTest")
			page = new PageTableViewTest();
		else if (pageClassName == "Sticker00")
			page = new Sticker00();

		if (page != NULL) 
		{
			if (pageClassName == "PagePainter")
				((PagePainter*)page)->initWithIndex(1);
			else
				page->init();
			
			page->autorelease();
		}
		else 
			CCAssert(0, "존재하지 않는 페이지 생성호출");
	

		return page;
		 */
		return NULL;
	}
protected:
	std::string pageClassName;
};

#endif // __VVPageFactory_H__