#pragma once

#include <stdint.h>

namespace RCN { namespace World { namespace Warden {

    struct ModuleHeader 
    {
        uint32_t MapedSize;
        uint32_t Unknown1;
        uint32_t RefTable;
        uint32_t RefCount;
        uint32_t InitAddr;
        uint32_t Unknown3;
        uint32_t Unknown4;
        uint32_t LibTable;
        uint32_t LibCount;
        uint32_t Unknown5;
    };

    struct LibraryReferance 
    {
        uint32_t NameAddress;
        uint32_t FunctionTable;
    };

}   ///< namespace Warden
}   ///< namespace World
}   ///< namespace RCN
