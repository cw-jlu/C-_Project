#ifndef FRIEND_MENU_XST_H
#define FRIEND_MENU_XST_H

#include <string>
#include <vector>

#include "FriendManagerXST.h"
#include "FriendXST.h"
#include "ServiceMenuXST.h"

// 好友管理菜单：在当前服务中维护好友，并支持跨服务的共同好友与推荐
class FriendMenuXST : public ServiceMenuXST {
public:
    FriendMenuXST(PlatformXST& platform, const std::string& personId);

protected:
    std::string title() const override;
    std::vector<std::string> items() const override;
    void handle(int index) override;

private:
    void listFriends() const;
    void addFriend();
    void editRemark();
    void removeFriend();
    void searchFriends() const;
    void showCommonWithin() const;
    void showCommonAcross() const;
    void addFromOtherService();

    void printFriendTable(const std::vector<FriendXST>& items) const;

    FriendManagerXST m_manager;
};

#endif
