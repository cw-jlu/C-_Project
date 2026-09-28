#include "WeChatAccountXST.h"

WeChatAccountXST::WeChatAccountXST(const std::string& wxId, const UserXST* owner,
                                   const std::string& nickname, const DateXST& registerDate,
                                   const std::string& boundQQ)
    : BindableAccountXST(wxId, owner, nickname, registerDate, boundQQ) {}

ServiceTypeXST WeChatAccountXST::serviceType() const { return ServiceTypeXST::WeChat; }

std::string WeChatAccountXST::idKindName() const { return "Î¢ÐÅºÅ"; }
