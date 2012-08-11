//
//  VVLocalizedStringManager.cpp
//  Cocos2dxTemplate
//
//  Created by YoungJae Kwon on 12. 5. 2..
//  Copyright (c) 2012년 Beyondi Soft All rights reserved.
//

#include "VVLocalizedStringManager.h"
#include "cocos2d.h"
#include "CCFileUtils.h"

#define DEFAULT_LANG_CODE "en"

using namespace std;
USING_NS_CC;

static VVLocalizedStringManager *g_shared;


VVLocalizedStringManager* VVLocalizedStringManager::shared()
{
    if (!g_shared)
        g_shared = new VVLocalizedStringManager();
    
    return g_shared;
}
VVLocalizedStringManager::VVLocalizedStringManager()
{
    m_jsonObj = NULL;
    
    addLanguageSupport(DEFAULT_LANG_CODE);
}

void VVLocalizedStringManager::initialize()
{
    string langCode = CCApplication::getCurrentLanguageCode();
    
    if( !isLanguageSupported(langCode) )
        langCode = DEFAULT_LANG_CODE;
    
    currentLangCode = langCode;

    CCLog("LocalizedStringManager Created for : %s",langCode.c_str());
    loadLocalizedStringFile();
}

void VVLocalizedStringManager::addLanguageSupport(string langCode)
{
    supportLanguages[langCode] = true;
}


// maked localized string on given format
string VVLocalizedStringManager::makeLocalized(const char* formattedStr)
{
    if(strlen(formattedStr) > 512 )
        CCAssert(0, "formattedString cannot be longer than 512");
    
    char buf[512]; 
    sprintf(buf, formattedStr, currentLangCode.c_str());
    string ret = buf;
    return ret;
}

// load localized string by key from JSON langcode.str file.
string VVLocalizedStringManager::getStringForKey(const char* aKey)
{
    CCAssert(supportLanguages.size() != 0, "Add more than one support language first");
    CCAssert(m_jsonObj != NULL, "JSON string file it not loaded");
    
    return m_jsonObj[aKey].asString();
}


void VVLocalizedStringManager::loadLocalizedStringFile()
{
    CCAssert(supportLanguages.size() != 0, "Add more than one support language first");
    
    string filePath = currentLangCode + ".str"; // Localized string JSON file
    
    filePath = CCFileUtils::fullPathFromRelativePath(filePath.c_str());
    
    CCFileData fileData(filePath.c_str(), "rb");
	unsigned long size = fileData.getSize();
	char *pBuffer = (char*) fileData.getBuffer();
	
	if (pBuffer != NULL)
    {
        Json::Reader* jsonReader = new Json::Reader();
        bool isParsingSuccessful = jsonReader->parse( pBuffer, pBuffer+size, m_jsonObj,false );
        
        if ( !isParsingSuccessful )
        {
            string err = "JSON parsing Error :" + filePath;
            CCAssert(0,err.c_str());
        }
        
#if !(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        // sth wrong in NDK free() mechanism, so let it leak on android(BUG).
        delete jsonReader;
#endif    
	}
    else 
    {
        m_jsonObj = NULL;
    }
}


bool VVLocalizedStringManager::isLanguageSupported(string langCode)
{
    map<string,bool>::iterator it = supportLanguages.find(langCode);
    
    if (it == supportLanguages.end()) 
        return false;
    else
        return true;
}
