#include "FriendListXST.h"

#include <algorithm>

std::vector<FriendXST>::iterator FriendListXST::locate(const std::string& id) {
    return std::find_if(m_items.begin(), m_items.end(),
                        [&id](const FriendXST& f) { return f.id() == id; });
}

std::vector<FriendXST>::const_iterator FriendListXST::locate(const std::string& id) const {
    return std::find_if(m_items.begin(), m_items.end(),
                        [&id](const FriendXST& f) { return f.id() == id; });
}

bool FriendListXST::add(const FriendXST& item) {
    if (contains(item.id())) return false;
    m_items.push_back(item);
    return true;
}

bool FriendListXST::remove(const std::string& id) {
    auto it = locate(id);
    if (it == m_items.end()) return false;
    m_items.erase(it);
    return true;
}

bool FriendListXST::setRemark(const std::string& id, const std::string& remark) {
    auto it = locate(id);
    if (it == m_items.end()) return false;
    it->setRemark(remark);
    return true;
}

bool FriendListXST::contains(const std::string& id) const {
    return locate(id) != m_items.end();
}

const FriendXST* FriendListXST::find(const std::string& id) const {
    auto it = locate(id);
    return it == m_items.end() ? nullptr : &*it;
}

std::vector<FriendXST> FriendListXST::search(const std::string& keyword) const {
    std::vector<FriendXST> result;
    for (const FriendXST& f : m_items) {
        if (f.id().find(keyword) != std::string::npos ||
            f.remark().find(keyword) != std::string::npos) {
            result.push_back(f);
        }
    }
    return result;
}

std::vector<std::string> FriendListXST::ids() const {
    std::vector<std::string> result;
    result.reserve(m_items.size());
    for (const FriendXST& f : m_items) result.push_back(f.id());
    return result;
}

const std::vector<FriendXST>& FriendListXST::items() const { return m_items; }
size_t FriendListXST::size() const { return m_items.size(); }
bool FriendListXST::empty() const { return m_items.empty(); }
