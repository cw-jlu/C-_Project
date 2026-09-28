#include "BindableAccountXST.h"

#include <cctype>
#include <ostream>
#include <stdexcept>

#include "QQIdAccountXST.h"

namespace {
const std::string& requireValidIndependentId(const std::string& id) {
    if (!BindableAccountXST::isValidIndependentId(id)) {
        throw std::invalid_argument("ID 格式错误（6~20 位，字母开头，仅含字母/数字/_/-）: " + id);
    }
    return id;
}
}

BindableAccountXST::BindableAccountXST(const std::string& id, const UserXST* owner,
                                       const std::string& nickname, const DateXST& registerDate,
                                       const std::string& boundQQ)
    : AccountXST(requireValidIndependentId(id), owner, nickname, registerDate) {
    if (!boundQQ.empty() && !bindQQ(boundQQ)) {
        throw std::invalid_argument("绑定的 QQ 号格式错误: " + boundQQ);
    }
}

bool BindableAccountXST::isValidIndependentId(const std::string& id) {
    if (id.size() < 6 || id.size() > 20) return false;
    if (!std::isalpha(static_cast<unsigned char>(id[0]))) return false;
    for (char c : id) {
        unsigned char u = static_cast<unsigned char>(c);
        if (!std::isalnum(u) && c != '_' && c != '-') return false;
    }
    return true;
}

bool BindableAccountXST::isValidAccountId(const std::string& id) const {
    return isValidIndependentId(id);
}

std::string BindableAccountXST::linkedQQ() const { return m_boundQQ; }

bool BindableAccountXST::bindQQ(const std::string& qq) {
    if (!QQIdAccountXST::isValidQQ(qq)) return false;
    m_boundQQ = qq;
    return true;
}

bool BindableAccountXST::unbindQQ() {
    if (m_boundQQ.empty()) return false;
    m_boundQQ.clear();
    return true;
}

bool BindableAccountXST::isBound() const { return !m_boundQQ.empty(); }
const std::string& BindableAccountXST::boundQQ() const { return m_boundQQ; }

void BindableAccountXST::printExtraInfo(std::ostream& os) const {
    os << "  ID体系: 独立ID  绑定QQ: " << (isBound() ? m_boundQQ : "未绑定") << "\n";
}
