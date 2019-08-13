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

    ///
    /// \brief setEncryptionIV
    /// \param IV
    /// \param size The size must be 16
    /// \return
    ///
    virtual bool setEncryptionIV(const uint8_t *IV, int size) override;
    virtual bool setDecryptionIV(const uint8_t *IV, int size) override;
    virtual bool getEncryptionIV(uint8_t *IV, int &size) override;
    virtual bool getDecryptionIV(uint8_t *IV, int &size) override;
    virtual bool encrypt(const uint8_t *plaintext, int size, uint8_t *encrypted) override;
    virtual bool decrypt(const uint8_t *encrypted, int size, uint8_t *plaintext) override;
    virtual bool encryptStatic(const uint8_t *key, int keySize, const uint8_t *IV, int IVSize, const uint8_t *plaintext, int size, uint8_t *encrypted) override;
    virtual bool decrpteStatic(const uint8_t *key, int keySize, const uint8_t *IV, int IVSize, const uint8_t *encrypted, int size, uint8_t *decrypted) override;

private:
    mbedtls_gcm_context *_encctx;
    mbedtls_gcm_context *_decctx;
};

#endif // AESGCMENCRYPTIONPROVIDER_H
