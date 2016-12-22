#include "LogonManager.hpp"
#include "LogonEnums.hpp"
#include "LogonOpcodes.hpp"
#include "LogonPackets.hpp"
#include "../Crypto/BigNumber.hpp"
#include "../Crypto/SRP6.hpp"
#include "../Network/ByteBuffer.hpp"

namespace RCN { namespace Logon {

    /// Constructor
    /// @p_Server   : Server address
    /// @p_Port     : Auth server port
    /// @p_User     : Username
    /// @p_Password : Password
    LogonManager::LogonManager(std::string p_Server, uint16_t p_Port, std::string p_User, std::string p_Password)
        : m_Server(p_Server), m_Port(p_Port), m_User(p_User), m_Password(p_Password), m_Connecting(false)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Do the connection
    bool LogonManager::Connect()
    {
        if (!m_Client.Connect(m_Server, m_Port))
        {
            printf("[AUTH] failed to connect to %s\n", m_Server.c_str());
            return false;
        }

        SendAuthLogonChallenge();

        m_Connecting    = true;
        m_IsAuthed      = false;

        while (m_Connecting)
        {
            uint8_t l_Command = 0;
            m_Client.Receive(&l_Command, sizeof(uint8_t));

            switch (static_cast<Opcodes::Type>(l_Command))
            {
                case Opcodes::CMD_AUTH_LOGON_CHALLENGE:
                    printf("[AUTH] Received Opcodes::CMD_AUTH_LOGON_CHALLENGE\n");
                    HandleAuthLogonChallenge();
                    break;
                case Opcodes::CMD_AUTH_LOGON_PROOF:
                    printf("[AUTH] Received Opcodes::CMD_AUTH_LOGON_PROOF\n");
                    HandleAuthLogonProof();
                    break;
                case Opcodes::CMD_AUTH_RECONNECT_CHALLENGE:
                    printf("[AUTH] Received UNHANDLED Opcodes::CMD_AUTH_RECONNECT_CHALLENGE\n");
                    break;
                case Opcodes::CMD_AUTH_RECONNECT_PROOF:
                    printf("[AUTH] Received UNHANDLED Opcodes::CMD_AUTH_RECONNECT_PROOF\n");
                    break;
                case Opcodes::CMD_REALM_LIST:
                    printf("[AUTH] Received Opcodes::CMD_REALM_LIST\n");
                    HandleRealmList();
                    break;
                case Opcodes::CMD_XFER_INITIATE:
                    printf("[AUTH] Received UNHANDLED Opcodes::CMD_XFER_INITIATE\n");
                    break;
                case Opcodes::CMD_XFER_DATA:
                    printf("[AUTH] Received UNHANDLED Opcodes::CMD_XFER_DATA\n");
                    break;
                case Opcodes::CMD_XFER_ACCEPT:
                    printf("[AUTH] Received UNHANDLED Opcodes::CMD_XFER_ACCEPT\n");
                    break;
                case Opcodes::CMD_XFER_RESUME:
                    printf("[AUTH] Received UNHANDLED Opcodes::CMD_XFER_RESUME\n");
                    break;
                case Opcodes::CMD_XFER_CANCEL:
                    printf("[AUTH] Received UNHANDLED Opcodes::CMD_XFER_CANCEL\n");
                    break;
                default:
                    printf("[AUTH] Received UNK %u\n", l_Command);
                    break;
            }
        }

        m_Client.Disconnect();

        return m_IsAuthed;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Send Opcodes::CMD_AUTH_LOGON_CHALLENGE
    void LogonManager::SendAuthLogonChallenge()
    {
        uint8_t l_GameName[]        = "WoW\0";
        uint8_t l_PlatformName[]    = "68x\0";
        uint8_t l_OSName[]          = "niW\0";
        uint8_t l_CountryName[]     = "BGne";

        union IPAddress
        {
            uint32_t Packed;
            uint8_t Bytes[4];
        } l_IPAddress;

        l_IPAddress.Bytes[0] = 127;
        l_IPAddress.Bytes[1] = 0;
        l_IPAddress.Bytes[2] = 0;
        l_IPAddress.Bytes[3] = 1;

        Client_AuthLogonChallenge l_AuthLogonChallengePacket;
        l_AuthLogonChallengePacket._Header.Opcode   = Opcodes::CMD_AUTH_LOGON_CHALLENGE;
        l_AuthLogonChallengePacket.Error            = 3;
        l_AuthLogonChallengePacket.Size             = 30 + m_User.length();
        memcpy(l_AuthLogonChallengePacket.GameName, l_GameName, 4);
        l_AuthLogonChallengePacket.Version1         = 1;
        l_AuthLogonChallengePacket.Version2         = 12;
        l_AuthLogonChallengePacket.Version3         = 1;
        l_AuthLogonChallengePacket.Build            = 5875;
        memcpy(l_AuthLogonChallengePacket.Platform, l_PlatformName, 4);
        memcpy(l_AuthLogonChallengePacket.OS,       l_OSName,       4);
        memcpy(l_AuthLogonChallengePacket.Country,  l_CountryName,  4);
        l_AuthLogonChallengePacket.TimezoneBias     = 1;
        l_AuthLogonChallengePacket.IP               = l_IPAddress.Packed;
        l_AuthLogonChallengePacket.UsernameLen      = m_User.length();
        memcpy(l_AuthLogonChallengePacket.Username, m_User.c_str(), min((size_t)15, (size_t)m_User.c_str()));

        m_Client.Send(reinterpret_cast<uint8_t*>(&l_AuthLogonChallengePacket), 4 + 30 + m_User.length());

        printf("[AUTH] Sent Opcodes::CMD_AUTH_LOGON_CHALLENGE\n");
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Handle Opcodes::CMD_AUTH_LOGON_CHALLENGE
    void LogonManager::HandleAuthLogonChallenge()
    {
        Server_GruntAuthChallenge l_Result;
        m_Client.Receive(reinterpret_cast<uint8_t*>(&l_Result), sizeof(Server_GruntAuthChallenge));

        if (l_Result.Result != AuthResult::WOW_SUCCESS)
        {
            m_Connecting = false;
            printf("[AUTH] Auth failed : %s(%u)\n", AuthResultStr[l_Result.Result], l_Result.Result);
            return;
        }

        Server_GruntAuthChallenge_OnSuccess l_CryptoContext;
        m_Client.Receive(reinterpret_cast<uint8_t*>(&l_CryptoContext), sizeof(Server_GruntAuthChallenge_OnSuccess));

        Crypto::BigNumber l_B;
        Crypto::BigNumber l_G;
        Crypto::BigNumber l_N;
        Crypto::BigNumber l_Salt;

        l_B.SetBinary(l_CryptoContext.B, 32);
        l_G.SetBinary(l_CryptoContext.G, l_CryptoContext.GLen);
        l_N.SetBinary(l_CryptoContext.N, l_CryptoContext.NLen);
        l_Salt.SetBinary(l_CryptoContext.Salt, 32);

        Crypto::SRP6 l_SRP6(l_N, l_G);
        Crypto::BigNumber l_AuthHash = l_SRP6.GetLogonHash(m_User, m_Password);

        Crypto::BigNumber l_A;
        Crypto::BigNumber l_S;
        do 
        {
            Crypto::BigNumber l_a;
            l_a.SetRand(19 * 8);

            l_A = l_SRP6.GetA(l_a);

            Crypto::BigNumber l_x = l_SRP6.Getx(l_Salt, l_AuthHash);
            Crypto::BigNumber l_u = l_SRP6.Getu(l_A, l_B);
            l_S = l_SRP6.GetClientS(l_a, l_B, l_x, l_u);
        } while (l_S.AsDword() < 0);

        SessionKey = l_SRP6.SHAInterleave(l_S);
        Crypto::BigNumber l_M = l_SRP6.GetM(m_User, l_Salt, l_A, l_B, SessionKey);

        Client_AuthLogonProof l_Response;
        l_Response._Header.Opcode   = Opcodes::CMD_AUTH_LOGON_PROOF;
        memcpy(l_Response.A, l_A.AsByteArray(32), 32);
        memcpy(l_Response.M1, l_M.AsByteArray(20), 20);
        memset(l_Response.CRCHash, 0, 20);
        l_Response.SurveyID         = 0;
        l_Response.SecurityFlags    = l_CryptoContext.SecurityFlag;

        m_Client.Send(reinterpret_cast<uint8_t*>(&l_Response), sizeof(Client_AuthLogonProof));

        printf("[AUTH] SessionKey : %s\n", SessionKey.AsHexStr());
        printf("[AUTH] Sent Opcodes::CMD_AUTH_LOGON_PROOF\n");
    }
    /// Handle Opcodes::CMD_AUTH_LOGON_PROOF
    void LogonManager::HandleAuthLogonProof()
    {
        Server_GruntAuthProof l_Result;
        m_Client.Receive(reinterpret_cast<uint8_t*>(&l_Result), sizeof(Server_GruntAuthProof));

        if (l_Result.Result != AuthResult::WOW_SUCCESS)
        {
            m_Connecting = false;
            printf("[AUTH] Auth failed : %s(%u)\n", AuthResultStr[l_Result.Result], l_Result.Result);
            return;
        }

        Server_GruntAuthProof_OnSuccess l_CryptoResult;
        m_Client.Receive(reinterpret_cast<uint8_t*>(&l_CryptoResult), sizeof(Server_GruntAuthProof_OnSuccess));

        Client_AuthRealmList l_Query;
        l_Query._Header.Opcode  = Opcodes::CMD_REALM_LIST;
        l_Query.Unk             = 0x00;

        m_Client.Send(reinterpret_cast<uint8_t*>(&l_Query), sizeof(Client_AuthRealmList));

        printf("[AUTH] Sent Opcodes::CMD_REALM_LIST\n");
    }
    /// Handle Opcodes::CMD_REALM_LIST
    void LogonManager::HandleRealmList()
    {
        Server_AuthRealmList l_Result;
        m_Client.Receive(reinterpret_cast<uint8_t*>(&l_Result), sizeof(Server_AuthRealmList));

        uint8_t* l_Buffer = new uint8_t[l_Result.Size];
        m_Client.Receive(l_Buffer, l_Result.Size);

        Network::ByteBuffer l_Packet;
        l_Packet.Append(l_Buffer, l_Result.Size);
        l_Packet.ReadPosition(0);
        delete[] l_Buffer;

        uint32_t    l_UnusedValue   = l_Packet.Read<uint32_t>();
        uint8_t     l_RealmCount    = l_Packet.Read<uint8_t>();

        for (size_t l_I = 0; l_I < l_RealmCount; ++l_I)
        {
            RealmInfo l_Info;
            l_Packet >> l_Info.Icon;
            l_Packet >> l_Info.Flags;
            l_Packet >> l_Info.Name;
            l_Packet >> l_Info.Address;
            l_Packet >> l_Info.PopulationLevel;
            l_Packet >> l_Info.AmountOfCharacters;
            l_Packet >> l_Info.TimeZone;
            l_Packet >> l_Info.UnkRealmID;

            Realms.push_back(l_Info);
        }

        uint16_t    l_UnusedValue2  = l_Packet.Read<uint16_t>();

        m_Connecting    = false;
        m_IsAuthed      = true;
    }

}   ///< namespace Logon
}   ///< namespace RCN