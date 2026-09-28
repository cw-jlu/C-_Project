#ifndef FRIEND_MANAGER_XST_H
#define FRIEND_MANAGER_XST_H

#include <string>
#include <vector>

#include "AccountLinkXST.h"
#include "FriendOpResultXST.h"
#include "ServiceTypeXST.h"

class AccountXST;
class PlatformXST;

// 跨服务好友管理
// 不同服务间以 QQ 号作为账号对应的纽带：QQ/微博为本号，微信为绑定的 QQ（未绑定则无法对应）
class FriendManagerXST {
public:
    explicit FriendManagerXST(PlatformXST& platform);

    // account 在 target 服务中对应的账号，找不到返回 nullptr
    const AccountXST* counterpartOf(const AccountXST& account, ServiceTypeXST target) const;

    // 共同好友：在 A、B 两个服务中都与本人是好友的人
    // 返回 (A 中的好友账号 -> B 中对应的好友账号)
    std::vector<AccountLinkXST> commonFriendsAcross(const std::string& personId,
                                                    ServiceTypeXST a, ServiceTypeXST b) const;

    // 推荐好友：本人在 source 服务中的好友，若在 target 服务有对应账号且尚未成为好友则推荐
    // 返回 (source 中的好友账号 -> target 中可添加的账号)
    std::vector<AccountLinkXST> recommend(const std::string& personId,
                                          ServiceTypeXST target, ServiceTypeXST source) const;

    // 添加一条推荐：在 target 服务中与对方互加好友，并沿用 source 服务中的备注
    FriendOpResultXST acceptRecommendation(const std::string& personId, ServiceTypeXST target,
                                           const AccountLinkXST& link);

private:
    PlatformXST& m_platform;
};

#endif
