#include "SubGroupXST.h"

#include <algorithm>
#include <stdexcept>

SubGroupXST::SubGroupXST(const std::string& name, const std::string& creatorId,
                         const std::vector<std::string>& members)
    : m_name(name), m_creatorId(creatorId), m_members(members) {
    if (m_name.empty()) throw std::invalid_argument("讨论组名称不能为空");
    if (!hasMember(m_creatorId)) m_members.insert(m_members.begin(), m_creatorId);
}

const std::string& SubGroupXST::name() const { return m_name; }
const std::string& SubGroupXST::creatorId() const { return m_creatorId; }
const std::vector<std::string>& SubGroupXST::members() const { return m_members; }

bool SubGroupXST::hasMember(const std::string& id) const {
    return std::find(m_members.begin(), m_members.end(), id) != m_members.end();
}

bool SubGroupXST::removeMember(const std::string& id) {
    auto it = std::find(m_members.begin(), m_members.end(), id);
    if (it == m_members.end()) return false;
    m_members.erase(it);
    return true;
}

std::string SubGroupXST::membersText() const {
    std::string text;
    for (const std::string& id : m_members) {
        if (!text.empty()) text += ", ";
        text += id;
    }
    return text;
}
