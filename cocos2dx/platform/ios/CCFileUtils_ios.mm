/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.

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
#import <Foundation/Foundation.h>
#import <UIKit/UIDevice.h>

#include <string>
#include <stack>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include "CCString.h"
#include "CCFileUtils.h"
#include "CCDirector.h"
#include "CCSAXParser.h"
#include "support/zip_support/unzip.h"
#include "ccTypes.h"

#define MAX_PATH 260

using namespace cocos2d;

// added by YoungJae Kwon 
// to support CCNumber class parsing
static void static_addValueToCCDict(id key, id value, CCDictionary<std::string, CCObject*>* pDict, bool isUsingCCNumber);
static void static_addItemToCCArray(id item, CCObject* pArray, bool isUsingCCNumber);
//static void static_addValueToCCDict(id key, id value, CCDictionary<std::string, CCObject*>* pDict);
//static void static_addItemToCCArray(id item, CCMutableArray<CCObject*> *pArray);


static NSString *__suffixiPhoneRetinaDisplay =@"-hd";
static NSString *__suffixiPad =@"-ipad";
static NSString *__suffixiPadRetinaDisplay =@"-ipadhd";
static NSFileManager *__localFileManager= [[NSFileManager alloc] init];

// NSString *ccRemoveSuffixFromPath( NSString *suffix, NSString *path);


static NSString* removeSuffixFromPath(NSString *suffix, NSString *path)
{
    // quick return
	if( ! suffix || [suffix length] == 0 )
	{
		return path;
    }
    
	NSString *name = [path lastPathComponent];
    
	// check if path already has the suffix.
	if( [name rangeOfString:suffix].location != NSNotFound ) {
        
		CCLOG("cocos2d: Filename(%s) contains %s suffix. Removing it. See cocos2d issue #1040", [path UTF8String], [suffix UTF8String]);
        
		NSString *newLastname = [name stringByReplacingOccurrencesOfString:suffix withString:@""];
        
		NSString *pathWithoutLastname = [path stringByDeletingLastPathComponent];
		return [pathWithoutLastname stringByAppendingPathComponent:newLastname];
	}
    
	return path;
}

static NSString* getPathForSuffix(NSString *path, NSString *suffix)
{
    // quick return
	if( ! suffix || [suffix length] == 0 )
	{
		return path;
    }
    
	NSString *pathWithoutExtension = [path stringByDeletingPathExtension];
	NSString *name = [pathWithoutExtension lastPathComponent];
    
	// check if path already has the suffix.
	if( [name rangeOfString:suffix].location != NSNotFound ) {
        
		CCLOG("cocos2d: WARNING Filename(%s) already has the suffix %s. Using it.", [name UTF8String], [suffix UTF8String]);
		return path;
	}
    
    
	NSString *extension = [path pathExtension];
    
	if( [extension isEqualToString:@"ccz"] || [extension isEqualToString:@"gz"] )
	{
		// All ccz / gz files should be in the format filename.xxx.ccz
		// so we need to pull off the .xxx part of the extension as well
		extension = [NSString stringWithFormat:@"%@.%@", [pathWithoutExtension pathExtension], extension];
		pathWithoutExtension = [pathWithoutExtension stringByDeletingPathExtension];
	}
    
    
	NSString *newName = [pathWithoutExtension stringByAppendingString:suffix];
	newName = [newName stringByAppendingPathExtension:extension];
    
	if( [__localFileManager fileExistsAtPath:newName] )
		return newName;
    
	CCLOG("cocos2d: CCFileUtils: Warning file not found: %s", [[newName lastPathComponent] UTF8String] );
    
	return nil;
}

