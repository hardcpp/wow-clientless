#include "WorldHWarden.hpp"
#include "../Enums/WorldEnums.hpp"
#include "../Network/WorldOpcodes.hpp"
#include "../Warden/WorldWardenUtils.hpp"
#include "../../Crypto/WardenKeyGenerator.hpp"
#include "../../Crypto/ARC4.hpp"
#include "../../Crypto/SHA1.hpp"
#include <zlib/zlib.h>

namespace RCN { namespace World { namespace Handlers {

    struct WardenContext
    {
        WardenContext()
        {
            IsWardenCryptOK = false;
        }

        bool IsWardenCryptOK;
        Crypto::BigNumber EncryptKey;
        Crypto::BigNumber DecryptKey;
        Crypto::ARC4 * Encrypter;
        Crypto::ARC4 * Decrypter;
        uint8_t EncryptFirstByte;

        Crypto::BigNumber ModuleID;
        Crypto::BigNumber ModuleKey;
        uint32_t ModuleSize;

        RCN::Network::ByteBuffer WardenModule;
        RCN::Network::ByteBuffer WowModule;
    };

    /// Handle WardenOpcodes::WARDEN_SMSG_MODULE_USE
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    /// @p_Context  : Warden context
    void HandleWardenModuleUse(Network::WorldPacket& p_Packet, WorldManager* p_Manager, WardenContext* p_Context);
    /// Handle WardenOpcodes::WARDEN_SMSG_MODULE_CACHE
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    /// @p_Context  : Warden context
    void HandleWardenModuleCache(Network::WorldPacket& p_Packet, WorldManager* p_Manager, WardenContext* p_Context);
    /// Handle WardenOpcodes::WARDEN_SMSG_HASH_REQUEST
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    /// @p_Context  : Warden context
    void HandleWardenHashRequest(Network::WorldPacket& p_Packet, WorldManager* p_Manager, WardenContext* p_Context);
    /// Handle WardenOpcodes::WARDEN_SMSG_CHEAT_CHECKS_REQUEST
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    /// @p_Context  : Warden context
    void HandleWardenCheatChecksRequest(Network::WorldPacket& p_Packet, WorldManager* p_Manager, WardenContext* p_Context);
    /// Handle WardenOpcodes::WARDEN_SMSG_MODULE_INITIALIZE
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    /// @p_Context  : Warden context
    void HandleWardenModuleInitialize(Network::WorldPacket& p_Packet, WorldManager* p_Manager, WardenContext* p_Context);

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Handle SMSG_WARDEN_DATA
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    void Handle_WardenData(Network::WorldPacket& p_Packet, WorldManager* p_Manager)
    {
        /// Private code
    }

}   ///< namespace Handlers
}   ///< namespace World
}   ///< namespace RCN