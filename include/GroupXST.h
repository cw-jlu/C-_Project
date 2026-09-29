#ifndef GROUP_XST_H
#define GROUP_XST_H

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "GroupOpResultXST.h"
#include "GroupPolicyXST.h"
#include "GroupRoleXST.h"
#include "SubGroupXST.h"

// 群：保存群号、群名、群主、成员、管理员、临时讨论组等数据，并持有一个管理策略
// 通用校验（是否成员等）在本类完成，权限与功能开关交给策略判断
// 切换策略时数据不变：当前模式不支持的管理员/讨论组数据会保留，切回支持的模式即恢复
class GroupXST {
public:
    GroupXST(int id, const std::string& name, const std::string& ownerId,
             std::unique_ptr<GroupPolicyXST> policy);
    GroupXST(const GroupXST&) = delete;
    GroupXST& operator=(const GroupXST&) = delete;

    // ---------- 基本信息 ----------
    int id() const;
    const std::string& name() const;
    const std::string& ownerId() const;
    const GroupPolicyXST& policy() const;

    // ---------- 身份 ----------
    bool isMember(const std::string& id) const;
    bool isOwner(const std::string& id) const;
    bool isAdmin(const std::string& id) const;        // 当前模式下是否为生效的管理员
    GroupRoleXST roleOf(const std::string& id) const;

    const std::vector<std::string>& members() const;  // 按入群顺序
    const std::set<std::string>& adminRecords() const;         // 管理员记录（含未生效的）
    const std::vector<SubGroupXST>& subGroupRecords() const;   // 讨论组记录（含未生效的）

    // ---------- 群操作（账号是否存在由调用方 ServiceXST 保证） ----------
    GroupOpResultXST apply(const std::string& applicant);
    GroupOpResultXST invite(const std::string& inviter, const std::string& invitee);
    GroupOpResultXST quit(const std::string& member);
    GroupOpResultXST kick(const std::string& op, const std::string& target);
    GroupOpResultXST setAdmin(const std::string& op, const std::string& target, bool grant);
    GroupOpResultXST createSubGroup(const std::string& op, const std::string& name,
                                    const std::vector<std::string>& members);
    GroupOpResultXST dissolveSubGroup(const std::string& op, const std::string& name);

    // 切换管理模式（仅群主）：只替换策略，成员等数据保持不变
    GroupOpResultXST changePolicy(const std::string& op, std::unique_ptr<GroupPolicyXST> policy);

    // ---------- 数据恢复（初始化/读文件用，不做权限检查，数据非法时抛出 std::invalid_argument） ----------
    void restoreMember(const std::string& id);
    void restoreAdmin(const std::string& id);
    void restoreSubGroup(const SubGroupXST& subGroup);

private:
    const SubGroupXST* findSubGroup(const std::string& name) const;
    void removeMemberEverywhere(const std::string& id);   // 退群/被踢：同时移出管理员与讨论组

    int m_id;
    std::string m_name;
    std::string m_ownerId;
    std::vector<std::string> m_members;
    std::set<std::string> m_admins;
    std::vector<SubGroupXST> m_subGroups;
    std::unique_ptr<GroupPolicyXST> m_policy;
};

#endif
