#include "WorldManager.hpp"
#include "Network/WorldOpcodes.hpp"
#include <algorithm>

namespace RCN { namespace World {

    /// Constructor
    WorldManager::WorldManager()
    {
        Network::InitWorldNetworkOpcodes();

        m_Socket = new Network::WorldSocket(this);
    }
    /// Destructor
    WorldManager::~WorldManager()
    {
        delete m_Socket;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Do the connection
    /// @p_Realm   : Realm
    /// @p_Key     : Session key
    /// @p_Account : Account name
    bool WorldManager::Connect(Logon::RealmInfo p_Realm, Crypto::BigNumber p_SessionKey, std::string p_Account)
    {
        m_Account = p_Account;

        std::transform(m_Account.begin(), m_Account.end(), m_Account.begin(), ::toupper);

        if (!m_Socket->Connect(p_Realm, p_SessionKey))
            return false;

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Run
    void WorldManager::Run()
    {
        while (m_Socket->IsConnected())
        {
            /// Copy all packets in queue with 20 as max limit
            std::vector<Network::WorldPacket> l_Packets;
            {
                std::lock_guard<std::mutex> l_Lock(m_PacketQueueMutex);

                for (size_t l_I = 0; l_I < 20; ++l_I)
                {
                    if (m_PacketQueue.empty())
                        break;

                    l_Packets.push_back(std::move(m_PacketQueue.front()));
                    m_PacketQueue.pop();
                }
            }

            /// Handle packets
            for (auto& l_Packet : l_Packets)
            {
                printf("[WORLD] Receive packet %s(%u) size %u\n", Network::OpcodesName[l_Packet.GetOpcode()], l_Packet.GetOpcode(), l_Packet.GetSize());

                if (Network::OpcodesHandlers[l_Packet.GetOpcode()])
                    Network::OpcodesHandlers[l_Packet.GetOpcode()](l_Packet, this);
            }

            Sleep(5);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Enqueue a packet
    /// @p_Packet : World packet to enqueue
    void WorldManager::EnqueuePacket(Network::WorldPacket&& p_Packet)
    {
        std::lock_guard<std::mutex> l_Lock(m_PacketQueueMutex);
        m_PacketQueue.push(std::move(p_Packet));
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get world socket
    Network::WorldSocket * WorldManager::GetWorldSocket()
    {
        return m_Socket;
    }
    /// Get account name
    std::string WorldManager::GetAccountName()
    {
        return m_Account;
    }

}   ///< namespace World
}   ///< namespace RCN