static void static_addItemToCCArray(id item, CCMutableArray<CCObject*> *pArray)
{
	// add string value into array
    if ([item isKindOfClass:[NSString class]]) {
        CCString* pValue = new CCString([item UTF8String]);
        if (!isUsingCCNumber) {
            ((CCMutableArray<>*)pArray)->addObject(pValue);
        }
        else {
            ((CCArray*)pArray)->addObject(pValue);
        }
        
        pValue->release();
        return;
    }

	// add number value into array(such as int, float, bool and so on)
    if ([item isKindOfClass:[NSNumber class]]) {
        NSString* pStr = [item stringValue];
        if( !isUsingCCNumber )
        {
        CCString* pValue = new CCString([pStr UTF8String]);
        
            ((CCMutableArray<>*)pArray)->addObject(pValue);
        pValue->release();
        }
        else
        {
            CCLog("array_type:%s",[item objCType]);
            if( strcmp([item objCType], @encode(int)) == 0 ||
               strcmp([item objCType], @encode(long)) == 0 ||
               strcmp([item objCType], @encode(unsigned int)) == 0 ||
               strcmp([item objCType], @encode(unsigned long)) == 0 ||
               strcmp([item objCType], @encode(long long)) == 0 ||
               strcmp([item objCType], @encode(unsigned long long)) == 0 )
            {
                ((CCArray*)pArray)->addObject( CCNumber<int>::numberWithValue([item intValue]) );
            }
            else if( strcmp([item objCType], @encode(float)) == 0 || 
                    strcmp([item objCType], @encode(double)) == 0 )
            {
                ((CCArray*)pArray)->addObject( CCNumber<float>::numberWithValue([item floatValue]) );                
            }
            else if( strcmp([item objCType], @encode(bool)) == 0 )
            {
                ((CCArray*)pArray)->addObject( CCNumber<bool>::numberWithValue([item boolValue]) );                                
            }
            else {
                CCAssert(0, "static_addValueToCCArray::Unknown value type paring");
            }
        }
        return;
    }
    
    // add dictionary value into array
    if ([item isKindOfClass:[NSDictionary class]]) {
        CCDictionary<std::string, CCObject*>* pDictItem = new CCDictionary<std::string, CCObject*>();
        for (id subKey in [item allKeys]) {
            id subValue = [item objectForKey:subKey];
            static_addValueToCCDict(subKey, subValue, pDictItem, isUsingCCNumber);
        }
        if (!isUsingCCNumber) {
            ((CCMutableArray<>*)pArray)->addObject(pDictItem);
        }
        else {
            ((CCArray*)pArray)->addObject(pDictItem);
        }

        pDictItem->release();
        return;
    }
    
    // add array value into array
    if ([item isKindOfClass:[NSArray class]]) {
        if(!isUsingCCNumber)
        {
            
        CCMutableArray<CCObject*> *pArrayItem = new CCMutableArray<CCObject*>();
        for (id subItem in item) {
                static_addItemToCCArray(subItem, pArrayItem, isUsingCCNumber);
        }
            ((CCMutableArray<>*)pArray)->addObject(pArrayItem);
        pArrayItem->release();
        }
        else
        {

            CCArray *pArrayItem = CCArray::array();
            CCLog("adding item to array %p",pArrayItem);            
            for (id subItem in item) {
                
                static_addItemToCCArray(subItem, pArrayItem, isUsingCCNumber);
            }
            ((CCArray*)pArray)->addObject(pArrayItem);
            CCLog("to array %p added complete",pArrayItem);
        }
        return;
    }
}

static void static_addValueToCCDict(id key, id value, CCDictionary<std::string, CCObject*>* pDict, bool isUsingCCNumber)
{
	// the key must be a string
    CCAssert([key isKindOfClass:[NSString class]], "The key should be a string!");
    std::string pKey = [key UTF8String];

	// the value is a new dictionary
    if ([value isKindOfClass:[NSDictionary class]]) {
        CCDictionary<std::string, CCObject*>* pSubDict = new CCDictionary<std::string, CCObject*>();
        for (id subKey in [value allKeys]) {
            id subValue = [value objectForKey:subKey];
            static_addValueToCCDict(subKey, subValue, pSubDict, isUsingCCNumber);
        }
        pDict->setObject(pSubDict, pKey);
        pSubDict->release();
        return;
    }

	// the value is a string
    if ([value isKindOfClass:[NSString class]]) {
        CCString* pValue = new CCString([value UTF8String]);

        pDict->setObject(pValue, pKey);
        pValue->release();
        return;
    }

	// the value is a number
    if ([value isKindOfClass:[NSNumber class]]) {
        NSString* pStr = [value stringValue];
        if( !isUsingCCNumber )
        {
        CCString* pValue = new CCString([pStr UTF8String]);
        
        pDict->setObject(pValue, pKey);
        pValue->release();
        }
        else
        {
            CCLog("dic_type:%s",[value objCType]);
            if( strcmp([value objCType], @encode(int)) == 0 ||
               strcmp([value objCType], @encode(long)) == 0 ||
               strcmp([value objCType], @encode(unsigned int)) == 0 ||
               strcmp([value objCType], @encode(unsigned long)) == 0 ||
               strcmp([value objCType], @encode(long long)) == 0 ||
              strcmp([value objCType], @encode(unsigned long long)) == 0 )
            {
                CCLog("added int:%d",[value intValue]);
                pDict->setObject( CCNumber<int>::numberWithValue([value intValue]), pKey);
            }
            else if( strcmp([value objCType], @encode(float)) == 0 || 
                    strcmp([value objCType], @encode(double)) == 0 )
            {
                CCLog("added float:%f",[value floatValue]);
                pDict->setObject( CCNumber<float>::numberWithValue([value floatValue]), pKey);                
            }
            else if( strcmp([value objCType], @encode(bool)) == 0 )
            {
                CCLog("added bool:%d",[value boolValue]);                
                pDict->setObject( CCNumber<bool>::numberWithValue([value boolValue]), pKey);                                
            }
            else {
                CCAssert(0, "static_addValueToCCDict::Unknown value type paring");
            }
                

        }

        return;
    }

	// the value is a array
    if ([value isKindOfClass:[NSArray class]]) {
        if( !isUsingCCNumber )
        {
        CCMutableArray<CCObject*> *pArray = new CCMutableArray<CCObject*>();
        for (id item in value) {
                static_addItemToCCArray(item, pArray, isUsingCCNumber);
        }
        pDict->setObject(pArray, pKey);
        pArray->release();
        }
        else
        {
            CCArray *pArray = CCArray::array();
            for (id item in value) {
                static_addItemToCCArray(item, pArray, isUsingCCNumber);
            }
            pDict->setObject(pArray, pKey);
        }
        return;
    }
}

