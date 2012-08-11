/*
 *  VVOpenLink.cpp
 *  CollinsPictureCardHD
 *
 *  Created by YoungJae Kwon on 11. 11. 1..
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "VVOpenLink.h"

void VVOpenLink::openAppLinkWithAppID(char* appid)
{
    CCAssert(appid, "appid is NULL");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	NSString * langCode = [[NSLocale preferredLanguages] objectAtIndex:0];
    NSString* appidStr = [[[NSString alloc] initWithCString:appid encoding:NSUTF8StringEncoding] autorelease];
	NSString * urlString = [NSString stringWithFormat:@"http://www.viscuit.net/applink/?appid=%@&lang=%@&os=ios",appidStr,langCode];
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:urlString]];	
	
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	char buf [512];
	const char* storeName = "samsungapps";
	sprintf(buf,"http://www.viscuit.net/applink/?appid=%s&lang=%s&os=android&store=%s",appid,CCApplication::getCurrentLanguageCode(),storeName);
	CCApplication::openURL(buf);
#endif
}