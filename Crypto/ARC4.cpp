#include "ARC4.hpp"
#include <openssl/evp.h>

namespace RCN { namespace Crypto {

    /// Constructor
    /// @p_Key : Panama cypher key
    ARC4::ARC4(BigNumber p_Key)
    {
        m_Context = new EVP_CIPHER_CTX();

        EVP_CIPHER_CTX_init(reinterpret_cast<EVP_CIPHER_CTX*>(m_Context));
        EVP_EncryptInit_ex(reinterpret_cast<EVP_CIPHER_CTX*>(m_Context), EVP_rc4(), NULL, NULL, NULL);
        EVP_CIPHER_CTX_set_key_length(reinterpret_cast<EVP_CIPHER_CTX*>(m_Context), p_Key.GetNumBytes());
        EVP_EncryptInit_ex(reinterpret_cast<EVP_CIPHER_CTX*>(m_Context), NULL, NULL, p_Key.AsByteArray(), NULL);
    }
    /// Destructor
    ARC4::~ARC4()
    {
        EVP_CIPHER_CTX_cleanup(reinterpret_cast<EVP_CIPHER_CTX*>(m_Context));

        delete m_Context;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Update
    /// @p_Data : input data
    /// @p_Size : input data len
    void ARC4::UpdateData(uint8_t* p_Data, int p_Size)
    {
        int l_OutLen = 0;
        EVP_EncryptUpdate(reinterpret_cast<EVP_CIPHER_CTX*>(m_Context), p_Data, &l_OutLen, p_Data, p_Size);
        EVP_EncryptFinal_ex(reinterpret_cast<EVP_CIPHER_CTX*>(m_Context), p_Data, &l_OutLen);
    }

}   ///< namespace Crypto
}   ///< namespace RCN