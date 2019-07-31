#ifndef AESGCMENCRYPTIONPROVIDER_H
#define AESGCMENCRYPTIONPROVIDER_H

#include "AbstractEncryptionProvider.h"

struct mbedtls_gcm_context;

class AESGCMEncryptionProvider : public AbstractEncryptionProvider {
public:
    explicit AESGCMEncryptionProvider();
    ~AESGCMEncryptionProvider();

    virtual bool setEncryptionKey(const uint8_t *key, int size) override;
    virtual bool setDecryptionKey(const uint8_t *key, int size) override;
    virtual bool setEncryptionIVOrNonceCounter(const uint8_t *IV, int size) override;
    virtual bool setDecryptionIVOrNonceCounter(const uint8_t *IV, int size) override;
    virtual bool getEncryptionIVOrNonceCounter(uint8_t *IV, int &size) override;
    virtual bool getDecryptionIVorNonceCounter(uint8_t *IV, int &size) override;
    virtual bool encrypt(const uint8_t *plaintext, int size, uint8_t *encrypted) override;
    virtual bool decrypt(const uint8_t *encrypted, int size, uint8_t *plaintext) override;

private:
    mbedtls_gcm_context *_encctx;
    mbedtls_gcm_context *_decctx;
};

#endif // AESGCMENCRYPTIONPROVIDER_H
