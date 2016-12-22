#pragma once

#include "../../Network/ByteBuffer.hpp"
#include "WorldWardenStructs.hpp"

namespace RCN { namespace World { namespace Warden {

    /// Get module mapped size
    /// @p_Input : Input decompressed warden module
    size_t GetModuleMapedSize(RCN::Network::ByteBuffer& p_Input);
    /// Remap warden module
    /// @p_Input : Input decompressed warden module
    uint32_t RemapModule(RCN::Network::ByteBuffer& p_Input);

    void ScrambleKey();

}   ///< namespace Warden
}   ///< namespace World
}   ///< namespace RCN
