#ifndef FORWARDMANAGER_H
#define FORWARDMANAGER_H

#include "PacketVector.h"

class ForwardManager {
public:
    explicit ForwardManager();

    bool linkForward(PacketVector *packet);
    bool netForward(PacketVector *packet);
};

#endif // FORWARDMANAGER_H
