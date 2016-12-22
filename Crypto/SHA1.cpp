#include "SHA1.hpp"

#include <stdarg.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>

namespace RCN { namespace Crypto {

    /// Constructor
    SHA1::SHA1()
    {
        m_Context = new SHA_CTX();
        SHA1_Init(reinterpret_cast<SHA_CTX*>(m_Context));
    }
    /// Destructor
    SHA1::~SHA1()
    {
        SHA1_Init(reinterpret_cast<SHA_CTX*>(m_Context));
        delete m_Context;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Update
    /// @p_BN1 : First big number
    /// ...
    void SHA1::UpdateBigNumbers(BigNumber* p_BN1, ...)
    {
        va_list l_VAList;
        BigNumber* l_BigNumber;

        va_start(l_VAList, p_BN1);
        l_BigNumber = p_BN1;

        while (l_BigNumber)
        {
            UpdateData(l_BigNumber->AsByteArray(), l_BigNumber->GetNumBytes());
            l_BigNumber = va_arg(l_VAList, BigNumber*);
        }

        va_end(l_VAList);
    }
    /// Update
    /// @p_Data : input data
    /// @p_Size : input data len
    void SHA1::UpdateData(const uint8_t* p_Data, int p_Size)
    {
        SHA1_Update(reinterpret_cast<SHA_CTX*>(m_Context), p_Data, p_Size);
    }
    /// Update
    /// @p_Str : input data
    void SHA1::UpdateData(const std::string& p_Str)
    {
        UpdateData((uint8_t const*)p_Str.c_str(), p_Str.length());
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Init
    void SHA1::Initialize()
    {
        SHA1_Init(reinterpret_cast<SHA_CTX*>(m_Context));
    }
    /// Finish
    void SHA1::Finalize()
    {
        SHA1_Final(m_Digest, reinterpret_cast<SHA_CTX*>(m_Context));
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get digest
    uint8_t* SHA1::GetDigest()
    {
        return m_Digest;
    };
    /// Get digest length
    int SHA1::GetLength()
    {
        return SHA_DIGEST_LENGTH;
    };

}   ///< namespace Crypto
}   ///< namespace RCN