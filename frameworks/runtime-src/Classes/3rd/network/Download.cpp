//
//  Download.cpp
//  MaJiang
//
//  Created by weile on 16/8/26.
//
//

#include <sstream>

#include "Download.h"

#include "cocos2d.h"
using namespace cocos2d;
namespace download
{
    Download& Download::getInstance()
    {
        static Download client;
        return client;
    }
    
    Download::Download()
    {
        _downloader.reset(new cocos2d::network::Downloader());
        _downloader->onTaskProgress = [this](const network::DownloadTask& task,
                                             int64_t bytesReceived,
                                             int64_t totalBytesReceived,
                                             int64_t totalBytesExpected)
        {
//            CCLOG("dowloading = %lld", bytesReceived);
        };
        
        _downloader->onDataTaskSuccess = [this](const network::DownloadTask& task,
                                               std::vector<unsigned char>& data)
        {
            auto id = task.identifier;
            int intId = atoi(id.c_str());
            bool result = true;
            DownloadInfo info(intId, result, task.requestURL);
            info.setData(&data);

            auto callback = _callbackMap[intId];
            callback(info);

            _callbackMap[intId] = nullptr;
        };
        _downloader->onTaskError = [this](const network::DownloadTask& task,
                                          int errorCode,
                                          int errorCodeInternal,
                                          const std::string& errorStr)
        {
            auto id = task.identifier;
            int intId = atoi(id.c_str());
            bool result = false;
            DownloadInfo info(intId, result, task.requestURL);
            info.setCode(errorCode);
            info.setErrorMsg(errorStr);
            
            auto callback = _callbackMap[intId];
            callback(info);
            _callbackMap[intId] = nullptr;
        };
    }
    
    uint64_t Download::start(const std::string& url, const Callback& callback)
    {
        auto newId = ++_id;
        _callbackMap[newId] = callback;
        
        std::stringstream ss;
        ss << newId;
        std::string key = ss.str();
        _downloader->createDownloadDataTask(url, key);
        return newId;
    }
}