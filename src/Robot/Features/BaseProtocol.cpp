#include "./BaseProtocol/BaseProtocol.hpp"

bool BaseProtocol::IsOnline() const {

    this->_statusStatusMutex.lock_shared();
    bool isOnline = this->_isOnline;
    this->_statusStatusMutex.unlock_shared();

    return isOnline;
}
bool BaseProtocol::IsThreadActive() const {

    this->_threadStatusMutex.lock_shared();
    bool isThreadActive = this->_isThreadActive;
    this->_threadStatusMutex.unlock_shared();

    return isThreadActive;
}
