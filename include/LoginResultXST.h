#ifndef LOGIN_RESULT_XST_H
#define LOGIN_RESULT_XST_H

#include <string>

// 登录操作结果
enum class LoginResultXST {
    Ok,
    AccountNotFound,     // 账号不存在
    WrongPassword,       // 密码错误
    AlreadyLoggedIn,     // 已有用户登录，需先注销
    NotLoggedIn,         // 尚未通过密码登录任何服务
    NotOpened            // 本人未开通该服务
};

inline std::string loginMessage(LoginResultXST result) {
    switch (result) {
        case LoginResultXST::Ok:              return "登录成功";
        case LoginResultXST::AccountNotFound: return "账号不存在";
        case LoginResultXST::WrongPassword:   return "密码错误";
        case LoginResultXST::AlreadyLoggedIn: return "已有用户登录，请先注销";
        case LoginResultXST::NotLoggedIn:     return "请先用账号密码登录任一服务";
        case LoginResultXST::NotOpened:       return "你尚未开通该服务";
    }
    return "";
}

#endif
