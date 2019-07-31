#include "AbstractEncryptionProvider.h"

bool AbstractEncryptionProvider::checkError(int rc)
{
    if (rc == 0) {
        return true;
    } else {
        // FIXME: Print error
        return false;
    }
}
