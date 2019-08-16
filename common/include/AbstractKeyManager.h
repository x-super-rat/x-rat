#ifndef ABSTRACTKEYMANAGER_H
#define ABSTRACTKEYMANAGER_H

#include <cstdint>
#include <string>
#include <vector>

class AbstractKeyManager {
public:
    ~AbstractKeyManager() {}
    virtual bool retrieveMasterKey(std::uint32_t userId, std::vector<std::uint8_t> &key) = 0;
    virtual bool setMasterKey(std::uint32_t userId, const std::vector<std::uint8_t> &key) = 0;

protected:
    AbstractKeyManager() {}
};

#endif // ABSTRACTKEYMANAGER_H
