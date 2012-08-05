/*
 *  CCRawImageData.cpp
 *  CocosX_test
 *
 *  Created by YoungJae Kwon on 11. 5. 28..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#include "CCRawImageData.h"
#include "CCFileUtils.h"

#include <stdio.h>

using namespace std;
namespace   cocos2d {
	
	CCRawImageData::CCRawImageData(void)
	: m_pData(NULL)
	{
	}
	
	CCRawImageData::~CCRawImageData(void)
	{
		CC_SAFE_DELETE_ARRAY(m_pData);
	}
	
	CCRawImageData* CCRawImageData::dataWithContentsOfFile(const string &strPath)
	{
		CCFileData data(strPath.c_str(), "rb");
		unsigned long  nSize = data.getSize();
		unsigned char* pBuffer = data.getBuffer();
		
		if (! pBuffer)
		{
			return NULL;
		}
		
		CCRawImageData *pRet = new CCRawImageData();
		pRet->autorelease();
		pRet->m_pData = new unsigned char[nSize];
		memcpy(pRet->m_pData, pBuffer, nSize);
		
		
		return pRet;
	}
	
	void* CCRawImageData::bytes(void)
	{
		return m_pData;
	}
	

	CCRawImageData* CCRawImageData::dataWithBytes(const unsigned char *pBytes, int size)
	{
		CCRawImageData *pRet = new CCRawImageData();
		pRet->autorelease();
		pRet->m_pData = new unsigned char[size];
		memcpy(pRet->m_pData, pBytes, size);
		
		return pRet;
	}
	
	// pBytes는 malloc이나 new를 통해 힙상에 잡혀있는 메모리여야 함
	// memcpy 없이 포인터만 저장
	CCRawImageData* CCRawImageData::dataWithBytesNoCopy(unsigned char *pBytes, int size)
	{
		CCRawImageData *pRet = new CCRawImageData();
		pRet->autorelease();
		pRet->m_pData = pBytes;
		pRet->length = size;
		return pRet;
	}
	
	void CCRawImageData::setInfo(CCSize aSize, int aBitsPerPixel, int aBitsPerComponent)
	{
		bitsPerPixel = aBitsPerPixel;
		bitsPerComponent = aBitsPerComponent;
		size = aSize;
		bytesPerRow	= (bitsPerPixel/8) * size.width;
		
	}
	
}//namespace   cocos2d 
