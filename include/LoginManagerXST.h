#ifndef LOGIN_MANAGER_XST_H
#define LOGIN_MANAGER_XST_H

#include <set>
#include <string>
#include <vector>

#include "LoginResultXST.h"
#include "ServiceTypeXST.h"

class PlatformXST;
class UserXST;

// 登录管理（单一会话）：
// 用账号密码登录任一服务后，本人开通的其他服务进入“待确认”状态，
// 简单确认（无需再次输入密码）即视为自动登录；注销时所有服务一并退出
class LoginManagerXST {
public:
    explicit LoginManagerXST(PlatformXST& platform);

    // 以某服务的账号密码登录
    LoginResultXST login(ServiceTypeXST type, const std::string& accountId,
                         const std::string& password);
    // 确认登录本人已开通的另一服务（无需密码）
    LoginResultXST confirm(ServiceTypeXST type);
    // 确认所有待确认的服务，返回本次新登录的服务
    std::vector<ServiceTypeXST> confirmAll();
    void logout();

    bool isActive() const;                            // 是否有用户登录
    const UserXST* currentUser() const;
    ServiceTypeXST primaryService() const;            // 用密码登录的服务
    bool isLoggedIn(ServiceTypeXST type) const;
    std::vector<ServiceTypeXST> loggedInServices() const;
    std::vector<ServiceTypeXST> pendingServices() const;  // 已开通、尚未确认登录

private:
    PlatformXST& m_platform;
    const UserXST* m_user;
    ServiceTypeXST m_primary;
    std::set<ServiceTypeXST> m_loggedIn;
};

#endif
