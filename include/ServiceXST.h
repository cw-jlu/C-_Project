#ifndef SERVICE_XST_H
#define SERVICE_XST_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "AccountXST.h"
#include "FriendOpResultXST.h"
#include "ServiceTypeXST.h"

// “微X”服务抽象基类：本服务的账号容器，并负责本服务内的好友关系维护
// 子类通过工厂方法 createAccount() 决定创建哪一种账号
class ServiceXST {
public:
    virtual ~ServiceXST() = default;
    ServiceXST(const ServiceXST&) = delete;
    ServiceXST& operator=(const ServiceXST&) = delete;

    virtual ServiceTypeXST type() const = 0;
    std::string name() const;

    // 工厂方法：创建本服务类型的账号（只创建，不加入容器）
    // extra：附加信息，微信为绑定的 QQ 号，其他服务忽略
    virtual std::unique_ptr<AccountXST> createAccount(
        const std::string& id, const UserXST* owner, const std::string& nickname,
        const DateXST& registerDate, const std::string& extra = "") const = 0;

    // ---------- 账号容器 ----------
    // ID 重复或该用户已在本服务有账号时抛出 std::invalid_argument
    AccountXST& addAccount(std::unique_ptr<AccountXST> account);
    AccountXST* findAccount(const std::string& id);
    const AccountXST* findAccount(const std::string& id) const;
    AccountXST* findAccountByOwner(const std::string& personId);
    const AccountXST* findAccountByOwner(const std::string& personId) const;
    // 按对应 QQ 号查找（QQ/微博为本号，微信为绑定的 QQ）
    const AccountXST* findAccountByQQ(const std::string& qq) const;
    std::vector<const AccountXST*> accounts() const;     // 按 ID 排序
    size_t accountCount() const;

    // ---------- 好友关系（双向） ----------
    // 双方互加好友，remarkByA 为 A 给 B 的备注，remarkByB 为 B 给 A 的备注
    FriendOpResultXST addFriendship(const std::string& a, const std::string& b,
                                    const std::string& remarkByA = "",
                                    const std::string& remarkByB = "");
    // 解除好友关系，双方列表中都删除
    FriendOpResultXST removeFriendship(const std::string& a, const std::string& b);
    // 修改 owner 给好友设置的备注（只影响自己这一侧）
    FriendOpResultXST setFriendRemark(const std::string& owner, const std::string& friendId,
                                      const std::string& remark);
    // 本服务内两个账号的共同好友
    std::vector<const AccountXST*> commonFriends(const std::string& a, const std::string& b) const;

protected:
    ServiceXST() = default;

private:
    std::map<std::string, std::unique_ptr<AccountXST>> m_accounts;
};

#endif
