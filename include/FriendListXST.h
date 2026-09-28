#ifndef FRIEND_LIST_XST_H
#define FRIEND_LIST_XST_H

#include <string>
#include <vector>

#include "FriendXST.h"

// 好友容器：负责好友条目的增、删、改、查，保证同一 ID 不重复
// 按添加顺序保存，便于展示
class FriendListXST {
public:
    bool add(const FriendXST& item);                                 // ID 已存在返回 false
    bool remove(const std::string& id);                              // 不存在返回 false
    bool setRemark(const std::string& id, const std::string& remark);// 不存在返回 false

    bool contains(const std::string& id) const;
    const FriendXST* find(const std::string& id) const;              // 不存在返回 nullptr

    // 按 ID 或备注模糊查询
    std::vector<FriendXST> search(const std::string& keyword) const;

    std::vector<std::string> ids() const;
    const std::vector<FriendXST>& items() const;
    size_t size() const;
    bool empty() const;

private:
    std::vector<FriendXST>::iterator locate(const std::string& id);
    std::vector<FriendXST>::const_iterator locate(const std::string& id) const;

    std::vector<FriendXST> m_items;
};

#endif
