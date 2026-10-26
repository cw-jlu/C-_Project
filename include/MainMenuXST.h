#ifndef MAIN_MENU_XST_H
#define MAIN_MENU_XST_H

#include "LoginManagerXST.h"

class PlatformXST;

// 平台入口：先选择微X服务，再进入该服务的登录、注册等菜单
class MainMenuXST {
public:
    explicit MainMenuXST(PlatformXST& platform);

    void run();

private:
    void runService(ServiceTypeXST type);
    void login(ServiceTypeXST type);
    void registerUser(ServiceTypeXST type);
    void showOverview() const;

    PlatformXST& m_platform;
    LoginManagerXST m_session;
};

#endif
