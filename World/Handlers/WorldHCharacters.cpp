#include "WorldHCharacters.hpp"
#include "../Enums/WorldEnums.hpp"
#include "../Network/WorldOpcodes.hpp"

namespace RCN { namespace World { namespace Handlers {

    /// Handle SMSG_AUTH_CHALLENGE
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    void Handle_CharEnum(Network::WorldPacket& p_Packet, WorldManager* p_Manager)
    {
        uint8_t l_Num;
        p_Packet >> l_Num;

        for (uint8_t l_I = 0; l_I < l_Num; ++l_I)
        {
            std::string l_Name      = "";
            uint64_t    l_GUID      = 0;
            uint8_t     l_Level     = 0;
            float       l_X         = 0.0f;
            float       l_Y         = 0.0f;
            float       l_Z         = 0.0f;

            p_Packet >> l_GUID;
            p_Packet >> l_Name;
            p_Packet.ReadSkip<uint8_t>();           ///< Race
            p_Packet.ReadSkip<uint8_t>();           ///< Class
            p_Packet.ReadSkip<uint8_t>();           ///< Gender

            p_Packet.ReadSkip<uint8_t>();           ///< Skin
            p_Packet.ReadSkip<uint8_t>();           ///< Face
            p_Packet.ReadSkip<uint8_t>();           ///< HairStyle
            p_Packet.ReadSkip<uint8_t>();           ///< HairColor
            p_Packet.ReadSkip<uint8_t>();           ///< FacialHair

            p_Packet >> l_Level;
            p_Packet.ReadSkip<uint32_t>();          ///< Zone
            p_Packet.ReadSkip<uint32_t>();          ///< Map

            p_Packet >> l_X;
            p_Packet >> l_Y;
            p_Packet >> l_Z;

            p_Packet.ReadSkip<uint32_t>();          ///< GuildID
            p_Packet.ReadSkip<uint32_t>();          ///< CharFlags

            p_Packet.ReadSkip<uint32_t>();          ///< PetDisplayID
            p_Packet.ReadSkip<uint32_t>();          ///< PetLevel
            p_Packet.ReadSkip<uint32_t>();          ///< PetFamily

            for (size_t l_Slot = Enums::EquipmentSlots::EQUIPMENT_SLOT_START;
                l_Slot < Enums::EquipmentSlots::EQUIPMENT_SLOT_END;
                ++l_Slot)
            {

                p_Packet.ReadSkip<uint32_t>();      ///< DisplayID
                p_Packet.ReadSkip<uint8_t>();       ///< InventoryType
            }

            p_Packet.ReadSkip<uint32_t>();          ///< First bag display ID
            p_Packet.ReadSkip<uint8_t>();           ///< Fist bag inventory type

            printf("[WORLD] Character %s(%llu) Level %u X %f Y %f Z %f\n", l_Name.c_str(), l_GUID, l_Level, l_X, l_Y, l_Z);

            Network::WorldPacket l_Test(Network::Opcodes::CMSG_PLAYER_LOGIN);
            l_Test << l_GUID;

            p_Manager->GetWorldSocket()->SendPacket(l_Test);
            return;
        }
    }

}   ///< namespace Handlers
}   ///< namespace World
}   ///< namespace RCN