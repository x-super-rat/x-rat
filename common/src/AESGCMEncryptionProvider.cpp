#include <mbedtls/gcm.h>

#include "AESGCMEncryptionProvider.h"

AESGCMEncryptionProvider::AESGCMEncryptionProvider()
{
    _encctx = new mbedtls_gcm_context;
    _decctx = new mbedtls_gcm_context;
    mbedtls_gcm_init(_encctx);
    mbedtls_gcm_init(_decctx);
}

AESGCMEncryptionProvider::~AESGCMEncryptionProvider()
{
    mbedtls_gcm_free(_encctx);
    mbedtls_gcm_free(_decctx);
    delete _encctx;
    delete _decctx;
}

bool AESGCMEncryptionProvider::encrypt(const uint8_t *plaintext, int size, uint8_t *encrypted)
{
    int rc = mbedtls_gcm_update(_encctx, size, plaintext, encrypted);
    if (rc == 0) {
        return true;
    } else {
        return false; // Prompt
    }
}

bool AESGCMEncryptionProvider::decrypt(const uint8_t *encrypted, int size, uint8_t *plaintext)
{
    int rc = mbedtls_gcm_update(_decctx, size, encrypted, plaintext);
    if (rc == 0) {
        return true;
    } else {
        return false; // Prompt
    }
}

bool AESGCMEncryptionProvider::setEncryptionKey(const uint8_t *key, int size)
{
    int rc = mbedtls_gcm_setkey(_encctx, MBEDTLS_CIPHER_ID_AES, key, size * 8);
    if (rc != 0) {
        return false; // TODO: add error prompt
    } else {
        return true;
    }
}

bool AESGCMEncryptionProvider::setDecryptionKey(const uint8_t *key, int size)
{
    int rc = mbedtls_gcm_setkey(_decctx, MBEDTLS_CIPHER_ID_AES, key, size * 8);
    if (rc != 0) {
        return false; // TODO: add error prompt
    } else {
        return true;
    }
}

bool AESGCMEncryptionProvider::setEncryptionIVOrNonceCounter(const uint8_t *IV, int size)
{
    int rc = mbedtls_gcm_starts(_encctx, MBEDTLS_GCM_ENCRYPT, IV, size, NULL, 0);
    if (rc == 0) {
        return true;
    } else {
        return false; //TODO: prompt
    }
}

bool AESGCMEncryptionProvider::setDecryptionIVOrNonceCounter(const uint8_t *IV, int size)
{
    int rc = mbedtls_gcm_starts(_decctx, MBEDTLS_GCM_DECRYPT, IV, size, NULL, 0);
    if (rc == 0) {
        return true;
    } else {
        return false; //TODO: prompt
    }
}
