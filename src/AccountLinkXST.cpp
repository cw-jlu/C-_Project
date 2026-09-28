#include "AccountLinkXST.h"

AccountLinkXST::AccountLinkXST(const AccountXST& source, const AccountXST& target)
    : m_source(&source), m_target(&target) {}

const AccountXST& AccountLinkXST::source() const { return *m_source; }
const AccountXST& AccountLinkXST::target() const { return *m_target; }
