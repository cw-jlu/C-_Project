#ifndef ACCOUNT_XST_H
#define ACCOUNT_XST_H

#include <iosfwd>
#include <set>
#include <string>

#include "DateXST.h"
#include "FriendListXST.h"
#include "ServiceTypeXST.h"

class UserXST;

// 账号抽象基类：某个用户在某个“微X”服务中的账号
// 公共部分：号码 ID、所属用户、昵称、申请时间（T 龄）、好友列表、群列表
// 由子类决定：属于哪个服务、ID 体系（QQ 号 / 独立 ID）、对应的 QQ 号
class AccountXST {
public:
    AccountXST(const std::string& id, const UserXST* owner,
               const std::string& nickname, const DateXST& registerDate);
    virtual ~AccountXST() = default;

    // 账号是多态对象，禁止拷贝以免切片
    AccountXST(const AccountXST&) = delete;
    AccountXST& operator=(const AccountXST&) = delete;

    // ---------- 基本信息 ----------
    const std::string& id() const;
    const UserXST& owner() const;
    const std::string& nickname() const;
    void setNickname(const std::string& nickname);
    const DateXST& registerDate() const;
    int tAge(const DateXST& today = DateXST::today()) const;   // T 龄（整年）

    // ---------- 密码 ----------
    // 密码格式：6~16 位，不含空白、逗号、分号（逗号、分号为数据文件分隔符）
    static bool isValidPassword(const std::string& password);
    bool setPassword(const std::string& password);            // 格式不符返回 false
    bool checkPassword(const std::string& password) const;    // 未设置密码时恒为 false

    // ---------- 由子类实现的多态接口 ----------
    virtual ServiceTypeXST serviceType() const = 0;
    virtual std::string idKindName() const = 0;                     // 如 "QQ号"、"微信号"
    virtual bool isValidAccountId(const std::string& id) const = 0; // 本服务 ID 格式校验
    virtual std::string linkedQQ() const = 0;                       // 对应的 QQ 号，没有返回空串
    std::string serviceName() const;

    // ---------- 好友维护 ----------
    // 拒绝：自己、ID 格式不符、已是好友
    bool addFriend(const std::string& friendId, const std::string& remark = "");
    bool removeFriend(const std::string& friendId);
    bool setFriendRemark(const std::string& friendId, const std::string& remark);
    bool hasFriend(const std::string& friendId) const;
    const FriendXST* findFriend(const std::string& friendId) const;
    const FriendListXST& friends() const;

    // ---------- 群（本服务内的群号） ----------
    bool joinGroup(int groupId);
    bool leaveGroup(int groupId);
    bool inGroup(int groupId) const;
    const std::set<int>& groups() const;

    // 模板方法：打印公共信息，再调用 printExtraInfo 打印子类特有信息
    void printInfo(std::ostream& os, const DateXST& today = DateXST::today()) const;

protected:
    virtual void printExtraInfo(std::ostream& os) const;

private:
    std::string m_id;
    const UserXST* m_owner;          // 不拥有，由平台统一管理用户对象的生命周期
    std::string m_nickname;
    DateXST m_registerDate;
    std::string m_password;
    FriendListXST m_friends;
    std::set<int> m_groups;
};

#endif
