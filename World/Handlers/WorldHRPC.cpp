#include "WorldHChat.hpp"
#include "../Enums/WorldEnums.hpp"
#include "../Network/WorldOpcodes.hpp"
#include "../Entities/WorldEUnit.hpp"
#include <zlib/zlib.h>

namespace RCN { namespace World { namespace Handlers {

    /// Handle SMSG_UPDATE_OBJECT
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    void Handle_UpdateObject(Network::WorldPacket& p_Packet, WorldManager* p_Manager)
    {
        uint32_t    l_BlockCount    = p_Packet.Read<uint32_t>();
        uint8_t     l_HasTransport  = p_Packet.Read<uint8_t>();

        for (uint32_t l_I = 0; l_I < l_BlockCount; ++l_I)
        {
            Enums::ObjectUpdateType::Type l_BlockType = static_cast<Enums::ObjectUpdateType::Type>(p_Packet.Read<uint8_t>());

            if (l_BlockType == Enums::ObjectUpdateType::UPDATETYPE_VALUES)
            {
                uint64_t l_GUID = p_Packet.ReadPackGUID();

            }
            else if (l_BlockType == Enums::ObjectUpdateType::UPDATETYPE_MOVEMENT)
            {
                /// Nothing to do here
            }
            else if (l_BlockType == Enums::ObjectUpdateType::UPDATETYPE_CREATE_OBJECT
                  || l_BlockType == Enums::ObjectUpdateType::UPDATETYPE_CREATE_OBJECT2)
            {
                uint64_t    l_GUID          = p_Packet.ReadPackGUID();
                uint8_t     l_ObjectTypeID  = p_Packet.Read<uint8_t>();

                uint8_t l_UpdateFlags = p_Packet.Read<uint8_t>();

                Entities::MovementInfo l_MovementInfo;
                float l_Speeds[Enums::UnitMoveType::MAX];

                if ((l_UpdateFlags & Enums::ObjectUpdateFlags::UPDATEFLAG_LIVING) != 0)
                {
               //     l_MovementInfo.Read(p_Packet);

                    p_Packet >> l_Speeds[Enums::UnitMoveType::MOVE_WALK];
                    p_Packet >> l_Speeds[Enums::UnitMoveType::MOVE_RUN];
                    p_Packet >> l_Speeds[Enums::UnitMoveType::MOVE_RUN_BACK];
                    p_Packet >> l_Speeds[Enums::UnitMoveType::MOVE_SWIM];
                    p_Packet >> l_Speeds[Enums::UnitMoveType::MOVE_SWIM_BACK];
                    p_Packet >> l_Speeds[Enums::UnitMoveType::MOVE_TURN_RATE];



                }

                if ((l_UpdateFlags & Enums::ObjectUpdateFlags::UPDATEFLAG_HAS_POSITION) != 0)
                {

                }

                if ((l_UpdateFlags & Enums::ObjectUpdateFlags::UPDATEFLAG_HIGHGUID) != 0)
                    p_Packet.ReadSkip<uint32_t>();

                if ((l_UpdateFlags & Enums::ObjectUpdateFlags::UPDATEFLAG_ALL) != 0)
                    p_Packet.ReadSkip<uint32_t>();

                if ((l_UpdateFlags & Enums::ObjectUpdateFlags::UPDATEFLAG_FULLGUID) != 0)
                    p_Packet.ReadPackGUID();

                if ((l_UpdateFlags & Enums::ObjectUpdateFlags::UPDATEFLAG_TRANSPORT) != 0)
                    p_Packet.ReadSkip<uint32_t>();

            }
            else if (l_BlockType == Enums::ObjectUpdateType::UPDATETYPE_OUT_OF_RANGE_OBJECTS)
            {
                uint32_t l_ObjectCount = p_Packet.Read<uint32_t>();

                for (uint32_t l_Y = 0; l_Y < l_ObjectCount; ++l_Y)
                {
                    uint64_t l_GUID = p_Packet.ReadPackGUID();


                }
            }
            else if (l_BlockType == Enums::ObjectUpdateType::UPDATETYPE_NEAR_OBJECTS)
            {
                /// Nothing to do here
            }
        }
    }
    /// Handle SMSG_COMPRESSED_UPDATE_OBJECT
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    void Handle_CompressedUpdateObject(Network::WorldPacket& p_Packet, WorldManager* p_Manager)
    {
        uint32_t    l_DecompressedSizeUI32  = p_Packet.Read<uint32_t>();
        uLongf      l_DecompressedSize      = l_DecompressedSizeUI32;
        uint32_t    l_CompressedSize        = p_Packet.GetSize() - p_Packet.ReadPosition();
        uint8_t *   l_CompressedData        = new uint8_t[l_CompressedSize];

        p_Packet.Read(l_CompressedData, l_CompressedSize);

        uint8_t * l_UncompressedData = new uint8_t[l_DecompressedSizeUI32];

        if (uncompress(l_UncompressedData, &l_DecompressedSize, l_CompressedData, l_CompressedSize) == Z_OK)
        {
            Network::WorldPacket l_Packet(l_DecompressedSize);
            l_Packet.Append(l_UncompressedData, l_DecompressedSize);

            Handle_UpdateObject(l_Packet, p_Manager);
        }
        else
            printf("[WORLD] Decompression of SMSG_COMPRESSED_UPDATE_OBJECT failed !\n");

        delete[] l_UncompressedData;
        delete[] l_CompressedData;
    }
    /// Handle SMSG_DESTROY_OBJECT
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    void Handle_DestroyObject(Network::WorldPacket& p_Packet, WorldManager* p_Manager)
    {
        uint64_t l_GUID = p_Packet.Read<uint64_t>();

        /// @TODO
    }

}   ///< namespace Handlers
}   ///< namespace World
}   ///< namespace RCN