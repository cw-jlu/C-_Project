#ifndef SERVICE_MENU_XST_H
#define SERVICE_MENU_XST_H

#include <string>
#include <vector>

#include "ServiceTypeXST.h"

class AccountXST;
class LoginManagerXST;
class PlatformXST;
class ServiceXST;
class UserXST;

// 服务菜单抽象基类：以当前登录用户的身份，在其某个已登录的服务中进行操作
// run() 为模板方法：统一负责菜单循环，第 1 项固定为“切换服务”，其余选项由子类提供
class ServiceMenuXST {
public:
    ServiceMenuXST(PlatformXST& platform, LoginManagerXST& session);
    virtual ~ServiceMenuXST() = default;

    void run();

protected:
    virtual std::string title() const = 0;                  // 如 "好友管理"
    virtual std::vector<std::string> items() const = 0;     // 第 2 项起的菜单文字
    virtual void handle(int index) = 0;                     // index 对应 items() 下标

    PlatformXST& platform() const;
    LoginManagerXST& session() const;
    const UserXST& user() const;
    ServiceTypeXST currentType() const;
    ServiceXST& currentService() const;
    AccountXST& currentAccount() const;

    // 从本人已开通的服务中选择一个（显示登录状态）；excludeCurrent 为 true 时不列出当前服务
    bool chooseService(const std::string& prompt, bool excludeCurrent, ServiceTypeXST& out) const;

    static std::string describe(const AccountXST& account);  // 如 "QQ 10002(小李飞刀)"

private:
    void switchService();                                     // 切到未登录的服务需先确认登录

    PlatformXST& m_platform;
    LoginManagerXST& m_session;
    ServiceTypeXST m_current;
};

#endif
