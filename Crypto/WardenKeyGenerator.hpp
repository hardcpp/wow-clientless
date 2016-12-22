#pragma once

#include "BigNumber.hpp"
#include <string>

namespace RCN { namespace Crypto {

    class WardenKeyGenerator
    {
        public:
            /// Constructor
            /// @p_SessionKey : World crypto key
            WardenKeyGenerator(BigNumber p_SessionKey);
            /// Destructor
            ~WardenKeyGenerator();

            /// Generate a new key
            BigNumber GenerateKey();

        private:
            /// Fill up pair A
            void FillUp();

        private:
            uint8_t* m_PairA;
            uint8_t* m_PairB;
            uint8_t* m_PairC;

            size_t m_SizeTaked;

    };

}   ///< namespace Crypto
}   ///< namespace RCN