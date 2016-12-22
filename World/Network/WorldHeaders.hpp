#pragma once

#include "../../Network/TCPClient.hpp"

namespace RCN { namespace World { namespace Network {

#if defined( __GNUC__ )
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

    struct ServerPktHeader
    {
        uint16_t size;
        uint16_t cmd;
    };

    struct ClientPktHeader
    {
        uint16_t size;
        uint32_t cmd;
    };

#if defined( __GNUC__ )
#pragma pack()
#else
#pragma pack(pop)
#endif

}   ///< namespace Network
}   ///< namespace World
}   ///< namespace RCN
