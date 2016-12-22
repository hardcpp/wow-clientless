#pragma once

#include "../Network/WorldPacket.hpp"
#include "../WorldManager.hpp"

namespace RCN { namespace World { namespace Handlers {

    /// Handle SMSG_UPDATE_OBJECT
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    void Handle_UpdateObject(Network::WorldPacket& p_Packet, WorldManager* p_Manager);
    /// Handle SMSG_COMPRESSED_UPDATE_OBJECT
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    void Handle_CompressedUpdateObject(Network::WorldPacket& p_Packet, WorldManager* p_Manager);
    /// Handle SMSG_DESTROY_OBJECT
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    void Handle_DestroyObject(Network::WorldPacket& p_Packet, WorldManager* p_Manager);

}   ///< namespace Handlers
}   ///< namespace World
}   ///< namespace RCN#pragma once
