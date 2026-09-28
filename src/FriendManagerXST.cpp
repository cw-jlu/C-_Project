#include "FriendManagerXST.h"

#include "PlatformXST.h"

FriendManagerXST::FriendManagerXST(PlatformXST& platform) : m_platform(platform) {}

const AccountXST* FriendManagerXST::counterpartOf(const AccountXST& account,
                                                  ServiceTypeXST target) const {
    if (!m_platform.hasService(target)) return nullptr;
    if (account.serviceType() == target) return &account;
    return m_platform.service(target).findAccountByQQ(account.linkedQQ());
}

std::vector<AccountLinkXST> FriendManagerXST::commonFriendsAcross(const std::string& personId,
                                                                  ServiceTypeXST a,
                                                                  ServiceTypeXST b) const {
    std::vector<AccountLinkXST> result;
    const AccountXST* mineA = m_platform.accountOf(personId, a);
    const AccountXST* mineB = m_platform.accountOf(personId, b);
    if (!mineA || !mineB || a == b) return result;

    const ServiceXST& serviceA = m_platform.service(a);
    for (const FriendXST& f : mineA->friends().items()) {
        const AccountXST* friendA = serviceA.findAccount(f.id());
        if (!friendA) continue;
        const AccountXST* friendB = counterpartOf(*friendA, b);
        if (friendB && mineB->hasFriend(friendB->id())) {
            result.emplace_back(*friendA, *friendB);
        }
    }
    return result;
}

std::vector<AccountLinkXST> FriendManagerXST::recommend(const std::string& personId,
                                                        ServiceTypeXST target,
                                                        ServiceTypeXST source) const {
    std::vector<AccountLinkXST> result;
    const AccountXST* mineTarget = m_platform.accountOf(personId, target);
    const AccountXST* mineSource = m_platform.accountOf(personId, source);
    if (!mineTarget || !mineSource || target == source) return result;

    const ServiceXST& sourceService = m_platform.service(source);
    for (const FriendXST& f : mineSource->friends().items()) {
        const AccountXST* friendSource = sourceService.findAccount(f.id());
        if (!friendSource) continue;
        const AccountXST* candidate = counterpartOf(*friendSource, target);
        if (candidate && candidate != mineTarget && !mineTarget->hasFriend(candidate->id())) {
            result.emplace_back(*friendSource, *candidate);
        }
    }
    return result;
}

FriendOpResultXST FriendManagerXST::acceptRecommendation(const std::string& personId,
                                                         ServiceTypeXST target,
                                                         const AccountLinkXST& link) {
    const AccountXST* mineTarget = m_platform.accountOf(personId, target);
    if (!mineTarget) return FriendOpResultXST::AccountNotFound;

    // 沿用本人在来源服务中给对方的备注
    std::string remark;
    const AccountXST* mineSource = m_platform.accountOf(personId, link.source().serviceType());
    if (mineSource) {
        if (const FriendXST* f = mineSource->findFriend(link.source().id())) remark = f->remark();
    }
    return m_platform.service(target).addFriendship(mineTarget->id(), link.target().id(), remark);
}
