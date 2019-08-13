#ifndef AESCTRENCRYPTIONPROVIDER_H
#define AESCTRENCRYPTIONPROVIDER_H

#include "AbstractEncryptionProvider.h"

struct mbedtls_aes_context;

class AESCTREncryptionProvider : public AbstractEncryptionProvider {
public:
    explicit AESCTREncryptionProvider();
    ~AESCTREncryptionProvider();

    virtual bool setEncryptionKey(const uint8_t *key, int size) override;
    virtual bool setDecryptionKey(const uint8_t *key, int size) override;
    virtual bool setEncryptionIV(const uint8_t *IV, int size) override;
    virtual bool setDecryptionIV(const uint8_t *IV, int size) override;
    virtual bool getEncryptionIV(uint8_t *IV, int &size) override;
    virtual bool getDecryptionIV(uint8_t *IV, int &size) override;
    virtual bool encrypt(const uint8_t *plaintext, int size, uint8_t *encrypted) override;
    virtual bool decrypt(const uint8_t *encrypted, int size, uint8_t *plaintext) override;

private:
    mbedtls_aes_context *_encctx;
    mbedtls_aes_context *_decctx;

    std::uint8_t _encNonceCtr[16];
    std::uint8_t _decNonceCtr[16];
    std::size_t _encNcOffset = 0;
    std::size_t _decNcOffset = 0;
    std::uint8_t _encStreamBlock[16];
    std::uint8_t _decStreamBlock[16];
};

#endif // AESCTRENCRYPTIONPROVIDER_H
