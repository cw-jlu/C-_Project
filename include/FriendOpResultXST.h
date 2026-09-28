#ifndef FRIEND_OP_RESULT_XST_H
#define FRIEND_OP_RESULT_XST_H

#include <string>

// 好友操作结果：成功或失败原因，供菜单给出明确提示
enum class FriendOpResultXST {
    Ok,
    AccountNotFound,     // 本人账号不存在
    FriendNotFound,      // 对方账号在本服务中不存在
    InvalidId,           // 对方 ID 不符合本服务的格式
    SelfNotAllowed,      // 不能添加自己
    AlreadyFriends,      // 已经是好友
    NotFriends           // 还不是好友
};

inline std::string friendOpMessage(FriendOpResultXST result) {
    switch (result) {
        case FriendOpResultXST::Ok:              return "操作成功";
        case FriendOpResultXST::AccountNotFound: return "本人账号不存在";
        case FriendOpResultXST::FriendNotFound:  return "对方账号在本服务中不存在";
        case FriendOpResultXST::InvalidId:       return "ID 格式不符合本服务要求";
        case FriendOpResultXST::SelfNotAllowed:  return "不能添加自己为好友";
        case FriendOpResultXST::AlreadyFriends:  return "你们已经是好友了";
        case FriendOpResultXST::NotFriends:      return "对方不是你的好友";
    }
    return "";
}

#endif
