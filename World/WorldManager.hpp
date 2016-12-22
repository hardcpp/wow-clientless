#pragma once

#include "Network/WorldSocket.hpp"
#include <queue>
#include <mutex>

namespace RCN { namespace World {

    class WorldManager
    {
        public:
            /// Constructor
            WorldManager();
            /// Destructor
            ~WorldManager();

            /// Do the connection
            /// @p_Realm   : Realm
            /// @p_Key     : Session key
            /// @p_Account : Account name
            bool Connect(Logon::RealmInfo p_Realm, Crypto::BigNumber p_SessionKey, std::string p_Account);

            /// Run
            void Run();

            /// Enqueue a packet
            /// @p_Packet : World packet to enqueue
            void EnqueuePacket(Network::WorldPacket&& p_Packet);

        public:
            /// Get world socket
            Network::WorldSocket * GetWorldSocket();
            /// Get account name
            std::string GetAccountName();

        private:
            Network::WorldSocket  * m_Socket;   ///< World socket
            std::string             m_Account;  ///< Account name

            std::queue<Network::WorldPacket> m_PacketQueue;
            std::mutex                       m_PacketQueueMutex;
    };

}   ///< namespace World
}   ///< namespace RCN