/*
 *  StringHelper.cpp
 *  CollinsDictionaryHD
 *
 *  Created by YoungJae Kwon on 11. 10. 9..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#include "VVStringHelper.h"

#include <cctype>

using namespace std;

void VVStringHelper::deleteStringBetweenBracket(string &sInOut)
{
    size_t foundLeft, foundRight;
 
    // [aaa]형태를 찾아서 aaa를 삭제한다
    while (1) 
    {
        foundLeft = sInOut.find("[");
        foundRight = sInOut.find("]");
        if(foundLeft == string::npos || foundRight == string::npos)
            break;
        
        sInOut.erase(foundLeft,foundRight-foundLeft+1);
    }
}
    
void VVStringHelper::addCommaToNumberString( const string& sIn, string& sOut )
{
    int j, k, topI = sIn.length()-1;
    do
    {
        for( j = topI, k = 0; j >= 0 && k < 3; --j, ++k )
        {
            sOut = sIn[j] + sOut; // new char is added to front of sOut
            if( j > 0 && k == 2) sOut = "," + sOut; // j > 0 means still more digits
        }
        topI -= 3;
        
    }while( topI >= 0 );
}

void VVStringHelper::stdTrimBegin(string &sInOut)
{
	string::size_type iPosTrim = sInOut.find_first_not_of(" \t\r\n");
	if (iPosTrim == string::npos)
		sInOut.clear();
	else
	{
		string sTmp = sInOut.substr(iPosTrim);
		sInOut = sTmp;
	}
}

void VVStringHelper::stdTrimEnd(string &sInOut)
{
	string::size_type iPosTrim = sInOut.find_last_not_of(" \t\r\n");
	if (iPosTrim == string::npos)
		sInOut.clear();
	else
	{
		string sTmp = sInOut.substr(0, iPosTrim + 1);
		sInOut = sTmp;
	}
}

void VVStringHelper::stdTrim(string &sInOut)
{
	stdTrimBegin(sInOut);
	stdTrimEnd(sInOut);
}

void VVStringHelper::stdReplaceAll(string &sInOut, const char *pSearch, const char *pReplace)
{
	if (strlen(pSearch) == 0)
		return;
	
	string::size_type iBegin = 0;
	while (true)
	{
		iBegin = sInOut.find(pSearch, iBegin);
		if (iBegin == string::npos)
			break;
		
		sInOut.replace(iBegin, strlen(pSearch), pReplace);
		iBegin += strlen(pReplace);
	}
}

int VVStringHelper::stdSplit(const string& input, const string& delimiter, vector<string>& results, bool includeEmpties)
{
	int iPos = 0;
	int newPos = -1;
	int sizeS2 = (int)delimiter.size();
	int isize = (int)input.size();
	
	if( 
	   ( isize == 0 )
	   ||
	   ( sizeS2 == 0 )
	   )
	{
		return 0;
	}
	
	vector<int> positions;
	
	newPos = input.find (delimiter, 0);
	
	if( newPos < 0 )
	{ 
		return 0; 
	}
	
	int numFound = 0;
	
	while( newPos >= iPos )
	{
		numFound++;
		positions.push_back(newPos);
		iPos = newPos;
		newPos = input.find (delimiter, iPos+sizeS2);
	}
	
	if( numFound == 0 )
	{
		return 0;
	}
	
	for( int i=0; i <= (int)positions.size(); ++i )
	{
		string s("");
		if( i == 0 ) 
		{ 
			s = input.substr( i, positions[i] ); 
		}
		int offset = positions[i-1] + sizeS2;
		if( offset < isize )
		{
			if( i == positions.size() )
			{
				s = input.substr(offset);
			}
			else if( i > 0 )
			{
				s = input.substr( positions[i-1] + sizeS2, positions[i] - positions[i-1] - sizeS2 );
			}
		}
		if( includeEmpties || ( s.size() > 0 ) )
		{
			results.push_back(s);
		}
	}
	return numFound;
}

void VVStringHelper::toUpper(string &sInOut)
{
	std::transform(sInOut.begin(), sInOut.end(), sInOut.begin(), (int (*)(int))std::toupper);
}
void VVStringHelper::toLower(string &sInOut)
{
	std::transform(sInOut.begin(), sInOut.end(), sInOut.begin(), (int (*)(int))std::tolower);
}