#pragma once

#include "../../Network/TCPClient.hpp"
#include "../../Logon/LogonManager.hpp"
#include "../../Crypto/WoWCrypt.hpp"
#include "WorldHeaders.hpp"
#include "WorldPacket.hpp"
#include <thread>

namespace RCN { namespace World {

    class WorldManager;

namespace Network {

    class WorldSocket
    {
        public:
            /// Constructor
            /// @p_Manager : Manager
            WorldSocket(WorldManager* p_Manager);
            /// Destructor
            ~WorldSocket();

            /// Do the connection
            /// @p_Realm : Realm
            /// @p_Key   : Session key
            bool Connect(Logon::RealmInfo p_Realm, Crypto::BigNumber p_SessionKey);

            /// Send a packet
            /// @p_Packet : Packet to send
            void SendPacket(WorldPacket& p_Packet);

            /// Init WoWCrypt
            void InitWoWCrypt();

            /// Is connected
            bool IsConnected() const;
            /// Get session key
            Crypto::BigNumber GetSessionKey();

        private:
            /// Receive thread
            void ReceiveThread();

        private:
            WorldManager          * m_Manager;              ///< World manager
            RCN::Network::TCPClient m_Socket;               ///< Socket
            Crypto::BigNumber       m_SessionKey;           ///< Session key
            Crypto::WoWCrypt        m_Crypto;               ///< Packet crypto
            std::thread           * m_ReceiveThread;        ///< Receive thread
            bool                    m_ReceiveThreadCond;    ///< Receive thread condition
            bool                    m_IsConnected;          ///< Is the socket connected
    };

}   ///< namespace Network
}   ///< namespace World
}   ///< namespace RCN