#ifndef ENCRYPTIONMANAGER_H
#define ENCRYPTIONMANAGER_H

#include "AbstractEncryptionProvider.h"
#include "AbstractKeyManager.h"
#include "PacketVector.h"

class EncryptionManager {
public:
    explicit EncryptionManager(AbstractEncryptionProvider *encryptionProvider, AbstractKeyManager *keyManager)
        : _encryptionProvider(encryptionProvider)
        , _keyManager(keyManager)
    {
    }

    bool encryptPacket(PacketVector *packet);
    bool decryptPacket(PacketVector *packet);

private:
    AbstractEncryptionProvider *_encryptionProvider;
    AbstractKeyManager *_keyManager;
};

#endif // ENCRYPTIONMANAGER_H
