#ifndef MAIN_MENU_XST_H
#define MAIN_MENU_XST_H

#include "LoginManagerXST.h"

class PlatformXST;

// 平台入口菜单：登录、注册新用户、功能展示、平台概览、退出
class MainMenuXST {
public:
    explicit MainMenuXST(PlatformXST& platform);

    void run();

private:
    void login();
    void registerUser();
    void showOverview() const;

    PlatformXST& m_platform;
    LoginManagerXST m_session;
};

#endif
