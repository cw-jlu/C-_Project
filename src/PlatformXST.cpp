#include "PlatformXST.h"

#include <algorithm>
#include <stdexcept>

#include "QQServiceXST.h"
#include "WeChatServiceXST.h"
#include "WeiboServiceXST.h"

PlatformXST::PlatformXST() {
    registerService(std::make_unique<QQServiceXST>());
    registerService(std::make_unique<WeChatServiceXST>());
    registerService(std::make_unique<WeiboServiceXST>());
}

void PlatformXST::registerService(std::unique_ptr<ServiceXST> service) {
    if (!service) throw std::invalid_argument("服务为空");
    ServiceTypeXST type = service->type();
    if (m_services.count(type)) {
        throw std::invalid_argument("服务已注册: " + service->name());
    }
    m_services[type] = std::move(service);
}

bool PlatformXST::hasService(ServiceTypeXST type) const {
    return m_services.count(type) > 0;
}

ServiceXST& PlatformXST::service(ServiceTypeXST type) {
    auto it = m_services.find(type);
    if (it == m_services.end()) throw std::out_of_range("服务未注册: " + serviceDisplayName(type));
    return *it->second;
}

const ServiceXST& PlatformXST::service(ServiceTypeXST type) const {
    auto it = m_services.find(type);
    if (it == m_services.end()) throw std::out_of_range("服务未注册: " + serviceDisplayName(type));
    return *it->second;
}

std::vector<ServiceTypeXST> PlatformXST::serviceTypes() const {
    std::vector<ServiceTypeXST> result;
    for (const auto& entry : m_services) result.push_back(entry.first);
    return result;
}

UserXST& PlatformXST::addUser(const std::string& id, const std::string& name,
                              const DateXST& birthday, const std::string& location) {
    if (m_users.count(id)) throw std::invalid_argument("用户编号已存在: " + id);
    auto user = std::make_unique<UserXST>(id, name, birthday, location);
    UserXST& ref = *user;
    m_users[id] = std::move(user);
    return ref;
}

UserXST* PlatformXST::findUser(const std::string& id) {
    auto it = m_users.find(id);
    return it == m_users.end() ? nullptr : it->second.get();
}

const UserXST* PlatformXST::findUser(const std::string& id) const {
    auto it = m_users.find(id);
    return it == m_users.end() ? nullptr : it->second.get();
}

std::vector<const UserXST*> PlatformXST::users() const {
    std::vector<const UserXST*> result;
    result.reserve(m_users.size());
    for (const auto& entry : m_users) result.push_back(entry.second.get());
    return result;
}

bool PlatformXST::removeUser(const std::string& id) {
    auto it = m_users.find(id);
    if (it == m_users.end() || !it->second->services().empty()) return false;
    m_users.erase(it);
    return true;
}

std::string PlatformXST::nextUserId() const {
    int next = 1;
    for (const auto& entry : m_users) {
        const std::string& id = entry.first;
        if (id.size() > 1 && id[0] == 'P') {
            try {
                next = std::max(next, std::stoi(id.substr(1)) + 1);
            } catch (...) {
                // 非 P+数字 格式的编号不参与计算
            }
        }
    }
    std::string digits = std::to_string(next);
    return "P" + std::string(digits.size() < 3 ? 3 - digits.size() : 0, '0') + digits;
}

std::string PlatformXST::nextQQNumber() const {
    long long next = 10001;
    if (hasService(ServiceTypeXST::QQ)) {
        for (const AccountXST* account : service(ServiceTypeXST::QQ).accounts()) {
            next = std::max(next, std::stoll(account->id()) + 1);
        }
    }
    return std::to_string(next);
}

AccountXST& PlatformXST::openService(const std::string& personId, ServiceTypeXST type,
                                     const std::string& accountId, const std::string& nickname,
                                     const std::string& password, const DateXST& registerDate,
                                     const std::string& extra) {
    UserXST* user = findUser(personId);
    if (!user) throw std::invalid_argument("用户不存在: " + personId);
    ServiceXST& target = service(type);

    std::unique_ptr<AccountXST> account =
        target.createAccount(accountId, user, nickname, registerDate, extra);
    if (!account->setPassword(password)) {
        throw std::invalid_argument("密码格式错误（6~16 位，不含空白、逗号、分号）");
    }

    // 微博与 QQ 共用号码、微信绑定 QQ：对应的 QQ 号必须是本人已开通的 QQ
    const std::string qq = account->linkedQQ();
    if (type != ServiceTypeXST::QQ && !qq.empty()) {
        const AccountXST* qqAccount = service(ServiceTypeXST::QQ).findAccount(qq);
        if (!qqAccount || qqAccount->owner().id() != personId) {
            throw std::invalid_argument("QQ 号 " + qq + " 不是 " + user->name() + " 的 QQ");
        }
    }

    AccountXST& ref = target.addAccount(std::move(account));
    user->openService(type);
    return ref;
}

AccountXST* PlatformXST::accountOf(const std::string& personId, ServiceTypeXST type) {
    if (!hasService(type)) return nullptr;
    return service(type).findAccountByOwner(personId);
}

const AccountXST* PlatformXST::accountOf(const std::string& personId, ServiceTypeXST type) const {
    if (!hasService(type)) return nullptr;
    return service(type).findAccountByOwner(personId);
}
