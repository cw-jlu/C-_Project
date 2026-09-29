#include "LoginManagerXST.h"

#include "PlatformXST.h"

LoginManagerXST::LoginManagerXST(PlatformXST& platform)
    : m_platform(platform), m_user(nullptr), m_primary(ServiceTypeXST::QQ) {}

LoginResultXST LoginManagerXST::login(ServiceTypeXST type, const std::string& accountId,
                                      const std::string& password) {
    if (m_user) return LoginResultXST::AlreadyLoggedIn;
    if (!m_platform.hasService(type)) return LoginResultXST::AccountNotFound;
    const AccountXST* account = m_platform.service(type).findAccount(accountId);
    if (!account) return LoginResultXST::AccountNotFound;
    if (!account->checkPassword(password)) return LoginResultXST::WrongPassword;

    m_user = &account->owner();
    m_primary = type;
    m_loggedIn = {type};
    return LoginResultXST::Ok;
}

LoginResultXST LoginManagerXST::confirm(ServiceTypeXST type) {
    if (!m_user) return LoginResultXST::NotLoggedIn;
    if (!m_platform.accountOf(m_user->id(), type)) return LoginResultXST::NotOpened;
    m_loggedIn.insert(type);
    return LoginResultXST::Ok;
}

std::vector<ServiceTypeXST> LoginManagerXST::confirmAll() {
    std::vector<ServiceTypeXST> pending = pendingServices();
    for (ServiceTypeXST type : pending) m_loggedIn.insert(type);
    return pending;
}

void LoginManagerXST::logout() {
    m_user = nullptr;
    m_loggedIn.clear();
}

bool LoginManagerXST::isActive() const { return m_user != nullptr; }
const UserXST* LoginManagerXST::currentUser() const { return m_user; }
ServiceTypeXST LoginManagerXST::primaryService() const { return m_primary; }

bool LoginManagerXST::isLoggedIn(ServiceTypeXST type) const {
    return m_loggedIn.count(type) > 0;
}

std::vector<ServiceTypeXST> LoginManagerXST::loggedInServices() const {
    return std::vector<ServiceTypeXST>(m_loggedIn.begin(), m_loggedIn.end());
}

std::vector<ServiceTypeXST> LoginManagerXST::pendingServices() const {
    std::vector<ServiceTypeXST> result;
    if (!m_user) return result;
    for (ServiceTypeXST type : m_user->services()) {
        if (!m_loggedIn.count(type)) result.push_back(type);
    }
    return result;
}