namespace cocos2d {

    void CCFileUtils::setResourcePath(const char *pszResourcePath)
    {
        assert(0);
    }
    
    int CCFileUtils::ccLoadFileIntoMemory(const char *filename, unsigned char **out)
    {
        CCAssert( out, "ccLoadFileIntoMemory: invalid 'out' parameter");
        CCAssert( &*out, "ccLoadFileIntoMemory: invalid 'out' parameter");

        size_t size = 0;
        FILE *f = fopen(filename, "rb");
        if( !f ) { 
            *out = NULL;
            return -1;
        } 

        fseek(f, 0, SEEK_END);
        size = ftell(f);
        fseek(f, 0, SEEK_SET);

        *out = (unsigned char*)malloc(size);
        size_t read = fread(*out, 1, size, f);
        if( read != size ) { 
            free(*out);
            *out = NULL;
            return -1;
        }

        fclose(f);

        return size;
    }

    std::string& CCFileUtils::removeSuffixFromFile(std::string& cpath )
    {
        NSString *ret = nil;
        NSString *path = [NSString stringWithUTF8String:cpath.c_str()];
    
	    if( UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad )
	    {
		    if( CC_CONTENT_SCALE_FACTOR() == 2 )
		    {
			    ret = removeSuffixFromPath(__suffixiPadRetinaDisplay, path);
			}
		    else
		    {
			    ret = removeSuffixFromPath(__suffixiPad, path);
			}	
	    }
	    else
	    {
		    if( CC_CONTENT_SCALE_FACTOR() == 2 )
		    {
			    ret = removeSuffixFromPath(__suffixiPhoneRetinaDisplay, [NSString stringWithUTF8String:cpath.c_str()]);
			}
		    else
		    {
			    ret = path;
			}
	    }
	
	    cpath = [ret UTF8String];
        return cpath;
    }
    
    void CCFileUtils::setiPhoneRetinaDisplaySuffix(const char *suffix)
    {
        [__suffixiPhoneRetinaDisplay release];
	    __suffixiPhoneRetinaDisplay = [[NSString stringWithUTF8String:suffix] retain];
    }
    
    void CCFileUtils::setiPadSuffix(const char *suffix)
    {
        [__suffixiPad release];
	    __suffixiPad = [[NSString stringWithUTF8String:suffix] retain];
    }
    
    void CCFileUtils::setiPadRetinaDisplaySuffix(const char *suffix)
    {
        [__suffixiPadRetinaDisplay release];
	    __suffixiPadRetinaDisplay = [[NSString stringWithUTF8String:suffix] retain];
    }
    
    bool fileExistsAtPath(const char *cpath, const char *csuffix)
    {
        NSString *fullpath = nil;
        NSString *relPath = [NSString stringWithUTF8String:cpath];
        NSString *suffix = [NSString stringWithUTF8String:csuffix];
    
	    // only if it is not an absolute path
	    if( ! [relPath isAbsolutePath] ) {
		    // pathForResource also searches in .lproj directories. issue #1230
		    NSString *file = [relPath lastPathComponent];
		    NSString *imageDirectory = [relPath stringByDeletingLastPathComponent];
        
		    fullpath = [[NSBundle mainBundle] pathForResource:file
												   ofType:nil
											  inDirectory:imageDirectory];
        
	    }
    
	    if (fullpath == nil)
		    fullpath = relPath;
    
	    NSString *path = getPathForSuffix(fullpath, suffix);
    
	    return ( path != nil );
    }
    
