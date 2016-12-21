/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.
 
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

#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include <string>
#include <vector>

namespace network {
    
    class HttpClientEx;
    class HttpResponseEx;
    
    typedef std::function<void(HttpClientEx* client, HttpResponseEx* response)> ccHttpRequestCallback;
    typedef void (cocos2d::Ref::*SEL_HttpResponse)(HttpClientEx* client, HttpResponseEx* response);
#define httpresponse_selector(_SELECTOR) (cocos2d::network::SEL_HttpResponse)(&_SELECTOR)
    
    /**
     @brief defines the object which users must packed for HttpClient::send(HttpRequest*) method.
     Please refer to tests/test-cpp/Classes/ExtensionTest/NetworkTest/HttpClientTest.cpp as a sample
     @since v2.0.2
     */
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
#ifdef DELETE
#undef DELETE
#endif
#endif
    
    class HttpRequestEx : public Ref
    {
    public:
        /** Use this enum type as param in setReqeustType(param) */
        enum class Type
        {
            GET,
            POST,
            PUT,
            DELETE,
            PostFile,
            UNKNOWN,
        };
        
        /** Constructor
         Because HttpRequest object will be used between UI thead and network thread,
         requestObj->autorelease() is forbidden to avoid crashes in AutoreleasePool
         new/retain/release still works, which means you need to release it manually
         Please refer to HttpRequestTest.cpp to find its usage
         */
        HttpRequestEx()
        {
            _requestType = Type::UNKNOWN;
            _url.clear();
            _requestData.clear();
            _tag.clear();
            _pTarget = nullptr;
            _pSelector = nullptr;
            _pCallback = nullptr;
            _pUserData = nullptr;
        };
        
        /** Destructor */
        virtual ~HttpRequestEx()
        {
            if (_pTarget)
            {
                _pTarget->release();
            }
        };
        
        /** Override autorelease method to avoid developers to call it */
        Ref* autorelease(void)
        {
            CCASSERT(false, "HttpResponse is used between network thread and ui thread \
                     therefore, autorelease is forbidden here");
            return NULL;
        }
        
        // setter/getters for properties
        
        /** Required field for HttpRequest object before being sent.
         kHttpGet & kHttpPost is currently supported
         */
        inline void setRequestType(Type type)
        {
            _requestType = type;
        };
        /** Get back the kHttpGet/Post/... enum value */
        inline Type getRequestType()
        {
            return _requestType;
        };
        
        /** Required field for HttpRequest object before being sent.
         */
        inline void setUrl(const char* url)
        {
            _url = url;
        };
        /** Get back the setted url */
        inline const char* getUrl()
        {
            return _url.c_str();
        };
        
        /** Option field. You can set your post data here
         */
        inline void setRequestData(const char* buffer, size_t len)
        {
            _requestData.assign(buffer, buffer + len);
        };
        /** Get the request data pointer back */
        inline char* getRequestData()
        {
            if(_requestData.size() != 0)
                return &(_requestData.front());
            
            return nullptr;
        }
        /** Get the size of request data back */
        inline ssize_t getRequestDataSize()
        {
            return _requestData.size();
        }
        
        /** Option field. You can set a string tag to identify your request, this tag can be found in HttpResponse->getHttpRequest->getTag()
         */
        inline void setTag(const char* tag)
        {
            _tag = tag;
        };
        /** Get the string tag back to identify the request.
         The best practice is to use it in your MyClass::onMyHttpRequestCompleted(sender, HttpResponse*) callback
         */
        inline const char* getTag()
        {
            return _tag.c_str();
        };
        
        /** Option field. You can attach a customed data in each request, and get it back in response callback.
         But you need to new/delete the data pointer manully
         */
        inline void setUserData(void* pUserData)
        {
            _pUserData = pUserData;
        };
        /** Get the pre-setted custom data pointer back.
         Don't forget to delete it. HttpClient/HttpResponse/HttpRequest will do nothing with this pointer
         */
        inline void* getUserData()
        {
            return _pUserData;
        };
        
        /** Required field. You should set the callback selector function at ack the http request completed
         */
        CC_DEPRECATED_ATTRIBUTE inline void setResponseCallback(Ref* pTarget, SEL_CallFuncND pSelector)
        {
            setResponseCallback(pTarget, (SEL_HttpResponse) pSelector);
        }
        
        CC_DEPRECATED_ATTRIBUTE inline void setResponseCallback(Ref* pTarget, SEL_HttpResponse pSelector)
        {
            _pTarget = pTarget;
            _pSelector = pSelector;
            
            if (_pTarget)
            {
                _pTarget->retain();
            }
        }
        
        inline void setResponseCallback(const ccHttpRequestCallback& callback)
        {
            _pCallback = callback;
        }
        
        /** Get the target of callback selector funtion, mainly used by HttpClient */
        inline Ref* getTarget()
        {
            return _pTarget;
        }
        
        /* This sub class is just for migration SEL_CallFuncND to SEL_HttpResponse,
         someday this way will be removed */
        class _prxy
        {
        public:
            _prxy( SEL_HttpResponse cb ) :_cb(cb) {}
            ~_prxy(){};
            operator SEL_HttpResponse() const { return _cb; }
            CC_DEPRECATED_ATTRIBUTE operator SEL_CallFuncND()   const { return (SEL_CallFuncND) _cb; }
        protected:
            SEL_HttpResponse _cb;
        };
        
        /** Get the selector function pointer, mainly used by HttpClient */
        inline _prxy getSelector()
        {
            return _prxy(_pSelector);
        }
        
        inline const ccHttpRequestCallback& getCallback()
        {
            return _pCallback;
        }
        
        /** Set any custom headers **/
        inline void setHeaders(std::vector<std::string> pHeaders)
        {
            _headers=pHeaders;
        }
        
        /** Get custom headers **/
        inline std::vector<std::string> getHeaders()
        {
            return _headers;
        }
        
        std::map<std::string, std::string> getFileData(){
            return _pFileData;
        }
        
        void addRequestParams(std::map<std::string, std::string> params){
            _pFileData.insert(params.begin(), params.end());
        }
        
        void setFile(const char *filePath, const char *contentType, const char *fileName)
        {
            _pFileData.insert(std::pair<std::string, std::string>("fileName", fileName));
            _pFileData.insert(std::pair<std::string, std::string>("filePath", filePath));
            _pFileData.insert(std::pair<std::string, std::string>("contentType", contentType));
        }
    protected:
        // properties
        Type                        _requestType;    /// kHttpRequestGet, kHttpRequestPost or other enums
        std::string                 _url;            /// target url that this request is sent to
        std::vector<char>           _requestData;    /// used for POST
        std::string                 _tag;            /// user defined tag, to identify different requests in response callback
        Ref*                        _pTarget;        /// callback target of pSelector function
        SEL_HttpResponse            _pSelector;      /// callback function, e.g. MyLayer::onHttpResponse(HttpClient *sender, HttpResponse * response)
        ccHttpRequestCallback       _pCallback;      /// C++11 style callbacks
        void*                       _pUserData;      /// You can add your customed data here 
        std::vector<std::string>    _headers;		      /// custom http headers
        std::map<std::string, std::string> _pFileData;
    };
    
}

NS_CC_END

#endif //__HTTP_REQUEST_H__
