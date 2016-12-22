#pragma once

#include "BigNumber.hpp"
#include <stdint.h>
#include <string>

namespace RCN { namespace Crypto {

    class ARC4
    {
        public:
            /// Constructor
            /// @p_Key : Panama cypher key
            ARC4(BigNumber p_Key);
            /// Destructor
            ~ARC4();

            /// Update
            /// @p_Data : input data
            /// @p_Size : input data len
            void UpdateData(uint8_t* p_Data, int p_Size);

        private:
            void * m_Context;   ///< ARC4 context

    };

}   ///< namespace Crypto
}   ///< namespace RCN