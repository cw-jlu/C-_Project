#ifndef ACCOUNT_ID_RULE_XST_H
#define ACCOUNT_ID_RULE_XST_H

// 开通服务时账号 ID 的产生方式（对应题目中“两种 ID 体系”）
enum class AccountIdRuleXST {
    NewQQNumber,      // 由平台分配新的 QQ 号（QQ）
    SharedQQNumber,   // 沿用本人的 QQ 号，需先开通 QQ（微博等）
    IndependentId     // 用户自定义独立 ID，可选绑定本人 QQ（微信等）
};

#endif
