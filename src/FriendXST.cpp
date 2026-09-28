#include "FriendXST.h"

#include <stdexcept>

FriendXST::FriendXST(const std::string& id, const std::string& remark)
    : m_id(id), m_remark(remark) {
    if (m_id.empty()) throw std::invalid_argument("好友 ID 不能为空");
}

const std::string& FriendXST::id() const { return m_id; }
const std::string& FriendXST::remark() const { return m_remark; }
void FriendXST::setRemark(const std::string& remark) { m_remark = remark; }

std::string FriendXST::displayName() const {
    return m_remark.empty() ? m_id : m_remark;
}
