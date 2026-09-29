#ifndef GROUP_ROLE_XST_H
#define GROUP_ROLE_XST_H

#include <string>

// 群内身份，按权限从低到高排列，便于比较
enum class GroupRoleXST {
    None,        // 非成员
    Member,      // 普通成员
    Admin,       // 管理员（仅在支持管理员制度的模式下生效）
    Owner        // 群主
};

inline std::string groupRoleName(GroupRoleXST role) {
    switch (role) {
        case GroupRoleXST::None:   return "非成员";
        case GroupRoleXST::Member: return "成员";
        case GroupRoleXST::Admin:  return "管理员";
        case GroupRoleXST::Owner:  return "群主";
    }
    return "";
}

#endif
