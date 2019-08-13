#include "PacketChainManager.h"

bool PacketChainManager::linkInput(PacketVector *packet)
{
    if (executeHooks(packet, CHAIN_LINK_INPUT)) {
        _pendingPackets.push(std::make_tuple(packet, CHAIN_LINK_INPUT));
        return true;
    } else {
        return false;
    }
}

bool PacketChainManager::netInput(PacketVector *packet)
{
    if (executeHooks(packet, CHAIN_NET_INPUT)) {
        _pendingPackets.push(std::make_tuple(packet, CHAIN_NET_INPUT));
        return true;
    } else {
        return false;
    }
}

void PacketChainManager::processNextPacket()
{
    std::tuple<PacketVector *, PacketState> tup;
    _pendingPackets.pop(tup);

    PacketVector *pak = std::get<0>(tup);
    PacketState state = std::get<1>(tup);

    bool canContinue = false;
    do {
        canContinue = moveNext(pak, state);
    } while (canContinue);

    endPacket(pak);
}

bool PacketChainManager::moveNext(PacketVector *packet, PacketState packetState)
{
    ++packetState;

    switch (packetState) {
    case CHAIN_LINK_END:
        packetLinkEnd(packet) && executeHooks(packet, packetState);
        return false;

    case CHAIN_NET_END:
        packetLinkEnd(packet) && executeHooks(packet, packetState);
        return false;

    case CHAIN_LINK_INPUT:
    case CHAIN_NET_INPUT:
        throw; // This is not happening

    case CHAIN_LINK_DECRYPT:
        return packetLinkDecrpt(packet) && executeHooks(packet, packetState);
    case CHAIN_LINK_XFRM:
        return packetLinkXfrm(packet) && executeHooks(packet, packetState);
    case CHAIN_LINK_FORWARD:
        return packetLinkForward(packet) && executeHooks(packet, packetState);
    case CHAIN_NET_XFRM:
        return packetNetXfrm(packet) && executeHooks(packet, packetState);
    case CHAIN_NET_ENCRYPT:
        return packetNetEncrypt(packet) && executeHooks(packet, packetState);
    case CHAIN_NET_FORWARD:
        return packetNetForward(packet) && executeHooks(packet, packetState);
    default:
        return false;
    }
}

bool PacketChainManager::executeHooks(PacketVector *packet, PacketChainManager::PacketState state)
{
    auto &hooks = _chainHooks[state];

    for (auto &hook : hooks) {
        if (hook(packet))
            return false;
    }
    return true;
}

void PacketChainManager::endPacket(PacketVector *packet)
{
    delete packet;
}

bool PacketChainManager::packetLinkDecrpt(PacketVector *packet)
{
    return _encryptionManager->decryptPacket(packet);
}

bool PacketChainManager::packetLinkXfrm(PacketVector *packet)
{
    return _xfrmManager->link2net(packet);
}

bool PacketChainManager::packetLinkForward(PacketVector *packet)
{
    return _forwardManager->linkForward(packet);
}

bool PacketChainManager::packetLinkEnd(PacketVector *packet)
{
    (void)packet;
    return true;
}

bool PacketChainManager::packetNetXfrm(PacketVector *packet)
{
    return _xfrmManager->net2link(packet);
}

bool PacketChainManager::packetNetEncrypt(PacketVector *packet)
{
    return _encryptionManager->encryptPacket(packet);
}

bool PacketChainManager::packetNetForward(PacketVector *packet)
{
    return _forwardManager->netForward(packet);
}

bool PacketChainManager::packetNetEnd(PacketVector *packet)
{
    (void)packet;
    return true;
}

PacketChainManager::PacketState &operator++(PacketChainManager::PacketState &state)
{
    switch (state) {
    case PacketChainManager::CHAIN_LINK_INPUT:
    case PacketChainManager::CHAIN_LINK_DECRYPT:
    case PacketChainManager::CHAIN_LINK_XFRM:
    case PacketChainManager::CHAIN_LINK_FORWARD:
    case PacketChainManager::CHAIN_NET_INPUT:
    case PacketChainManager::CHAIN_NET_XFRM:
    case PacketChainManager::CHAIN_NET_ENCRYPT:
    case PacketChainManager::CHAIN_NET_FORWARD:
        state = static_cast<PacketChainManager::PacketState>(static_cast<std::uint32_t>(state) + 1);
        break;
    case PacketChainManager::CHAIN_LINK_END:
    case PacketChainManager::CHAIN_NET_END:
    case PacketChainManager::CHAIN_STATES_COUNT:
        break;
    default:
        state = PacketChainManager::CHAIN_STATES_COUNT;
        break;
    }

    return state;
}
