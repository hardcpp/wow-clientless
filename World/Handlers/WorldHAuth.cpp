#include "WorldHAuth.hpp"
#include "../Enums/WorldEnums.hpp"
#include "../Network/WorldOpcodes.hpp"
#include "../../Crypto/SHA1.hpp"

namespace RCN { namespace World { namespace Handlers {

    /// Handle SMSG_AUTH_CHALLENGE
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    void Handle_AuthChallenge(Network::WorldPacket& p_Packet, WorldManager* p_Manager)
    {
        uint32_t l_ServerSeed = 0;
        p_Packet >> l_ServerSeed;

        uint32_t l_DigestUnk    = 0;
        time_t   l_ClientSeed   = time(nullptr);

        Crypto::SHA1 l_SHA1;
        l_SHA1.UpdateData(p_Manager->GetAccountName());
        l_SHA1.UpdateData(reinterpret_cast<const uint8_t*>(&l_DigestUnk),     sizeof(uint32_t));
        l_SHA1.UpdateData(reinterpret_cast<const uint8_t*>(&l_ClientSeed),    sizeof(uint32_t));
        l_SHA1.UpdateData(reinterpret_cast<const uint8_t*>(&l_ServerSeed),    sizeof(uint32_t));
        l_SHA1.UpdateBigNumbers(&p_Manager->GetWorldSocket()->GetSessionKey(), nullptr);
        l_SHA1.Finalize();

        static const uint8_t s_AddonData[] =
        {
            0x56, 0x01, 0x00, 0x00, 0x78, 0x9C, 0x75, 0xCC, 0xBD, 0x0E, 0xC2, 0x30, 0x0C, 0x04, 0xE0, 0xF2,
            0x1E, 0xBC, 0x0C, 0x61, 0x40, 0x95, 0xC8, 0x42, 0xC3, 0x8C, 0x4C, 0xE2, 0x22, 0x0B, 0xC7, 0xA9,
            0x8C, 0xCB, 0x4F, 0x9F, 0x1E, 0x16, 0x24, 0x06, 0x73, 0xEB, 0x77, 0x77, 0x81, 0x69, 0x59, 0x40,
            0xCB, 0x69, 0x33, 0x67, 0xA3, 0x26, 0xC7, 0xBE, 0x5B, 0xD5, 0xC7, 0x7A, 0xDF, 0x7D, 0x12, 0xBE,
            0x16, 0xC0, 0x8C, 0x71, 0x24, 0xE4, 0x12, 0x49, 0xA8, 0xC2, 0xE4, 0x95, 0x48, 0x0A, 0xC9, 0xC5,
            0x3D, 0xD8, 0xB6, 0x7A, 0x06, 0x4B, 0xF8, 0x34, 0x0F, 0x15, 0x46, 0x73, 0x67, 0xBB, 0x38, 0xCC,
            0x7A, 0xC7, 0x97, 0x8B, 0xBD, 0xDC, 0x26, 0xCC, 0xFE, 0x30, 0x42, 0xD6, 0xE6, 0xCA, 0x01, 0xA8,
            0xB8, 0x90, 0x80, 0x51, 0xFC, 0xB7, 0xA4, 0x50, 0x70, 0xB8, 0x12, 0xF3, 0x3F, 0x26, 0x41, 0xFD,
            0xB5, 0x37, 0x90, 0x19, 0x66, 0x8F
        };

        Network::WorldPacket l_Packet(Network::Opcodes::CMSG_AUTH_SESSION);
        l_Packet << uint32_t(5875);                                 ///< Build
        l_Packet << uint32_t(0);                                    ///< Unk 2
        l_Packet << p_Manager->GetAccountName();                    ///< Account name
        l_Packet << uint32_t(l_ClientSeed);                         ///< ClientSeed
        l_Packet.Append(l_SHA1.GetDigest(), SHA_DIGEST_LENGTH);     ///< Logon digest
        l_Packet.Append(&s_AddonData[0], sizeof(s_AddonData));      ///< Addon infos

        p_Manager->GetWorldSocket()->SendPacket(l_Packet);
        p_Manager->GetWorldSocket()->InitWoWCrypt();
    }
    /// Handle SMSG_AUTH_RESULT
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    void Handle_AuthResult(Network::WorldPacket& p_Packet, WorldManager* p_Manager)
    {
        uint8_t l_Result;
        p_Packet >> l_Result;

        if (l_Result == Enums::ResponseCodes::AUTH_OK)
        {
            printf("[WORLD] Auth success!!\n");

            uint32_t l_BillingTimeRemaining = 0;
            uint32_t l_BillingPlanFlags     = 0;
            uint32_t l_BillingTimeRested    = 0;

            p_Packet >> l_BillingTimeRemaining;
            p_Packet >> l_BillingPlanFlags;
            p_Packet >> l_BillingTimeRested;

            Network::WorldPacket l_Packet(Network::Opcodes::CMSG_CHAR_ENUM);
            p_Manager->GetWorldSocket()->SendPacket(l_Packet);
        }
        else
            printf("[WORLD] Auth failed : %u\n", l_Result);
    }

}   ///< namespace Handlers
}   ///< namespace World
}   ///< namespace RCN