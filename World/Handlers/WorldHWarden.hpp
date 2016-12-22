#pragma once

#include "../Network/WorldPacket.hpp"
#include "../WorldManager.hpp"

namespace RCN { namespace World { namespace Handlers {

    namespace WardenOpcodes
    {
        enum Type
        {
            /// Client->Server
            WARDEN_CMSG_MODULE_MISSING                  = 0,
            WARDEN_CMSG_MODULE_OK                       = 1,
            WARDEN_CMSG_CHEAT_CHECKS_RESULT             = 2,
            WARDEN_CMSG_MEM_CHECKS_RESULT               = 3,
            WARDEN_CMSG_HASH_RESULT                     = 4,
            WARDEN_CMSG_MODULE_FAILED                   = 5,

            /// Server->Client
            WARDEN_SMSG_MODULE_USE                      = 0,
            WARDEN_SMSG_MODULE_CACHE                    = 1,
            WARDEN_SMSG_CHEAT_CHECKS_REQUEST            = 2,
            WARDEN_SMSG_MODULE_INITIALIZE               = 3,
            WARDEN_SMSG_MEM_CHECKS_REQUEST              = 4,
            WARDEN_SMSG_HASH_REQUEST                    = 5
        };
    }

    namespace WardenCheckType
    {
        enum Type
        {
            Timing      = 0x57,             ///<  87: empty (check to ensure GetTickCount() isn't detoured)
            Driver      = 0x71,             ///< 113: uint Seed + byte[20] SHA1 + byte driverNameIndex (check to ensure driver isn't loaded)
            Proc        = 0x7E,             ///< 126: uint Seed + byte[20] SHA1 + byte moluleNameIndex + byte procNameIndex + uint Offset + byte Len (check to ensure proc isn't detoured)
            LuaStr      = 0x8B,             ///< 139: byte luaNameIndex (check to ensure LUA string isn't used)
            Mpq         = 0x98,             ///< 152: byte fileNameIndex (check to ensure MPQ file isn't modified)
            PageCheckA  = 0xB2,             ///< 178: uint Seed + byte[20] SHA1 + uint Addr + byte Len (scans all pages for specified hash)
            PageCheckB  = 0xBF,             ///< 191: uint Seed + byte[20] SHA1 + uint Addr + byte Len (scans only pages starts with MZ+PE headers for specified hash)
            Module      = 0xD9,             ///< 217: uint Seed + byte[20] SHA1 (check to ensure module isn't injected)
            Memory      = 0xF3,             ///< 243: byte moduleNameIndex + uint Offset + byte Len (check to ensure memory isn't modified)
        };
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Handle SMSG_WARDEN_DATA
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    void Handle_WardenData(Network::WorldPacket& p_Packet, WorldManager* p_Manager);

}   ///< namespace Handlers
}   ///< namespace World
}   ///< namespace RCN#pragma once
