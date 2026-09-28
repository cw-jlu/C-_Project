#ifndef QQ_ID_ACCOUNT_XST_H
#define QQ_ID_ACCOUNT_XST_H

#include "AccountXST.h"

// 使用 QQ 号作为 ID 的账号（抽象）：QQ 本身，以及与 QQ 共享 ID 的微博等服务
// 新的“共享 QQ 号”类微X产品只需继承本类并实现 serviceType()
class QQIdAccountXST : public AccountXST {
public:
    QQIdAccountXST(const std::string& qq, const UserXST* owner,
                   const std::string& nickname, const DateXST& registerDate);

    // QQ 号格式：5~11 位数字，首位不为 0
    static bool isValidQQ(const std::string& qq);

    std::string idKindName() const override;
    bool isValidAccountId(const std::string& id) const override;
    std::string linkedQQ() const override;        // 即自身 ID

protected:
    void printExtraInfo(std::ostream& os) const override;
};

#endif
