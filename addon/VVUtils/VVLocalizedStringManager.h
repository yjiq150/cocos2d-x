//
//  VVLocalizedStringManager.h
//  Cocos2dxTemplate
//
//  Created by YoungJae Kwon on 12. 5. 2..
//  Copyright (c) 2012년 Beyondi Soft All rights reserved.
//

#ifndef _VVLocalizedStringManager_h
#define _VVLocalizedStringManager_h

#include <json/json.h>
#include <string>

#define VVLocalizedString(key, comment) \
    VVLocalizedStringManager::shared()->getStringForKey(key).c_str()

#define VVLocalizedFontName() \
VVLocalizedStringManager::shared()->getLocalizedFontName().c_str()


#define VVLocalizedStringMake(formattedStr) \
    VVLocalizedStringManager::shared()->makeLocalized(formattedStr).c_str()

using namespace std;

class VVLocalizedStringManager
{
public:
    static VVLocalizedStringManager* shared();
    VVLocalizedStringManager();

    void initialize();
    void addLanguageSupport(string langCode);
    void addFontNameForLanguage(string langCode, string aFontName);
    string makeLocalized(const char* formattedStr);
    string getStringForKey(const char* aKey);
    string getLocalizedFontName();


    
    
protected:
    map<string, bool> supportLanguages;
    map<string, string> fontNames;
    Json::Value m_jsonObj;
    string currentLangCode;

private:
    bool isLanguageSupported(string langCode);    
    void loadLocalizedStringFile();
};


#endif