#ifndef ABSTRACTENCRYPTIONPROVIDER_H
#define ABSTRACTENCRYPTIONPROVIDER_H

#include <cstdint>

class AbstractEncryptionProvider {
public:
    virtual bool setEncryptionKey(const std::uint8_t *key, int size) = 0;
    virtual bool setDecryptionKey(const std::uint8_t *key, int size) = 0;
    virtual bool setEncryptionIVOrNonceCounter(const std::uint8_t *IV, int size) = 0;
    virtual bool setDecryptionIVOrNonceCounter(const std::uint8_t *IV, int size) = 0;
    virtual bool getEncryptionIVOrNonceCounter(uint8_t *IV, int &size);
    virtual bool getDecryptionIVorNonceCounter(uint8_t *IV, int &size);
    virtual bool encrypt(const std::uint8_t *plaintext, int size, std::uint8_t *encrypted) = 0;
    virtual bool decrypt(const std::uint8_t *encrypted, int size, std::uint8_t *plaintext) = 0;

protected:
    explicit AbstractEncryptionProvider() {}
    ~AbstractEncryptionProvider() {}
    bool checkError(int rc);
};

#endif // ABSTRACTENCRYPTIONPROVIDER_H
