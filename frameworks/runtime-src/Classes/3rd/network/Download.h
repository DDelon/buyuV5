//
//  Download.hpp
//  MaJiang
//
//  Created by weile on 16/8/26.
//
//

#ifndef Download_hpp
#define Download_hpp

#include <string>
#include <map>
#include "network/CCDownloader.h"

namespace download
{
    typedef unsigned long long uint64_t;
    typedef std::vector<unsigned char> Data;
    class DownloadInfo
    {
    public:
        DownloadInfo(int Id, bool result, const std::string& url)
            :_uniqueId(Id) , _result(result) , _url(url)
        {}
        
        void setCode(int code)  { _code = code; }
        void setErrorMsg(const std::string& msg)    { _errorMsg = msg; }
        void setData(Data* data)  { _data = data; }
        
        const int getId()         { return _uniqueId; }
        const int getCode()       { return _code; }
        const std::string& getErrorMsg() { return _errorMsg; }
        const Data* getData()     { return _data; }
        const bool getResult()    { return _result;   }
        const std::string& getUrl() { return _url; }
        
    private:
        int         _uniqueId = 0;
        bool        _result = false;
        std::string _url;
        
        int         _code = 0;
        std::string _errorMsg;
        Data*       _data = nullptr;
        
    };
    
    typedef std::function<void(DownloadInfo&)> Callback;
    class Download
    {
    public:
        static Download& getInstance();
        
        uint64_t start(const std::string& url, const Callback& callback);
        
        /*
         * 描述：取消请求下载
         * 参数：Id由start()返回
         */
        void cancel(uint64_t Id);
        
    private:
        Download();
        
    private:
        uint64_t _id = 0;
        std::map<int, Callback> _callbackMap;
        std::unique_ptr<cocos2d::network::Downloader> _downloader;
        
    };
}


#endif /* Download_hpp */
