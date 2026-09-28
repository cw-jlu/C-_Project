#ifndef QQ_ACCOUNT_XST_H
#define QQ_ACCOUNT_XST_H

#include "QQIdAccountXST.h"

// QQ ук╨е
class QQAccountXST : public QQIdAccountXST {
public:
    QQAccountXST(const std::string& qq, const UserXST* owner,
                 const std::string& nickname, const DateXST& registerDate);

    ServiceTypeXST serviceType() const override;
};

#endif
