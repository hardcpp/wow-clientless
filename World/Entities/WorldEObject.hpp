#pragma once

#include "../Enums/WorldEnums.hpp"
#include "WorldEUpdateFields.hpp"

namespace RCN { namespace World { namespace Entities {

    struct Position
    {
        /// Constructor
        Position()
            : X(0.0f), Y(0.0f), Z(0.0f), O(0.0f)
        {

        }

        float X, Y, Z, O;
    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class Object
    {
        public:
            /// Constructor
            Object();
            /// Destructor
            virtual ~Object();

    };

}   ///< namespace Entities
}   ///< namespace World
}   ///< namespace RCN