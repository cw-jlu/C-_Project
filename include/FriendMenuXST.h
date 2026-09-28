#ifndef FRIEND_MENU_XST_H
#define FRIEND_MENU_XST_H

#include <string>
#include <vector>

#include "FriendManagerXST.h"
#include "FriendXST.h"
#include "ServiceTypeXST.h"

class AccountXST;
class PlatformXST;
class ServiceXST;
class UserXST;

// 好友管理菜单：以某个用户的身份，在其已开通的服务中维护好友
class FriendMenuXST {
public:
    FriendMenuXST(PlatformXST& platform, const std::string& personId);

    void run();

private:
    AccountXST& currentAccount() const;
    ServiceXST& currentService() const;

    // 从本人已开通的服务中选择一个；excludeCurrent 为 true 时不列出当前服务
    bool chooseService(const std::string& title, bool excludeCurrent, ServiceTypeXST& out) const;

    void switchService();
    void listFriends() const;
    void addFriend();
    void editRemark();
    void removeFriend();
    void searchFriends() const;
    void showCommonWithin() const;
    void showCommonAcross() const;
    void addFromOtherService();

    void printFriendTable(const std::vector<FriendXST>& items) const;
    static std::string describe(const AccountXST& account);   // 如 "QQ 10002(小李飞刀)"

    PlatformXST& m_platform;
    FriendManagerXST m_manager;
    const UserXST* m_user;
    ServiceTypeXST m_current;
};

#endif
