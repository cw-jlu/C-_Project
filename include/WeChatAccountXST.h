#ifndef WECHAT_ACCOUNT_XST_H
#define WECHAT_ACCOUNT_XST_H

#include "BindableAccountXST.h"

// 微信账号：独立微信号，可与 QQ 号绑定
class WeChatAccountXST : public BindableAccountXST {
public:
    WeChatAccountXST(const std::string& wxId, const UserXST* owner,
                     const std::string& nickname, const DateXST& registerDate,
                     const std::string& boundQQ = "");

    ServiceTypeXST serviceType() const override;
    std::string idKindName() const override;
};

#endif
