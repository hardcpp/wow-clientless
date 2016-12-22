#pragma once

#include "../Network/WorldPacket.hpp"
#include "../WorldManager.hpp"

namespace RCN { namespace World { namespace Handlers {

    /// Handle SMSG_MESSAGECHAT
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    void Handle_MessageChat(Network::WorldPacket& p_Packet, WorldManager* p_Manager);

}   ///< namespace Handlers
}   ///< namespace World
}   ///< namespace RCN#pragma once
