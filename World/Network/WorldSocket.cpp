#include "WorldSocket.hpp"
#include "WorldOpcodes.hpp"
#include "../WorldManager.hpp"
#include "../../Utils/UtilsString.hpp"
#include "../../Utils/UtilsEndian.hpp"

namespace RCN { namespace World { namespace Network {

    /// Constructor
    /// @p_Manager : Manager
    WorldSocket::WorldSocket(WorldManager* p_Manager)
        : m_Manager(p_Manager), m_ReceiveThread(nullptr), m_ReceiveThreadCond(false)
    {

    }
    /// Destructor
    WorldSocket::~WorldSocket()
    {
        if (m_ReceiveThread)
        {
            m_ReceiveThreadCond = false;
            m_ReceiveThread->join();
            delete m_ReceiveThread;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Do the connection
    /// @p_Realm : Realm
    /// @p_Key   : Session key
    bool WorldSocket::Connect(Logon::RealmInfo p_Realm, Crypto::BigNumber p_SessionKey)
    {
        auto l_Parts = Utils::Split(p_Realm.Address, ":", false);

        std::string l_Host = l_Parts[0];
        uint16_t    l_Port = atoi(l_Parts[1].c_str());

        printf("[WORLD] Connecting to world server %s:%u\n", l_Host.c_str(), l_Port);

        if (!m_Socket.Connect(l_Host, l_Port))
            return false;

        printf("[WORLD] Connected !\n");

        m_SessionKey    = p_SessionKey;
        m_IsConnected   = true;

        m_ReceiveThreadCond = true;
        m_ReceiveThread     = new std::thread(&WorldSocket::ReceiveThread, this);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Send a packet
    /// @p_Packet : Packet to send
    void WorldSocket::SendPacket(WorldPacket& p_Packet)
    {
        ClientPktHeader l_Header;
        l_Header.cmd    = p_Packet.GetOpcode();
        l_Header.size   = static_cast<uint16_t>(p_Packet.GetSize() + 4);

        Utils::ByteConverter::Apply(&l_Header.size);  ///< Endian reverse

        m_Crypto.Encrypt(reinterpret_cast<uint8_t*>(&l_Header), sizeof(l_Header));

        WorldPacket l_Copy;
        l_Copy.Append(reinterpret_cast<uint8_t*>(&l_Header), sizeof(l_Header));

        if (p_Packet.GetSize())
            l_Copy.Append(p_Packet.GetData(), p_Packet.GetSize());

        if (!m_Socket.Send(l_Copy.GetData(), l_Copy.GetSize()))
        {
            printf("[WORLD] Socket error\n");
            m_ReceiveThreadCond = false;
            m_IsConnected       = false;
        }
        else
            printf("[WORLD] Send packet %s(%u) size %u\n", OpcodesName[p_Packet.GetOpcode()], p_Packet.GetOpcode(), p_Packet.GetSize());
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Init WoWCrypt
    void WorldSocket::InitWoWCrypt()
    {
        m_Crypto.Init();
        m_Crypto.SetKey(m_SessionKey.AsByteArray(), m_SessionKey.GetNumBytes());
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Is connected
    bool WorldSocket::IsConnected() const
    {
        return m_IsConnected;
    }
    /// Get session key
    Crypto::BigNumber WorldSocket::GetSessionKey()
    {
        return m_SessionKey;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Receive thread
    void WorldSocket::ReceiveThread()
    {
        while (m_ReceiveThreadCond)
        {
            ServerPktHeader l_Header;

            if (!m_Socket.Receive(reinterpret_cast<uint8_t*>(&l_Header), sizeof(ServerPktHeader)))
            {
                printf("[WORLD] Socket error\n");
                m_ReceiveThreadCond = false;
                m_IsConnected = false;
            }

            m_Crypto.Decrypt(reinterpret_cast<uint8_t*>(&l_Header), sizeof(ServerPktHeader));

            Utils::ByteConverter::Apply(&l_Header.size);    ///< Endian reverse
            l_Header.size -= 2;                             ///< Don't count Size in size

            uint8_t* l_Data = new uint8_t[l_Header.size];
            m_Socket.Receive(l_Data, l_Header.size);

            WorldPacket l_Packet;
            l_Packet.Append(l_Data, l_Header.size);
            l_Packet.ReadPosition(0);
            l_Packet.SetOpcode(l_Header.cmd);

            delete[] l_Data;

            if (l_Header.cmd >= Opcodes::MAX)
            {
                printf("[WORLD] Socket error\n");
                m_ReceiveThreadCond = false;
                m_IsConnected       = false;
                break;
            }

            m_Manager->EnqueuePacket(std::move(l_Packet));

            Sleep(5);
        }
    }

}   ///< namespace Network
}   ///< namespace World
}   ///< namespace RCN