    bool CCFileUtils::iPhoneRetinaDisplayFileExistsAtPath(const char *cpath)
    {
        return fileExistsAtPath(cpath, [__suffixiPhoneRetinaDisplay UTF8String]);
    }
    
    bool CCFileUtils::iPadFileExistsAtPath(const char *cpath)
    {
        return fileExistsAtPath(cpath, [__suffixiPad UTF8String]);
    }
    
    bool CCFileUtils::iPadRetinaDisplayFileExistsAtPath(const char *cpath)
    {
        return fileExistsAtPath(cpath, [__suffixiPadRetinaDisplay UTF8String]);
    }

    const char* CCFileUtils::fullPathFromRelativePath(const char *pszRelativePath)
    {
        ccResolutionType ignore;
        return fullPathFromRelativePath(pszRelativePath, &ignore);
    }
    
    const char* CCFileUtils::fullPathFromRelativePath(const char *pszRelativePath, ccResolutionType *pResolutionType)
    {
        CCAssert(pszRelativePath != NULL, "CCFileUtils: Invalid path");
    
	    NSString *fullpath = nil;
	    NSString *relPath = [NSString stringWithUTF8String:pszRelativePath];
    
	    // only if it is not an absolute path
	    if( ! [relPath isAbsolutePath] ) {
        
		    // pathForResource also searches in .lproj directories. issue #1230
		    NSString *file = [relPath lastPathComponent];
		    NSString *imageDirectory = [relPath stringByDeletingLastPathComponent];
        
		    fullpath = [[NSBundle mainBundle] pathForResource:file
												   ofType:nil
											  inDirectory:imageDirectory];
        
        
	    }
    
	    if (fullpath == nil)
	    {
		    fullpath = relPath;
		}
    
	    NSString *ret = nil;
    
	    // iPad?
	    if( UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad)
	    {
		    // Retina Display ?
		    if( CC_CONTENT_SCALE_FACTOR() == 2 ) {
			    ret = getPathForSuffix(fullpath, __suffixiPadRetinaDisplay);
			    *pResolutionType = kCCResolutioniPadRetinaDisplay;
		    }
		    else
		    {
			    ret = getPathForSuffix(fullpath, __suffixiPad);
			    *pResolutionType = kCCResolutioniPad;			
		    }
	    }
	    // iPhone ?
	    else
	    {
		    // Retina Display ?
		    if( CC_CONTENT_SCALE_FACTOR() == 2 ) {
			    ret = getPathForSuffix(fullpath, __suffixiPhoneRetinaDisplay);
			    *pResolutionType = kCCResolutioniPhoneRetinaDisplay;
		    }
	    }
	
	    // If it is iPhone Non RetinaDisplay, or if the previous "getPath" failed, then use iPhone images.
	    if( ret == nil )
	    {
		    *pResolutionType = kCCResolutioniPhone;
		    ret = fullpath;
	    }
    
	    return [ret UTF8String];
    }

    const char *CCFileUtils::fullPathFromRelativeFile(const char *pszFilename, const char *pszRelativeFile)
    {
        std::string relativeFile = fullPathFromRelativePath(pszRelativeFile);
        CCString *pRet = new CCString();
        pRet->autorelease();
        pRet->m_sString = relativeFile.substr(0, relativeFile.rfind('/')+1);
        pRet->m_sString += pszFilename;
        return pRet->m_sString.c_str();
    }
    
	CCDictionary<std::string, CCObject*> *CCFileUtils::dictionaryWithContentsOfFile(const char *pFileName)
    {
	      //         CCDictionary<std::string, CCObject*> *ret = dictionaryWithContentsOfFileThreadSafe(pFileName, false);
	      // ret->autorelease();
	      // 
	      // return ret;
		dictionaryWithContentsOfFile(pFileName, false);
    }

    CCDictionary<std::string, CCObject*> *CCFileUtils::dictionaryWithContentsOfFileThreadSafe(const char *pFileName)
    {
        // NSString* pPath = [NSString stringWithUTF8String:pFileName];
        // NSDictionary* pDict = [NSDictionary dictionaryWithContentsOfFile:pPath];
        // 
        // CCDictionary<std::string, CCObject*>* pRet = new CCDictionary<std::string, CCObject*>();
        // for (id key in [pDict allKeys]) {
        //     id value = [pDict objectForKey:key];
        //     static_addValueToCCDict(key, value, pRet, false);
        // }
        // 
        // return pRet;
		return dictionaryWithContentsOfFileThreadSafe(pFileName, false);
    }
    
