#ifndef USER_XST_H
#define USER_XST_H

#include <set>
#include <string>

#include "DateXST.h"
#include "ServiceTypeXST.h"

// 平台用户（自然人）：一个人可以开通多个“微X”服务，每个服务对应一个账号
// 出生日期、所在地属于人本身，由其名下各账号共享
class UserXST {
public:
    UserXST(const std::string& id, const std::string& name,
            const DateXST& birthday, const std::string& location);

    const std::string& id() const;
    const std::string& name() const;
    const DateXST& birthday() const;
    const std::string& location() const;

    void setName(const std::string& name);
    void setBirthday(const DateXST& birthday);
    void setLocation(const std::string& location);

    // 周岁
    int age(const DateXST& today = DateXST::today()) const;

    // 开通记录
    bool openService(ServiceTypeXST type);       // 已开通返回 false
    bool closeService(ServiceTypeXST type);      // 未开通返回 false
    bool hasService(ServiceTypeXST type) const;
    const std::set<ServiceTypeXST>& services() const;
    std::string servicesText() const;            // 如 "QQ、微信"，无则 "无"

private:
    std::string m_id;
    std::string m_name;
    DateXST m_birthday;
    std::string m_location;
    std::set<ServiceTypeXST> m_services;
};

#endif
