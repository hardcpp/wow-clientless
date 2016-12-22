#pragma once

#include <stdint.h>
#include <vector>

namespace RCN { namespace Crypto {

    class WoWCrypt
    {
        public:
            /// Constructor
            WoWCrypt();
            /// Destructor
            ~WoWCrypt();

            /// Init WoWCrypt
            void Init();
            /// Set encrypt key
            /// @p_Key      : Key 
            /// @p_KeySize  : Size of the key
            void SetKey(uint8_t* p_Key, size_t p_KeySize);
            /// Is the WoWCrypt initialized
            bool IsInitialized();

            /// Decrypt data
            /// @p_Data : Data to decrypt
            /// @p_Size : Size of data
            void Decrypt(uint8_t* p_Data, size_t p_Size);
            /// Encrypt data
            /// @p_Data : Data to encrypt
            /// @p_Size : Size of data
            void Encrypt(uint8_t* p_Data, size_t p_Size);

        private:
            std::vector<uint8_t>    m_Key;          ///< Encrypt/Decrypt key
            uint8_t                 m_SendA;        ///< Encrypt state A
            uint8_t                 m_SendB;        ///< Encrypt state B
            uint8_t                 m_RecvA;        ///< Decrypt state A
            uint8_t                 m_RecvB;        ///< Decrypt state B
            bool                    m_Initialized;  ///< Is the system initialized

    };

}   ///< namespace Crypto
}   ///< namespace RCN