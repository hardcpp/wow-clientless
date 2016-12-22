#pragma once

#include "BigNumber.hpp"
#include <stdint.h>
#include <string>

#define SHA_DIGEST_LENGTH 20

namespace RCN { namespace Crypto {

    class SHA1
    {
        public:
            /// Constructor
            SHA1();
            /// Destructor
            ~SHA1();

            /// Update
            /// @p_BN1 : First big number
            /// ...
            void UpdateBigNumbers(BigNumber* p_BN1, ...);
            /// Update
            /// @p_Data : input data
            /// @p_Size : input data len
            void UpdateData(const uint8_t* p_Data, int p_Size);
            /// Update
            /// @p_Str : input data
            void UpdateData(const std::string& p_Str);

            /// Init
            void Initialize();
            /// Finish
            void Finalize();

            /// Get digest
            uint8_t* GetDigest();
            /// Get digest length
            int GetLength();

        private:
            void          *   m_Context;                        ///< SHA1 context
            uint8_t           m_Digest[SHA_DIGEST_LENGTH];      ///< Output digest

    };

}   ///< namespace Crypto
}   ///< namespace RCN