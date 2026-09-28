#ifndef FRIEND_XST_H
#define FRIEND_XST_H

#include <string>

// 好友条目：好友账号 ID + 本人为其设置的备注
// 好友关系是单向记录的，双方各自维护自己的备注
class FriendXST {
public:
    explicit FriendXST(const std::string& id, const std::string& remark = "");

    const std::string& id() const;
    const std::string& remark() const;
    void setRemark(const std::string& remark);

    // 有备注显示备注，否则显示 ID
    std::string displayName() const;

private:
    std::string m_id;
    std::string m_remark;
};

#endif
