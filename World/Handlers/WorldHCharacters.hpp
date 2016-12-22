#pragma once

#include "../Network/WorldPacket.hpp"
#include "../WorldManager.hpp"

namespace RCN { namespace World { namespace Handlers {

    /// Handle SMSG_AUTH_CHALLENGE
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    void Handle_CharEnum(Network::WorldPacket& p_Packet, WorldManager* p_Manager);

}   ///< namespace Handlers
}   ///< namespace World
}   ///< namespace RCN#pragma once
