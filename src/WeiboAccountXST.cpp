#include "WeiboAccountXST.h"

WeiboAccountXST::WeiboAccountXST(const std::string& qq, const UserXST* owner,
                                 const std::string& nickname, const DateXST& registerDate)
    : QQIdAccountXST(qq, owner, nickname, registerDate) {}

ServiceTypeXST WeiboAccountXST::serviceType() const { return ServiceTypeXST::Weibo; }
