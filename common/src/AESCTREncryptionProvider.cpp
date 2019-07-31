#include <cstring>

#include <mbedtls/aes.h>

#include "AESCTREncryptionProvider.h"

AESCTREncryptionProvider::AESCTREncryptionProvider()
{
    _encctx = new mbedtls_aes_context;
    _decctx = new mbedtls_aes_context;
    mbedtls_aes_init(_encctx);
    mbedtls_aes_init(_decctx);
}

AESCTREncryptionProvider::~AESCTREncryptionProvider()
{
    mbedtls_aes_free(_encctx);
    mbedtls_aes_free(_decctx);
}

bool AESCTREncryptionProvider::setEncryptionKey(const uint8_t *key, int size)
{
    return checkError(mbedtls_aes_setkey_enc(_encctx, key, size * 8));
}

bool AESCTREncryptionProvider::setDecryptionKey(const uint8_t *key, int size)
{
    return checkError(mbedtls_aes_setkey_enc(_encctx, key, size * 8));
}

bool AESCTREncryptionProvider::setEncryptionIVOrNonceCounter(const uint8_t *IV, int size)
{
    if (size <= 16) {
        std::memcpy(_encNonceCtr, IV, size);
        return true;
    } else {
        return false;
    }
}

bool AESCTREncryptionProvider::setDecryptionIVOrNonceCounter(const uint8_t *IV, int size)
{
    if (size <= 16) {
        std::memcpy(_decNonceCtr, IV, size);
        return true;
    } else {
        return false;
    }
}

bool AESCTREncryptionProvider::getEncryptionIVOrNonceCounter(std::uint8_t *IV, int &size)
{
    throw; //FIXME: remove this function
}

bool AESCTREncryptionProvider::getDecryptionIVorNonceCounter(std::uint8_t *IV, int &size)
{
    throw; //FIXME: remove this function
}

bool AESCTREncryptionProvider::encrypt(const uint8_t *plaintext, int size, uint8_t *encrypted)
{
    return checkError(mbedtls_aes_crypt_ctr(_encctx, size, &_encNcOffset, _encNonceCtr, _encStreamBlock, plaintext, encrypted));
}

bool AESCTREncryptionProvider::decrypt(const uint8_t *encrypted, int size, uint8_t *plaintext)
{
    return checkError(mbedtls_aes_crypt_ctr(_decctx, size, &_decNcOffset, _decNonceCtr, _decStreamBlock, encrypted, plaintext));
}
