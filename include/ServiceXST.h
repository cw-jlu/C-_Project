#ifndef SERVICE_XST_H
#define SERVICE_XST_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "AccountIdRuleXST.h"
#include "AccountXST.h"
#include "FriendOpResultXST.h"
#include "GroupOpResultXST.h"
#include "GroupXST.h"
#include "ServiceTypeXST.h"

// “微X”服务抽象基类：本服务的账号容器与群容器，负责本服务内的好友关系与群维护
// 子类通过工厂方法 createAccount() / createDefaultGroupPolicy() 决定账号类型与默认群模式
class ServiceXST {
public:
    virtual ~ServiceXST() = default;
    ServiceXST(const ServiceXST&) = delete;
    ServiceXST& operator=(const ServiceXST&) = delete;

    virtual ServiceTypeXST type() const = 0;
    std::string name() const;

    // 开通时账号 ID 的产生方式：分配新 QQ 号 / 沿用本人 QQ 号 / 用户自定义独立 ID
    virtual AccountIdRuleXST idRule() const = 0;

    // 工厂方法：创建本服务类型的账号（只创建，不加入容器）
    // extra：附加信息，微信为绑定的 QQ 号，其他服务忽略
    virtual std::unique_ptr<AccountXST> createAccount(
        const std::string& id, const UserXST* owner, const std::string& nickname,
        const DateXST& registerDate, const std::string& extra = "") const = 0;

    // 工厂方法：本服务新建群时采用的默认管理模式
    virtual std::unique_ptr<GroupPolicyXST> createDefaultGroupPolicy() const = 0;

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

    // ---------- 群容器 ----------
    // 以默认管理模式建群，群主自动入群；群号重复或群主账号不存在时抛出 std::invalid_argument
    GroupXST& createGroup(int groupId, const std::string& name, const std::string& ownerId);
    GroupXST* findGroup(int groupId);
    const GroupXST* findGroup(int groupId) const;
    std::vector<const GroupXST*> groups() const;                              // 按群号排序
    std::vector<const GroupXST*> groupsOf(const std::string& accountId) const;

    // ---------- 群数据恢复（初始化/读文件用，不做权限检查，非法时抛出 std::invalid_argument） ----------
    void restoreGroupMember(int groupId, const std::string& accountId);
    void restoreGroupAdmin(int groupId, const std::string& accountId);
    void restoreSubGroup(int groupId, const std::string& name, const std::string& creatorId,
                         const std::vector<std::string>& members);

    // ---------- 群操作（成功后同步维护账号的群列表） ----------
    GroupOpResultXST applyJoin(int groupId, const std::string& applicant);
    // 只能邀请自己的好友
    GroupOpResultXST invite(int groupId, const std::string& inviter, const std::string& invitee);
    GroupOpResultXST quitGroup(int groupId, const std::string& member);
    GroupOpResultXST kick(int groupId, const std::string& op, const std::string& target);
    GroupOpResultXST setGroupAdmin(int groupId, const std::string& op, const std::string& target,
                                   bool grant);
    GroupOpResultXST createSubGroup(int groupId, const std::string& op, const std::string& name,
                                    const std::vector<std::string>& members);
    GroupOpResultXST dissolveSubGroup(int groupId, const std::string& op, const std::string& name);
    // 动态切换群管理模式（policyCode: QQ / WX / WB），群成员数据不变
    GroupOpResultXST changeGroupPolicy(int groupId, const std::string& op,
                                       const std::string& policyCode);

protected:
    ServiceXST() = default;

private:
    GroupXST& requireGroup(int groupId);
    AccountXST& requireAccount(const std::string& id);

    std::map<std::string, std::unique_ptr<AccountXST>> m_accounts;
    std::map<int, std::unique_ptr<GroupXST>> m_groups;
};

#endif
