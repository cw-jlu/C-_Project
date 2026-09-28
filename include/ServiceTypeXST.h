#ifndef SERVICE_TYPE_XST_H
#define SERVICE_TYPE_XST_H

#include <string>
#include <vector>

// 平台上的“微X”服务类型
// 新增服务时：在此处加枚举值，并补全下面三个转换函数
enum class ServiceTypeXST {
    QQ,
    WeChat,
    Weibo
};

// 数据文件中使用的服务代码：QQ / WX / WB
inline std::string serviceCode(ServiceTypeXST type) {
    switch (type) {
        case ServiceTypeXST::QQ:     return "QQ";
        case ServiceTypeXST::WeChat: return "WX";
        case ServiceTypeXST::Weibo:  return "WB";
    }
    return "";
}

// 界面上显示的服务名称
inline std::string serviceDisplayName(ServiceTypeXST type) {
    switch (type) {
        case ServiceTypeXST::QQ:     return "QQ";
        case ServiceTypeXST::WeChat: return "微信";
        case ServiceTypeXST::Weibo:  return "微博";
    }
    return "";
}

// 由服务代码解析服务类型，未知代码返回 false
inline bool parseServiceCode(const std::string& code, ServiceTypeXST& out) {
    if (code == "QQ") { out = ServiceTypeXST::QQ;     return true; }
    if (code == "WX") { out = ServiceTypeXST::WeChat; return true; }
    if (code == "WB") { out = ServiceTypeXST::Weibo;  return true; }
    return false;
}

inline const std::vector<ServiceTypeXST>& allServiceTypes() {
    static const std::vector<ServiceTypeXST> types = {
        ServiceTypeXST::QQ, ServiceTypeXST::WeChat, ServiceTypeXST::Weibo
    };
    return types;
}

#endif
