#ifndef PACKETCHAINMANAGER_H
#define PACKETCHAINMANAGER_H

#include <atomic>
#include <functional>
#include <map>
#include <mutex>
#include <thread>

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/task_group.h>
#include <tbb/task_scheduler_init.h>

#include "CheesePacket.h"
#include "EncodeManager.h"
#include "EncryptionManager.h"
#include "ForwardManager.h"
#include "PacketVector.h"
#include "XfrmManager.h"

class PacketChainManager {
public:
    explicit PacketChainManager(EncryptionManager *encryptionManager,
        ForwardManager *forwardManager,
        XfrmManager *xfrmManager,
        int processThreads = 0)
        : _encryptionManager(encryptionManager)
        , _xfrmManager(xfrmManager)
        , _forwardManager(forwardManager)
        , _runProcessing(false)
        , _threadGroupOperationLock()
    {
        if (processThreads == 0) {
            _threadCount = std::thread::hardware_concurrency();
        } else if (processThreads < 0) {
            int count = std::thread::hardware_concurrency() + processThreads;
            _threadCount = count <= 0 ? 1 : count;
        } else if (processThreads > 0) {
            _threadCount = processThreads;
        }
    }

    enum PacketState : std::uint32_t {
        CHAIN_LINK_INPUT = 0,
        CHAIN_LINK_DECODE,
        CHAIN_LINK_DECRYPT,
        CHAIN_LINK_XFRM,
        CHAIN_LINK_FORWARD,
        CHAIN_LINK_END,

        CHAIN_NET_INPUT,
        CHAIN_NET_XFRM,
        CHAIN_NET_ENCRYPT,
        CHAIN_NET_ENCODE,
        CHAIN_NET_FORWARD,
        CHAIN_NET_END,

        CHAIN_STATES_COUNT,
    };

    bool linkInput(PacketVector *packet);
    bool netInput(PacketVector *packet);

    void startProcessing();
    void stopProcessing();

private:
    void processNextPacket();
    bool moveNext(PacketVector *packet, PacketState packetState);
    bool executeHooks(PacketVector *packet, PacketState state);
    void endPacket(PacketVector *packet);

    bool packetLinkDecode(PacketVector *packet);
    bool packetLinkDecrpt(PacketVector *packet);
    bool packetLinkXfrm(PacketVector *packet);
    bool packetLinkForward(PacketVector *packet);
    bool packetLinkEnd(PacketVector *packet);

    bool packetNetXfrm(PacketVector *packet);
    bool packetNetEncrypt(PacketVector *packet);
    bool packetNetEncode(PacketVector *packet);
    bool packetNetForward(PacketVector *packet);
    bool packetNetEnd(PacketVector *packet);

    void processLoop();

    EncryptionManager *_encryptionManager;
    XfrmManager *_xfrmManager;
    ForwardManager *_forwardManager;
    EncodeManager _encodeManager;

    tbb::concurrent_bounded_queue<std::tuple<PacketVector *, PacketState>> _pendingPackets;
    std::vector<std::function<bool(PacketVector *)>> _chainHooks[CHAIN_STATES_COUNT];
    volatile bool _runProcessing;
    std::mutex _threadGroupOperationLock;
    std::vector<std::thread> _threadGroup;
    unsigned int _threadCount;
};

PacketChainManager::PacketState &operator++(PacketChainManager::PacketState &state);

inline PacketChainManager::PacketState operator++(PacketChainManager::PacketState &state, int)
{
    PacketChainManager::PacketState s(state);

    return ++s;
}

#endif // PACKETCHAINMANAGER_H
