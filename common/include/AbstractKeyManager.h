#ifndef ABSTRACTKEYMANAGER_H
#define ABSTRACTKEYMANAGER_H

#include <cstdint>
#include <string>
#include <vector>

class AbstractKeyManager {
public:
    ~AbstractKeyManager() {}
    virtual bool retrieveKey(std::uint32_t userId, std::vector<std::uint8_t> &key, std::vector<std::uint8_t> &salt) = 0;
    virtual bool setKey(std::uint32_t userId, const std::vector<std::uint8_t> &key, const std::vector<std::uint8_t> &salt) = 0;

protected:
    AbstractKeyManager() {}
};

#endif // ABSTRACTKEYMANAGER_H
