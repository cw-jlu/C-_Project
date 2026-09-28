#include "QQAccountXST.h"

QQAccountXST::QQAccountXST(const std::string& qq, const UserXST* owner,
                           const std::string& nickname, const DateXST& registerDate)
    : QQIdAccountXST(qq, owner, nickname, registerDate) {}

ServiceTypeXST QQAccountXST::serviceType() const { return ServiceTypeXST::QQ; }
