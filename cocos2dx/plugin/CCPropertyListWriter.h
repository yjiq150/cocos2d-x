/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org  http://cocos2d-x.org
 Copyright (c) 2010 Максим Аксенов
 
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

#ifndef __CCPropertyListWriter_H__
#define __CCPropertyListWriter_H__



#include "CCPlatformConfig.h"
#include "CCCommon.h"
#include <string>
#include "CCObject.h"
#include "CCMutableDictionary.h"
#include "CCArray.h"



using namespace std;


// libxml2 on most platforms are using "unsigned char" as type of string, while on airplay sdk using "char"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WOPHONE) || \
(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)   || \
(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || \
(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
typedef unsigned char CC_XML_CHAR;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_AIRPLAY)
typedef char CC_XML_CHAR;
#else
#error
#endif

typedef struct _xmlTextWriter xmlTextWriter;
typedef xmlTextWriter *xmlTextWriterPtr;

NS_CC_BEGIN;

class CC_DLL CCPropertyListWriter
{

public:

	CCPropertyListWriter();
	~CCPropertyListWriter();

    bool writeToFile(CCDictionary<std::string,CCObject*>* aDic , const char *pszFile);


    
protected:
    void writeValue(CCObject* aValue);
    void writeArray(CCArray* aArray);
    void writeDictionary(CCMutableDictionary<>* aDic);
    
    void openPlistTag();
    void closePlistTag();
    
    string fullPath;
    xmlTextWriterPtr writer;
};
NS_CC_END;

#endif //__CCPropertyListWriter_H__
