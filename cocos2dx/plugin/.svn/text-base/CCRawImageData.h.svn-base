/*
 *  CCRawImageData.h
 *  CocosX_test
 *
 *  Created by YoungJae Kwon on 11. 5. 28..
 *  Copyright 2011 Viscuit Creative Inc. All rights reserved.
 *
 */

#ifndef __CCRAWIMAGE_DATA_H__
#define __CCRAWIMAGE_DATA_H__

#include "CCObject.h"
#include <string>
#include "CCGeometry.h"
namespace   cocos2d {
	
	class CCRawImageData : public CCObject
	{
	public:
		CCRawImageData(void);
		~CCRawImageData(void);
		
		void* bytes(void);
		static CCRawImageData* dataWithBytesNoCopy(unsigned char *pBytes, int size);	
		static CCRawImageData* dataWithBytes(const unsigned char *pBytes, int size);
		static CCRawImageData* dataWithContentsOfFile(const std::string &strPath);
		
		void setInfo(CCSize aSize, int aBitsPerPixel=32, int aBitsPerComponent=8);
		CC_SYNTHESIZE_READONLY(unsigned int, length, Length);

		int bitsPerComponent;			
		int bitsPerPixel;		
		int bytesPerRow;
		CCSize size;
		
		unsigned char *m_pData;
		
	private:
		
				
		
		
	};
}//namespace   cocos2d 

#endif //__CCRAWIMAGE_DATA_H__
