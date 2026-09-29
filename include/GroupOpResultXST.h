#ifndef GROUP_OP_RESULT_XST_H
#define GROUP_OP_RESULT_XST_H

#include <string>

// 群操作结果：成功或失败原因
enum class GroupOpResultXST {
    Ok,
    GroupNotFound,       // 群不存在
    AccountNotFound,     // 相关账号在本服务中不存在
    NotMember,           // 操作者不是群成员
    TargetNotMember,     // 目标不是群成员
    AlreadyMember,       // 已经是群成员
    NotFriends,          // 只能邀请自己的好友
    PermissionDenied,    // 身份权限不足
    NotSupported,        // 当前群管理模式不支持该功能
    OwnerCannotQuit,     // 群主不能退群
    InvalidTarget,       // 不能对自己或群主进行该操作
    SubGroupExists,      // 同名讨论组已存在
    SubGroupNotFound,    // 讨论组不存在
    InvalidSubGroup,     // 讨论组名称为空或成员不足 2 人
    InvalidPolicy        // 未知的群管理模式
};

inline std::string groupOpMessage(GroupOpResultXST result) {
    switch (result) {
        case GroupOpResultXST::Ok:               return "操作成功";
        case GroupOpResultXST::GroupNotFound:    return "群不存在";
        case GroupOpResultXST::AccountNotFound:  return "账号在本服务中不存在";
        case GroupOpResultXST::NotMember:        return "你不是该群成员";
        case GroupOpResultXST::TargetNotMember:  return "对方不是该群成员";
        case GroupOpResultXST::AlreadyMember:    return "已经是该群成员";
        case GroupOpResultXST::NotFriends:       return "只能邀请自己的好友";
        case GroupOpResultXST::PermissionDenied: return "权限不足";
        case GroupOpResultXST::NotSupported:     return "当前群管理模式不支持该功能";
        case GroupOpResultXST::OwnerCannotQuit:  return "群主不能退出自己的群";
        case GroupOpResultXST::InvalidTarget:    return "不能对自己或群主进行该操作";
        case GroupOpResultXST::SubGroupExists:   return "同名讨论组已存在";
        case GroupOpResultXST::SubGroupNotFound: return "讨论组不存在";
        case GroupOpResultXST::InvalidSubGroup:  return "讨论组名称不能为空，且至少包含 2 名成员";
        case GroupOpResultXST::InvalidPolicy:    return "未知的群管理模式";
    }
    return "";
}

#endif
