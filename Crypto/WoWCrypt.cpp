#include "WoWCrypt.hpp"

const static size_t CRYPTED_SEND_LEN = 6;
const static size_t CRYPTED_RECV_LEN = 4;

namespace RCN { namespace Crypto {

    /// Constructor
    WoWCrypt::WoWCrypt()
    {
        m_Initialized = false;
    }
    /// Destructor
    WoWCrypt::~WoWCrypt()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Init WoWCrypt
    void WoWCrypt::Init()
    {
        m_SendA = 0;
        m_SendB = 0;
        m_RecvA = 0;
        m_RecvB = 0;
        m_Initialized = true;
    }
    /// Set encrypt key
    /// @p_Key      : Key 
    /// @p_KeySize  : Size of the key
    void WoWCrypt::SetKey(uint8_t* p_Key, size_t p_KeySize)
    {
        m_Key.resize(p_KeySize);
        std::copy(p_Key, p_Key + p_KeySize, m_Key.begin());
    }
    /// Is the WoWCrypt initialized
    bool WoWCrypt::IsInitialized()
    {
        return m_Initialized;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Decrypt data
    /// @p_Data : Data to decrypt
    /// @p_Size : Size of data
    void WoWCrypt::Decrypt(uint8_t* p_Data, size_t p_Size)
    {
        if (!m_Initialized)
            return;

        if (p_Size < CRYPTED_RECV_LEN)
            return;

        for (size_t l_I = 0; l_I < CRYPTED_RECV_LEN; l_I++)
        {
            m_RecvA %= m_Key.size();

            uint8_t l_Byte = (p_Data[l_I] - m_RecvB) ^ m_Key[m_RecvA];

            ++m_RecvA;

            m_RecvB = p_Data[l_I];
            p_Data[l_I] = l_Byte;
        }
    }
    /// Encrypt data
    /// @p_Data : Data to encrypt
    /// @p_Size : Size of data
    void WoWCrypt::Encrypt(uint8_t* p_Data, size_t p_Size)
    {
        if (!m_Initialized)
            return;

        if (p_Size < CRYPTED_SEND_LEN)
            return;

        for (size_t l_I = 0; l_I < CRYPTED_SEND_LEN; l_I++)
        {
            m_SendA %= m_Key.size();

            uint8_t l_Byte = (p_Data[l_I] ^ m_Key[m_SendA]) + m_SendB;

            ++m_SendA;

            p_Data[l_I] = m_SendB = l_Byte;
        }
    }

}   ///< namespace Crypto
}   ///< namespace RCN