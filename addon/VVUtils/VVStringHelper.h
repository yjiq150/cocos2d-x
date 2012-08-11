/*
 *  StringHelper.h
 *  CollinsDictionaryHD
 *
 *  Created by YoungJae Kwon on 11. 10. 9..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#ifndef __StringHelper_H__
#define __StringHelper_H__

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctype.h>
#include <algorithm>

using namespace std;

class VVStringHelper
{
public:
    static void deleteStringBetweenBracket(string &sInOut);
	static void addCommaToNumberString( const string& sIn, string& sOut );
	
	static void stdTrimBegin(string &sInOut);

	static void stdTrimEnd(string &sInOut);

	static void stdTrim(string &sInOut);

	static void stdReplaceAll(string &sInOut, const char *pSearch, const char *pReplace);

	static int stdSplit(const string& input, const string& delimiter, vector<string>& results, bool includeEmpties);
	
	static void toUpper(string& sInOut);
	static void toLower(string& sInOut);
};
#endif