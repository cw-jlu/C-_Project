#include "UserXST.h"

#include <stdexcept>

UserXST::UserXST(const std::string& id, const std::string& name,
                 const DateXST& birthday, const std::string& location)
    : m_id(id), m_name(name), m_birthday(birthday), m_location(location) {
    if (m_id.empty()) throw std::invalid_argument("用户编号不能为空");
    if (m_name.empty()) throw std::invalid_argument("用户姓名不能为空");
}

const std::string& UserXST::id() const { return m_id; }
const std::string& UserXST::name() const { return m_name; }
const DateXST& UserXST::birthday() const { return m_birthday; }
const std::string& UserXST::location() const { return m_location; }

void UserXST::setName(const std::string& name) {
    if (name.empty()) throw std::invalid_argument("用户姓名不能为空");
    m_name = name;
}

void UserXST::setBirthday(const DateXST& birthday) { m_birthday = birthday; }
void UserXST::setLocation(const std::string& location) { m_location = location; }

int UserXST::age(const DateXST& today) const {
    return m_birthday.fullYearsUntil(today);
}

bool UserXST::openService(ServiceTypeXST type) {
    return m_services.insert(type).second;
}

bool UserXST::closeService(ServiceTypeXST type) {
    return m_services.erase(type) > 0;
}

bool UserXST::hasService(ServiceTypeXST type) const {
    return m_services.count(type) > 0;
}

const std::set<ServiceTypeXST>& UserXST::services() const { return m_services; }

std::string UserXST::servicesText() const {
    std::string text;
    for (ServiceTypeXST type : m_services) {
        if (!text.empty()) text += "、";
        text += serviceDisplayName(type);
    }
    return text.empty() ? "无" : text;
}
