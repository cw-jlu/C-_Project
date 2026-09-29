#ifndef OPEN_SERVICE_MENU_XST_H
#define OPEN_SERVICE_MENU_XST_H

#include <string>

#include "ServiceTypeXST.h"

class AccountXST;
class LoginManagerXST;
class PlatformXST;

// 开通管理：用户可自行选择开通平台上的 N 个“微X”服务
// 账号 ID 按各服务的 idRule() 产生：QQ 分配新号、微博沿用本人 QQ 号、微信自定义并可绑定 QQ
class OpenServiceMenuXST {
public:
    OpenServiceMenuXST(PlatformXST& platform, LoginManagerXST& session);

    // 登录后使用：查看各服务开通情况并开通新服务，开通后简单确认即可登录
    void run();

    // 交互式为某用户开通一个服务（注册新用户时也会调用）；成功返回新账号，失败返回 nullptr
    static const AccountXST* openInteractive(PlatformXST& platform, const std::string& personId,
                                             ServiceTypeXST type);

private:
    PlatformXST& m_platform;
    LoginManagerXST& m_session;
};

#endif
