#ifndef HOME_MENU_XST_H
#define HOME_MENU_XST_H

#include <string>

class LoginManagerXST;
class PlatformXST;

// 登录后的个人主页：进入好友管理、群管理、开通管理，查看/确认各服务登录状态，查看个人资料
class HomeMenuXST {
public:
    HomeMenuXST(PlatformXST& platform, LoginManagerXST& session);

    void run();                       // 选择“注销”后返回

private:
    void manageLogin();               // 确认登录其他已开通的服务
    void showProfile() const;
    std::string statusLine() const;   // 如 "已登录: QQ、微信  待确认: 微博"

    PlatformXST& m_platform;
    LoginManagerXST& m_session;
};

#endif
