//
//  SimpleHttpRequest.cpp
//  drawstory
//
//  Created by 张 靖宇 on 12-3-30.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "SimpleHttpRequest.h"

#include "CCThread.h"

using namespace cocos2d;

SimpleHttpRequest* SimpleHttpRequest::simpleHttpRequestWithURL(const std::string &url,
                                                               HttpMethod method,
                                                               SimpleHttpRequestDelegate* delegate,
                                                               int tag)
{
    SimpleHttpRequest* request = new SimpleHttpRequest(url,method,delegate,tag);
    CCLOG("SimpleHttpRequest::simpleHttpRequestWithURL:%s",url.c_str());
    if(request)
        request->autorelease();
    return request;
}

void* SimpleHttpRequest::doRequest(void *param)
{
    // create autorelease pool for iOS
	CCThread thread;
	thread.createAutoreleasePool();
    
    SimpleHttpRequest* request = static_cast<SimpleHttpRequest*>(param);
    // retain it
    
    char curlErrStr[CURL_ERROR_SIZE];
    
    request->isRequestErrorOccured_ = false;
    
    CURL* handle = curl_easy_init();
    if(handle)
    {
        curl_slist *httpHeaders = NULL;
        
        
        curl_easy_setopt(handle, CURLOPT_ERRORBUFFER,curlErrStr);
        curl_easy_setopt(handle, CURLOPT_URL, request->url().c_str());
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, SimpleHttpRequest::writeData);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, request );
        //        curl_easy_setopt(handle, CURLOPT_ENCODING, "gzip");
        
        HttpMethod method = request->httpMethod();
        if(method == kHttpMethodPost || method == kHttpMethodDelete) {
            curl_easy_setopt(handle, CURLOPT_POSTFIELDS,request->post().c_str());
            
            if(method == kHttpMethodDelete)
                curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST,"DELETE");
        }
        
        
        CURLcode curlErr = curl_easy_perform(handle);
        
        if(httpHeaders)
            curl_slist_free_all(httpHeaders);
        curl_easy_cleanup(handle);
        
        if(curlErr)
        {
            // Write error message to buffer
            std::stringstream ss(std::stringstream::in | std::stringstream::out);
            ss << "{\"result\":false,\"reason\":" << curlErr <<"}";
            request->buffer() = ss.str();
            
            request->isRequestErrorOccured_ = true;
        }
        else
            request->isRequestErrorOccured_ = false;
    }
    
    if(!request->cancelled() && request->delegate_)
    {
        request->isResponseReady_ = true;
    }
    
    pthread_detach(pthread_self());
    return (void*)0;
}

SimpleHttpRequest::SimpleHttpRequest(const std::string& url,HttpMethod method,SimpleHttpRequestDelegate* delegate,int tag)
: thread_(NULL)
, delegate_(delegate)
, tag_(tag)
, url_(url)
, method_(method)
, cancelled_(false)
, isResponseReady_(false)
, isRequestErrorOccured_(false)
{
    pthread_mutex_init(&mutex_, NULL);
}

SimpleHttpRequest::~SimpleHttpRequest()
{
    CCLOG("SimpleHttpRequest::~SimpleHttpRequest()");
    pthread_mutex_destroy(&mutex_);
}


size_t SimpleHttpRequest::writeData(char *data, size_t size, size_t nmemb, SimpleHttpRequest *userdata)
{
    size_t bytes = 0;
    
    if(userdata != NULL && !userdata->cancelled())
    {
        bytes = size * nmemb;
        userdata->buffer().append(data,bytes);
    }
    
    return bytes;
}

void SimpleHttpRequest::requestAsyncCallBack(ccTime dt)
{
    if( !isResponseReady_)
        return;
    
    delegate_->onResponse( !isRequestErrorOccured_, buffer(), this);
    
    CCScheduler::sharedScheduler()->unscheduleSelector(schedule_selector(SimpleHttpRequest::requestAsyncCallBack),this);
}

bool SimpleHttpRequest::start()
{
    CCScheduler::sharedScheduler()->scheduleSelector(schedule_selector(SimpleHttpRequest::requestAsyncCallBack), this, 0, false);
    
    return (pthread_create(&thread_, NULL, SimpleHttpRequest::doRequest, this) == 0);
}

void SimpleHttpRequest::cancel()
{
    if(!cancelled_)
    {
        pthread_mutex_lock(&mutex_);
        
        cancelled_ = true;
        delegate_ = NULL;
        
        pthread_mutex_unlock(&mutex_);
    }
}


