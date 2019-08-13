#ifndef ABSTRACTENCRYPTIONPROVIDER_H
#define ABSTRACTENCRYPTIONPROVIDER_H

#include <cstdint>

class AbstractEncryptionProvider {
public:
    virtual ~AbstractEncryptionProvider() {}
    virtual bool setEncryptionKey(const std::uint8_t *key, int size) = 0;
    virtual bool setDecryptionKey(const std::uint8_t *key, int size) = 0;
    virtual bool setEncryptionIV(const std::uint8_t *IV, int size) = 0;
    virtual bool setDecryptionIV(const std::uint8_t *IV, int size) = 0;
    virtual bool getEncryptionIV(uint8_t *IV, int &size);
    virtual bool getDecryptionIV(uint8_t *IV, int &size);
    virtual bool encrypt(const std::uint8_t *plaintext, int size, std::uint8_t *encrypted) = 0;
    virtual bool decrypt(const std::uint8_t *encrypted, int size, std::uint8_t *plaintext) = 0;

    virtual bool encryptStatic(const std::uint8_t *key, int keySize, const std::uint8_t *IV, int IVSize,
        const std::uint8_t *plaintext, int size, std::uint8_t *encrypted)
        = 0;
    virtual bool decrpteStatic(const std::uint8_t *key, int keySize, const std::uint8_t *IV, int IVSize,
        const std::uint8_t *encrypted, int size, std::uint8_t *decrypted)
        = 0;

protected:
    explicit AbstractEncryptionProvider() {}
    static bool checkError(int rc);
};

#endif // ABSTRACTENCRYPTIONPROVIDER_H
