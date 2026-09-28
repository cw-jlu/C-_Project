#include "QQIdAccountXST.h"

#include <ostream>
#include <stdexcept>

namespace {
// 在进入基类构造之前完成 ID 校验
const std::string& requireValidQQ(const std::string& qq) {
    if (!QQIdAccountXST::isValidQQ(qq)) {
        throw std::invalid_argument("QQ 号格式错误（5~11 位数字，首位非 0）: " + qq);
    }
    return qq;
}
}

QQIdAccountXST::QQIdAccountXST(const std::string& qq, const UserXST* owner,
                               const std::string& nickname, const DateXST& registerDate)
    : AccountXST(requireValidQQ(qq), owner, nickname, registerDate) {}

bool QQIdAccountXST::isValidQQ(const std::string& qq) {
    if (qq.size() < 5 || qq.size() > 11 || qq[0] == '0') return false;
    for (char c : qq) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

std::string QQIdAccountXST::idKindName() const { return "QQ号"; }

bool QQIdAccountXST::isValidAccountId(const std::string& id) const {
    return isValidQQ(id);
}

std::string QQIdAccountXST::linkedQQ() const { return id(); }

void QQIdAccountXST::printExtraInfo(std::ostream& os) const {
    os << "  ID体系: QQ号体系\n";
}
