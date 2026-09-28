#include "AccountXST.h"

#include <ostream>
#include <stdexcept>

#include "UserXST.h"

AccountXST::AccountXST(const std::string& id, const UserXST* owner,
                       const std::string& nickname, const DateXST& registerDate)
    : m_id(id), m_owner(owner), m_nickname(nickname), m_registerDate(registerDate) {
    if (m_id.empty()) throw std::invalid_argument("账号 ID 不能为空");
    if (m_owner == nullptr) throw std::invalid_argument("账号必须属于某个用户");
    if (m_nickname.empty()) throw std::invalid_argument("昵称不能为空");
}

const std::string& AccountXST::id() const { return m_id; }
const UserXST& AccountXST::owner() const { return *m_owner; }
const std::string& AccountXST::nickname() const { return m_nickname; }

void AccountXST::setNickname(const std::string& nickname) {
    if (nickname.empty()) throw std::invalid_argument("昵称不能为空");
    m_nickname = nickname;
}

const DateXST& AccountXST::registerDate() const { return m_registerDate; }

int AccountXST::tAge(const DateXST& today) const {
    int years = m_registerDate.fullYearsUntil(today);
    return years < 0 ? 0 : years;
}

std::string AccountXST::serviceName() const {
    return serviceDisplayName(serviceType());
}

bool AccountXST::addFriend(const std::string& friendId, const std::string& remark) {
    if (friendId == m_id || !isValidAccountId(friendId)) return false;
    return m_friends.add(FriendXST(friendId, remark));
}

bool AccountXST::removeFriend(const std::string& friendId) {
    return m_friends.remove(friendId);
}

bool AccountXST::setFriendRemark(const std::string& friendId, const std::string& remark) {
    return m_friends.setRemark(friendId, remark);
}

bool AccountXST::hasFriend(const std::string& friendId) const {
    return m_friends.contains(friendId);
}

const FriendXST* AccountXST::findFriend(const std::string& friendId) const {
    return m_friends.find(friendId);
}

const FriendListXST& AccountXST::friends() const { return m_friends; }

bool AccountXST::joinGroup(int groupId) {
    if (groupId <= 0) return false;
    return m_groups.insert(groupId).second;
}

bool AccountXST::leaveGroup(int groupId) {
    return m_groups.erase(groupId) > 0;
}

bool AccountXST::inGroup(int groupId) const {
    return m_groups.count(groupId) > 0;
}

const std::set<int>& AccountXST::groups() const { return m_groups; }

void AccountXST::printInfo(std::ostream& os, const DateXST& today) const {
    os << "[" << serviceName() << "] " << idKindName() << ": " << m_id
       << "  昵称: " << m_nickname << "\n";
    os << "  所属用户: " << m_owner->name() << " (" << m_owner->id() << ")"
       << "  出生: " << m_owner->birthday()
       << "  所在地: " << m_owner->location() << "\n";
    os << "  申请时间: " << m_registerDate << "  T龄: " << tAge(today) << " 年\n";
    printExtraInfo(os);
    os << "  好友: " << m_friends.size() << " 人  群: " << m_groups.size() << " 个";
    if (!m_groups.empty()) {
        os << " (";
        bool first = true;
        for (int g : m_groups) {
            os << (first ? "" : ", ") << g;
            first = false;
        }
        os << ")";
    }
    os << "\n";
}

void AccountXST::printExtraInfo(std::ostream&) const {}