    // added by YoungJae Kwon
    CCDictionary<std::string, CCObject*> *CCFileUtils::dictionaryWithContentsOfFile(const char *pFileName, bool isUsingCCNumber)
    {
        CCDictionary<std::string, CCObject*> *ret = dictionaryWithContentsOfFileThreadSafe(pFileName, isUsingCCNumber);
	      ret->autorelease();
	      
	      return ret;
    }
    
	// added by YoungJae Kwon
	CCDictionary<std::string, CCObject*> *CCFileUtils::dictionaryWithContentsOfFileThreadSafe(const char *pFileName, bool isUsingCCNumber)
    {
        NSString* pPath = [NSString stringWithUTF8String:pFileName];
        NSDictionary* pDict = [NSDictionary dictionaryWithContentsOfFile:pPath];
        
        CCDictionary<std::string, CCObject*>* pRet = new CCDictionary<std::string, CCObject*>();
        for (id key in [pDict allKeys]) {
            id value = [pDict objectForKey:key];
            static_addValueToCCDict(key, value, pRet, isUsingCCNumber);
        }

        return pRet;
    }

    unsigned char* CCFileUtils::getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize)
    {
        unsigned char * pBuffer = NULL;

        do 
        {
            // read the file from hardware
            FILE *fp = fopen(pszFileName, pszMode);
            CC_BREAK_IF(!fp);

            fseek(fp,0,SEEK_END);
            *pSize = ftell(fp);
            fseek(fp,0,SEEK_SET);
            pBuffer = new unsigned char[*pSize];
            *pSize = fread(pBuffer,sizeof(unsigned char), *pSize,fp);
            fclose(fp);
        } while (0);

        if (! pBuffer && getIsPopupNotify()) 
        {
            std::string title = "Notification";
            std::string msg = "Get data from file(";
            msg.append(pszFileName).append(") failed!");

            CCMessageBox(msg.c_str(), title.c_str());
        }
        return pBuffer;
    }
    void CCFileUtils::setResource(const char* pszZipFileName)
    {
        CCAssert(0, "Have not implement!");
    }

    // notification support when getFileData from a invalid file
    //static bool s_bPopupNotify = true;
	// modified by LEETAEHO 11.06.30
	static bool s_bPopupNotify = false;

    void CCFileUtils::setIsPopupNotify(bool bNotify)
    {
        s_bPopupNotify = bNotify;
    }

    bool CCFileUtils::getIsPopupNotify()
    {
        return s_bPopupNotify;
    }
    
    std::string CCFileUtils::getWriteablePath()
    {
        // save to document folder
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        std::string strRet = [documentsDirectory UTF8String];
        strRet.append("/");
        return strRet;
    }
    
    unsigned char* CCFileUtils::getFileDataFromZip(const char* pszZipFilePath, const char* pszFileName, unsigned long * pSize)
    {
		    unsigned char * pBuffer = NULL;
		    unzFile pFile = NULL;
		    *pSize = 0;
		
		    do 
		    {
		        CC_BREAK_IF(!pszZipFilePath || !pszFileName);
		        CC_BREAK_IF(strlen(pszZipFilePath) == 0);
		
		        pFile = unzOpen(pszZipFilePath);
		        CC_BREAK_IF(!pFile);
		
		        int nRet = unzLocateFile(pFile, pszFileName, 1);
		        CC_BREAK_IF(UNZ_OK != nRet);
		
		        char szFilePathA[260];
		        unz_file_info FileInfo;
		        nRet = unzGetCurrentFileInfo(pFile, &FileInfo, szFilePathA, sizeof(szFilePathA), NULL, 0, NULL, 0);
		        CC_BREAK_IF(UNZ_OK != nRet);
		
		        nRet = unzOpenCurrentFile(pFile);
		        CC_BREAK_IF(UNZ_OK != nRet);
		
		        pBuffer = new unsigned char[FileInfo.uncompressed_size];
		        int nSize = 0;
		        nSize = unzReadCurrentFile(pFile, pBuffer, FileInfo.uncompressed_size);
		        CCAssert(nSize == 0 || nSize == (int)FileInfo.uncompressed_size, "the file size is wrong");
		
		        *pSize = FileInfo.uncompressed_size;
		        unzCloseCurrentFile(pFile);
		    } while (0);
		
		    if (pFile)
		    {
		        unzClose(pFile);
		    }

				return pBuffer;
    }

}//namespace cocos2d

