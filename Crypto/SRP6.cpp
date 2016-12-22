#include "SRP6.hpp"
#include "SHA1.hpp"
#include <algorithm>

namespace RCN { namespace Crypto {

    /// Constructor
    /// p_N : N number
    /// p_G : G number
    SRP6::SRP6(BigNumber p_N, BigNumber p_G)
        : m_N(p_N), m_G(p_G), m_K(3)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get A number
    /// @p_A : a input
    BigNumber SRP6::GetA(BigNumber p_a)
    {
        return m_G.ModExp(p_a, m_N);
    }
    /// Get x number
    /// @p_Salt         : Salt
    /// @p_LogonHash    : Logon hash
    BigNumber SRP6::Getx(BigNumber p_Salt, BigNumber p_LogonHash)
    {
        SHA1 l_SHA1;
        l_SHA1.Initialize();
        l_SHA1.UpdateBigNumbers(&p_Salt, &p_LogonHash, nullptr);
        l_SHA1.Finalize();

        BigNumber l_Result;
        l_Result.SetBinary(l_SHA1.GetDigest(), SHA_DIGEST_LENGTH);

        return l_Result;
    }
    /// Get u number
    /// @p_A : A number
    /// @p_B : B number
    BigNumber SRP6::Getu(BigNumber p_A, BigNumber p_B)
    {
        SHA1 l_SHA1;
        l_SHA1.Initialize();
        l_SHA1.UpdateBigNumbers(&p_A, &p_B, nullptr);
        l_SHA1.Finalize();

        BigNumber l_Result;
        l_Result.SetBinary(l_SHA1.GetDigest(), SHA_DIGEST_LENGTH);

        return l_Result;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get client session key
    /// @p_a : a input
    /// @p_B : B number
    /// @p_x : x number
    /// @p_u : u number
    BigNumber SRP6::GetClientS(BigNumber p_a, BigNumber p_B, BigNumber p_x, BigNumber p_u)
    {
        BigNumber l_S;
        l_S = (p_B - (m_K * m_G.ModExp(p_x, m_N))).ModExp(p_a + (p_u * p_x), m_N);

        return l_S;
    }

    //////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////<///////////

    /// Get logon hash
    /// @p_Username : Account name
    /// @p_Password : Account password
    BigNumber SRP6::GetLogonHash(std::string p_Username, std::string p_Password)
    {
        std::transform(p_Username.begin(), p_Username.end(), p_Username.begin(), ::toupper);
        std::transform(p_Password.begin(), p_Password.end(), p_Password.begin(), ::toupper);
        std::string l_Mid = p_Username + ":" + p_Password;

        SHA1 l_SHA1;
        l_SHA1.Initialize();
        l_SHA1.UpdateData(l_Mid);
        l_SHA1.Finalize();

        BigNumber l_Result;
        l_Result.SetBinary(l_SHA1.GetDigest(), SHA_DIGEST_LENGTH);

        return l_Result;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// SHAInterleave
    /// @p_S : S number
    BigNumber SRP6::SHAInterleave(BigNumber p_S)
    {
        uint8_t* l_T = p_S.AsByteArray();
        int l_HalfSize = p_S.GetNumBytes() / 2;
        uint8_t* l_T1 = new uint8_t[l_HalfSize];

        for (int l_I = 0; l_I < l_HalfSize; l_I++)
            l_T1[l_I] = l_T[l_I * 2];

        SHA1 l_SHA1;
        l_SHA1.Initialize();
        l_SHA1.UpdateData(l_T1, l_HalfSize);
        l_SHA1.Finalize();

        uint8_t* l_T1Hash = new uint8_t[SHA_DIGEST_LENGTH];
        memcpy(l_T1Hash, l_SHA1.GetDigest(), SHA_DIGEST_LENGTH);

        uint8_t* l_vK = new uint8_t[40];
        for (size_t l_I = 0; l_I < 20; l_I++)
            l_vK[l_I * 2] = l_T1Hash[l_I];

        for (size_t l_I = 0; l_I < l_HalfSize; l_I++)
            l_T1[l_I] = l_T[l_I * 2 + 1];

        l_SHA1.Initialize();
        l_SHA1.UpdateData(l_T1, l_HalfSize);
        l_SHA1.Finalize();
        memcpy(l_T1Hash, l_SHA1.GetDigest(), SHA_DIGEST_LENGTH);

        for (size_t l_I = 0; l_I < 20; l_I++)
            l_vK[l_I * 2 + 1] = l_T1Hash[l_I];

        BigNumber l_Result;
        l_Result.SetBinary(l_vK, 40);

        delete[] l_T1;
        delete[] l_T1Hash;
        delete[] l_vK;

        return l_Result;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// @p_Username : Account name
    /// @p_s : s number
    /// @p_A : A number
    /// @p_B : B number
    /// @p_K : K number
    BigNumber SRP6::GetM(std::string p_Username, BigNumber p_s, BigNumber p_A, BigNumber p_B, BigNumber p_K)
    {
        SHA1 l_SHA1;
        l_SHA1.Initialize();
        l_SHA1.UpdateBigNumbers(&m_N, nullptr);
        l_SHA1.Finalize();

        uint8_t* l_Hash = new uint8_t[SHA_DIGEST_LENGTH];
        memcpy(l_Hash, l_SHA1.GetDigest(), SHA_DIGEST_LENGTH);

        l_SHA1.Initialize();
        l_SHA1.UpdateBigNumbers(&m_G, nullptr);
        l_SHA1.Finalize();

        uint8_t* l_GHash = new uint8_t[SHA_DIGEST_LENGTH];
        memcpy(l_GHash, l_SHA1.GetDigest(), SHA_DIGEST_LENGTH);

        for (size_t l_I = 0; l_I < SHA_DIGEST_LENGTH; ++l_I)
            l_Hash[l_I] ^= l_GHash[l_I];

        BigNumber l_T3;
        l_T3.SetBinary(l_Hash, SHA_DIGEST_LENGTH);

        //////////////////////////////////////////////////////////////////////////

        l_SHA1.Initialize();
        l_SHA1.UpdateData(p_Username);
        l_SHA1.Finalize();

        BigNumber l_T4;
        l_T4.SetBinary(l_SHA1.GetDigest(), SHA_DIGEST_LENGTH);

        //////////////////////////////////////////////////////////////////////////

        l_SHA1.Initialize();
        l_SHA1.UpdateBigNumbers(&l_T3, &l_T4, &p_s, &p_A, &p_B, &p_K, nullptr);
        l_SHA1.Finalize();

        BigNumber l_Result;
        l_Result.SetBinary(l_SHA1.GetDigest(), SHA_DIGEST_LENGTH);

        delete[] l_Hash;
        delete[] l_GHash;

        return l_Result;
    }

}   ///< namespace Crypto
}   ///< namespace RCN