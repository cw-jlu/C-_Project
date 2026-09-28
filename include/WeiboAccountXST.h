#ifndef WEIBO_ACCOUNT_XST_H
#define WEIBO_ACCOUNT_XST_H

#include "QQIdAccountXST.h"

// Œ¢≤©’À∫≈£∫”Î QQ π≤œÌ ID
class WeiboAccountXST : public QQIdAccountXST {
public:
    WeiboAccountXST(const std::string& qq, const UserXST* owner,
                    const std::string& nickname, const DateXST& registerDate);

    ServiceTypeXST serviceType() const override;
};

#endif
