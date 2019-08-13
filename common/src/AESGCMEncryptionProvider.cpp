#include <cstring>

#include <mbedtls/gcm.h>

#include "AESGCMEncryptionProvider.h"

AESGCMEncryptionProvider::AESGCMEncryptionProvider()
{
    _encctx = new mbedtls_gcm_context;
    _decctx = new mbedtls_gcm_context;
    mbedtls_gcm_init(_encctx);
    mbedtls_gcm_init(_decctx);
    const unsigned char zero[16] = { 0 };
    mbedtls_gcm_starts(_encctx, MBEDTLS_GCM_ENCRYPT, zero, 16, nullptr, 0);
    mbedtls_gcm_starts(_decctx, MBEDTLS_GCM_DECRYPT, zero, 16, nullptr, 0);
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

bool AESGCMEncryptionProvider::setEncryptionIV(const uint8_t *IV, int size)
{
    //    int rc = mbedtls_gcm_starts(_encctx, MBEDTLS_GCM_ENCRYPT, IV, size, NULL, 0);
    //    if (rc == 0) {
    //        return true;
    //    } else {
    //        return false; //TODO: prompt
    //    }

    if (size != 16)
        return false;

    std::memcpy(_encctx->y, IV, 16);
    return true;
}

bool AESGCMEncryptionProvider::setDecryptionIV(const uint8_t *IV, int size)
{
    //    int rc = mbedtls_gcm_starts(_decctx, MBEDTLS_GCM_DECRYPT, IV, size, NULL, 0);
    //    if (rc == 0) {
    //        return true;
    //    } else {
    //        return false; //TODO: prompt
    //    }

    if (size != 16)
        return false;

    std::memcpy(_decctx->y, IV, 16);
    return true;
}

bool AESGCMEncryptionProvider::encryptStatic(const uint8_t *key, int keySize, const uint8_t *IV, int IVSize, const uint8_t *plaintext, int size, uint8_t *encrypted)
{
    mbedtls_gcm_context ctx;
    mbedtls_gcm_init(&ctx);
    mbedtls_gcm_setkey(&ctx, MBEDTLS_CIPHER_ID_AES, key, keySize * 8);
    mbedtls_gcm_starts(&ctx, MBEDTLS_ENCRYPT, IV, IVSize, nullptr, 0);
    mbedtls_gcm_update(&ctx, size, plaintext, encrypted);
    mbedtls_gcm_free(&ctx);
    return true;
}

bool AESGCMEncryptionProvider::decrpteStatic(const uint8_t *key, int keySize, const uint8_t *IV, int IVSize, const uint8_t *encrypted, int size, uint8_t *decrypted)
{
}
