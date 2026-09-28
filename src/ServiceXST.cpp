#include "ServiceXST.h"

#include <stdexcept>

#include "UserXST.h"

std::string ServiceXST::name() const {
    return serviceDisplayName(type());
}

AccountXST& ServiceXST::addAccount(std::unique_ptr<AccountXST> account) {
    if (!account) throw std::invalid_argument("账号为空");
    if (account->serviceType() != type()) {
        throw std::invalid_argument("账号类型与服务不符: " + account->id());
    }
    if (m_accounts.count(account->id())) {
        throw std::invalid_argument(name() + " 账号已存在: " + account->id());
    }
    if (findAccountByOwner(account->owner().id())) {
        throw std::invalid_argument(account->owner().name() + " 已开通" + name());
    }
    AccountXST& ref = *account;
    m_accounts[account->id()] = std::move(account);
    return ref;
}

AccountXST* ServiceXST::findAccount(const std::string& id) {
    auto it = m_accounts.find(id);
    return it == m_accounts.end() ? nullptr : it->second.get();
}

const AccountXST* ServiceXST::findAccount(const std::string& id) const {
    auto it = m_accounts.find(id);
    return it == m_accounts.end() ? nullptr : it->second.get();
}

AccountXST* ServiceXST::findAccountByOwner(const std::string& personId) {
    for (auto& entry : m_accounts) {
        if (entry.second->owner().id() == personId) return entry.second.get();
    }
    return nullptr;
}

const AccountXST* ServiceXST::findAccountByOwner(const std::string& personId) const {
    for (const auto& entry : m_accounts) {
        if (entry.second->owner().id() == personId) return entry.second.get();
    }
    return nullptr;
}

const AccountXST* ServiceXST::findAccountByQQ(const std::string& qq) const {
    if (qq.empty()) return nullptr;
    for (const auto& entry : m_accounts) {
        if (entry.second->linkedQQ() == qq) return entry.second.get();
    }
    return nullptr;
}

std::vector<const AccountXST*> ServiceXST::accounts() const {
    std::vector<const AccountXST*> result;
    result.reserve(m_accounts.size());
    for (const auto& entry : m_accounts) result.push_back(entry.second.get());
    return result;
}

size_t ServiceXST::accountCount() const { return m_accounts.size(); }

FriendOpResultXST ServiceXST::addFriendship(const std::string& a, const std::string& b,
                                            const std::string& remarkByA,
                                            const std::string& remarkByB) {
    AccountXST* first = findAccount(a);
    if (!first) return FriendOpResultXST::AccountNotFound;
    if (a == b) return FriendOpResultXST::SelfNotAllowed;
    if (!first->isValidAccountId(b)) return FriendOpResultXST::InvalidId;
    AccountXST* second = findAccount(b);
    if (!second) return FriendOpResultXST::FriendNotFound;
    if (first->hasFriend(b) && second->hasFriend(a)) return FriendOpResultXST::AlreadyFriends;

    // 任意一侧缺失都补齐，保证关系双向一致
    if (!first->hasFriend(b)) first->addFriend(b, remarkByA);
    if (!second->hasFriend(a)) second->addFriend(a, remarkByB);
    return FriendOpResultXST::Ok;
}

FriendOpResultXST ServiceXST::removeFriendship(const std::string& a, const std::string& b) {
    AccountXST* first = findAccount(a);
    if (!first) return FriendOpResultXST::AccountNotFound;
    if (!first->removeFriend(b)) return FriendOpResultXST::NotFriends;
    if (AccountXST* second = findAccount(b)) second->removeFriend(a);
    return FriendOpResultXST::Ok;
}

FriendOpResultXST ServiceXST::setFriendRemark(const std::string& owner, const std::string& friendId,
                                              const std::string& remark) {
    AccountXST* account = findAccount(owner);
    if (!account) return FriendOpResultXST::AccountNotFound;
    return account->setFriendRemark(friendId, remark) ? FriendOpResultXST::Ok
                                                      : FriendOpResultXST::NotFriends;
}

std::vector<const AccountXST*> ServiceXST::commonFriends(const std::string& a,
                                                         const std::string& b) const {
    std::vector<const AccountXST*> result;
    const AccountXST* first = findAccount(a);
    const AccountXST* second = findAccount(b);
    if (!first || !second) return result;
    for (const FriendXST& f : first->friends().items()) {
        if (second->hasFriend(f.id())) {
            if (const AccountXST* common = findAccount(f.id())) result.push_back(common);
        }
    }
    return result;
}
