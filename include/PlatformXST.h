#ifndef PLATFORM_XST_H
#define PLATFORM_XST_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "ServiceXST.h"
#include "UserXST.h"

// 立体社交平台：统一管理用户（自然人）与各“微X”服务
// 后续阶段在此基础上加入登录管理（P4）与文件读写（P5）
class PlatformXST {
public:
    PlatformXST();                                   // 注册 QQ、微信、微博 三个服务
    PlatformXST(const PlatformXST&) = delete;
    PlatformXST& operator=(const PlatformXST&) = delete;

    // ---------- 服务 ----------
    // 注册新的微X服务（可扩展），同类型重复注册抛出 std::invalid_argument
    void registerService(std::unique_ptr<ServiceXST> service);
    bool hasService(ServiceTypeXST type) const;
    ServiceXST& service(ServiceTypeXST type);        // 未注册抛出 std::out_of_range
    const ServiceXST& service(ServiceTypeXST type) const;
    std::vector<ServiceTypeXST> serviceTypes() const;

    // ---------- 用户 ----------
    UserXST& addUser(const std::string& id, const std::string& name,
                     const DateXST& birthday, const std::string& location);
    UserXST* findUser(const std::string& id);
    const UserXST* findUser(const std::string& id) const;
    std::vector<const UserXST*> users() const;      // 按编号排序
    bool removeUser(const std::string& id);         // 仅能删除尚未开通任何服务的用户
    std::string nextUserId() const;                 // 下一个可用用户编号，如 "P007"

    // ---------- 开通 ----------
    // 为用户开通服务并创建账号；账号对应的 QQ 号（微博共用、微信绑定）必须是本人的 QQ
    // 校验失败（含密码格式错误）抛出 std::invalid_argument
    AccountXST& openService(const std::string& personId, ServiceTypeXST type,
                            const std::string& accountId, const std::string& nickname,
                            const std::string& password, const DateXST& registerDate,
                            const std::string& extra = "");
    std::string nextQQNumber() const;               // 下一个可分配的 QQ 号

    // 用户在某服务中的账号，未开通返回 nullptr
    AccountXST* accountOf(const std::string& personId, ServiceTypeXST type);
    const AccountXST* accountOf(const std::string& personId, ServiceTypeXST type) const;

private:
    std::map<std::string, std::unique_ptr<UserXST>> m_users;
    std::map<ServiceTypeXST, std::unique_ptr<ServiceXST>> m_services;
};

#endif
