#include "WardenKeyGenerator.hpp"
#include "SHA1.hpp"
#include <algorithm>

namespace RCN { namespace Crypto {

    /// Constructor
    /// @p_SessionKey : World crypto key
    WardenKeyGenerator::WardenKeyGenerator(BigNumber p_SessionKey)
        : m_SizeTaked(0)
    {
        m_PairA = new uint8_t[SHA_DIGEST_LENGTH];
        m_PairB = new uint8_t[SHA_DIGEST_LENGTH];
        m_PairC = new uint8_t[SHA_DIGEST_LENGTH];

        //////////////////////////////////////////////////////////////////////////

        memset(m_PairA, 0, SHA_DIGEST_LENGTH);

        //////////////////////////////////////////////////////////////////////////

        uint8_t * l_SubSessionKeyA = new uint8_t[SHA_DIGEST_LENGTH];
        memcpy(l_SubSessionKeyA, p_SessionKey.AsByteArray(), 20);

        SHA1 l_Sha1A;
        l_Sha1A.Initialize();
        l_Sha1A.UpdateData(l_SubSessionKeyA, SHA_DIGEST_LENGTH);
        l_Sha1A.Finalize();

        memcpy(m_PairB, l_Sha1A.GetDigest(), SHA_DIGEST_LENGTH);

        //////////////////////////////////////////////////////////////////////////

        uint8_t * l_SubSessionKeyB = new uint8_t[SHA_DIGEST_LENGTH];
        memcpy(l_SubSessionKeyB, p_SessionKey.AsByteArray() + 20, 20);

        SHA1 l_Sha1B;
        l_Sha1B.Initialize();
        l_Sha1B.UpdateData(l_SubSessionKeyB, SHA_DIGEST_LENGTH);
        l_Sha1B.Finalize();

        memcpy(m_PairC, l_Sha1B.GetDigest(), SHA_DIGEST_LENGTH);

        //////////////////////////////////////////////////////////////////////////

        delete[] l_SubSessionKeyA;
        delete[] l_SubSessionKeyB;

        FillUp();
    }
    /// Destructor
    WardenKeyGenerator::~WardenKeyGenerator()
    {
        delete m_PairA;
        delete m_PairB;
        delete m_PairC;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Generate a new key
    BigNumber WardenKeyGenerator::GenerateKey()
    {
        uint8_t * l_NewKey = new uint8_t[16];

        for (size_t l_I = 0; l_I < 16; ++l_I)
        {
            if (m_SizeTaked == SHA_DIGEST_LENGTH)
                FillUp();

            l_NewKey[l_I] = m_PairA[m_SizeTaked];
            m_SizeTaked++;
        }

        BigNumber l_Result;
        l_Result.SetBinary(l_NewKey, 16);

        return l_Result;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Fill up pair A
    void WardenKeyGenerator::FillUp()
    {
        SHA1 l_Sha1;
        l_Sha1.UpdateData(m_PairB, SHA_DIGEST_LENGTH);
        l_Sha1.UpdateData(m_PairA, SHA_DIGEST_LENGTH);
        l_Sha1.UpdateData(m_PairC, SHA_DIGEST_LENGTH);
        l_Sha1.Finalize();

        memcpy(m_PairA, l_Sha1.GetDigest(), SHA_DIGEST_LENGTH);

        m_SizeTaked = 0;
    }


}   ///< namespace Crypto
}   ///< namespace RCN