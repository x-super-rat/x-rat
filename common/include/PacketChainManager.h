#ifndef PACKETCHAINMANAGER_H
#define PACKETCHAINMANAGER_H

#include <atomic>
#include <functional>
#include <map>

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_unordered_map.h>

#include "CheesePacket.h"
#include "EncryptionManager.h"
#include "ForwardManager.h"
#include "PacketVector.h"
#include "XfrmManager.h"

class PacketChainManager {
public:
    explicit PacketChainManager(EncryptionManager *encryptionManager, ForwardManager *forwardManager, XfrmManager *xfrmManager)
        : _encryptionManager(encryptionManager)
        , _xfrmManager(xfrmManager)
        , _forwardManager(forwardManager)
    {
    }

    enum PacketState : std::uint32_t {
        CHAIN_LINK_INPUT = 0,
        CHAIN_LINK_DECRYPT,
        CHAIN_LINK_XFRM,
        CHAIN_LINK_FORWARD,
        CHAIN_LINK_END,

        CHAIN_NET_INPUT,
        CHAIN_NET_XFRM,
        CHAIN_NET_ENCRYPT,
        CHAIN_NET_FORWARD,
        CHAIN_NET_END,

        CHAIN_STATES_COUNT,
    };

    bool linkInput(PacketVector *packet);
    bool netInput(PacketVector *packet);

private:
    void processNextPacket();
    bool moveNext(PacketVector *packet, PacketState packetState);
    bool executeHooks(PacketVector *packet, PacketState state);
    void endPacket(PacketVector *packet);

    bool packetLinkDecrpt(PacketVector *packet);
    bool packetLinkXfrm(PacketVector *packet);
    bool packetLinkForward(PacketVector *packet);
    bool packetLinkEnd(PacketVector *packet);

    bool packetNetXfrm(PacketVector *packet);
    bool packetNetEncrypt(PacketVector *packet);
    bool packetNetForward(PacketVector *packet);
    bool packetNetEnd(PacketVector *packet);

    EncryptionManager *_encryptionManager;
    XfrmManager *_xfrmManager;
    ForwardManager *_forwardManager;

    tbb::concurrent_bounded_queue<std::tuple<PacketVector *, PacketState>> _pendingPackets;
    std::vector<std::function<bool(PacketVector *)>> _chainHooks[CHAIN_STATES_COUNT];
};

PacketChainManager::PacketState &operator++(PacketChainManager::PacketState &state);

inline PacketChainManager::PacketState operator++(PacketChainManager::PacketState &state, int)
{
    PacketChainManager::PacketState s(state);

    return ++s;
}

#endif // PACKETCHAINMANAGER_H
