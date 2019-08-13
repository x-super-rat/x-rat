#ifndef XFRMMANAGER_H
#define XFRMMANAGER_H

#include "PacketVector.h"

class XfrmManager {
public:
    explicit XfrmManager();

    bool link2net(PacketVector *packet);
    bool net2link(PacketVector *packet);
};

#endif // XFRMMANAGER_H
