#include "BigNumber.hpp"
#include <openssl/bn.h>
#include <algorithm>

namespace RCN { namespace Crypto {

    /// Constructor
    BigNumber::BigNumber()
    {
        m_BN = BN_new();
        m_Array = NULL;
    }
    /// Constructor
    /// @p_BN : Copy
    BigNumber::BigNumber(const BigNumber& p_BN)
    {
        m_BN = BN_dup(p_BN.m_BN);
        m_Array = NULL;
    }
    /// Constructor
    /// @p_Value : Copy
    BigNumber::BigNumber(uint32_t p_Value)
    {
        m_BN = BN_new();
        BN_set_word(m_BN, p_Value);
        m_Array = NULL;
    }
    /// Destructor
    BigNumber::~BigNumber()
    {
        BN_free(m_BN);

        if (m_Array)
            delete[] m_Array;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set value
    /// @p_Value : New value
    void BigNumber::SetDword(uint32_t p_Value)
    {
        BN_set_word(m_BN, p_Value);
    }
    /// Set value
    /// @p_Value : New value
    void BigNumber::SetQword(uint64_t p_Value)
    {
        BN_add_word(m_BN, (uint32_t)(p_Value >> 32));
        BN_lshift(m_BN, m_BN, 32);
        BN_add_word(m_BN, (uint32_t)(p_Value & 0xFFFFFFFF));
    }
    /// Set value
    /// @p_Value : New value
    /// @p_Size  : New value size
    void BigNumber::SetBinary(const uint8_t* p_Value, size_t p_Size)
    {
        uint8_t l_Buffer[1000];
        for (int l_I = 0; l_I < p_Size; ++l_I)
            l_Buffer[l_I] = p_Value[p_Size - 1 - l_I];

        BN_bin2bn(l_Buffer, p_Size, m_BN);
    }
    /// Set value
    /// @p_Value : New value
    void BigNumber::SetHexStr(const char* p_Value)
    {
        BN_hex2bn(&m_BN, p_Value);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set rand value
    /// @p_NumBits : Num bits
    void BigNumber::SetRand(int p_NumBits)
    {
        BN_rand(m_BN, p_NumBits, 0, 1);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Operator
    /// @p_Other : Operand right value
    BigNumber BigNumber::operator=(const BigNumber& p_Other)
    {
        BN_copy(m_BN, p_Other.m_BN);
        return *this;
    }
    /// Operator
    /// @p_Other : Operand right value
    BigNumber BigNumber::operator+=(const BigNumber& p_Other)
    {
        BN_add(m_BN, m_BN, p_Other.m_BN);
        return *this;
    }
    /// Operator
    /// @p_Other : Operand right value
    BigNumber BigNumber::operator+(const BigNumber& p_Other)
    {
        BigNumber l_Temp(*this);
        return l_Temp += p_Other;
    }
    /// Operator
    /// @p_Other : Operand right value
    BigNumber BigNumber::operator-=(const BigNumber& p_Other)
    {
        BN_sub(m_BN, m_BN, p_Other.m_BN);
        return *this;
    }
    /// Operator
    /// @p_Other : Operand right value
    BigNumber BigNumber::operator-(const BigNumber& p_Other)
    {
        BigNumber l_Temp(*this);
        return l_Temp -= p_Other;
    }
    /// Operator
    /// @p_Other : Operand right value
    BigNumber BigNumber::operator*=(const BigNumber& p_Other)
    {
        BN_CTX* l_Context;

        l_Context = BN_CTX_new();
        BN_mul(m_BN, m_BN, p_Other.m_BN, l_Context);
        BN_CTX_free(l_Context);

        return *this;
    }
    /// Operator
    /// @p_Other : Operand right value
    BigNumber BigNumber::operator*(const BigNumber& p_Other)
    {
        BigNumber l_Temp(*this);
        return l_Temp *= p_Other;
    }
    /// Operator
    /// @p_Other : Operand right value
    BigNumber BigNumber::operator/=(const BigNumber& p_Other)
    {
        BN_CTX* l_Context;

        l_Context = BN_CTX_new();
        BN_div(m_BN, NULL, m_BN, p_Other.m_BN, l_Context);
        BN_CTX_free(l_Context);

        return *this;
    }
    /// Operator
    /// @p_Other : Operand right value
    BigNumber BigNumber::operator/(const BigNumber& p_Other)
    {
        BigNumber l_Temp(*this);
        return l_Temp /= p_Other;
    }
    /// Operator
    /// @p_Other : Operand right value
    BigNumber BigNumber::operator%=(const BigNumber& p_Other)
    {
        BN_CTX* l_Context;

        l_Context = BN_CTX_new();
        BN_mod(m_BN, m_BN, p_Other.m_BN, l_Context);
        BN_CTX_free(l_Context);

        return *this;
    }
    /// Operator
    /// @p_Other : Operand right value
    BigNumber BigNumber::operator%(const BigNumber& p_Other)
    {
        BigNumber l_Temp(*this);
        return l_Temp %= p_Other;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Is zero
    bool BigNumber::IsZero() const
    {
        return BN_is_zero(m_BN) != 0;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Mod exp
    /// @p_OtherA : A
    /// @p_OtherB : B
    BigNumber BigNumber::ModExp(const BigNumber& p_OtherA, const BigNumber& p_OtherB)
    {
        BigNumber l_Result;
        BN_CTX* l_Context;

        l_Context = BN_CTX_new();
        BN_mod_exp(l_Result.m_BN, m_BN, p_OtherA.m_BN, p_OtherB.m_BN, l_Context);
        BN_CTX_free(l_Context);

        return l_Result;
    }
    /// Exp
    /// @p_Other : Other
    BigNumber BigNumber::Exp(const BigNumber& p_Other)
    {
        BigNumber l_Result;
        BN_CTX* l_Context;

        l_Context = BN_CTX_new();
        BN_exp(l_Result.m_BN, m_BN, p_Other.m_BN, l_Context);
        BN_CTX_free(l_Context);

        return l_Result;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get num bytes
    int BigNumber::GetNumBytes()
    {
        return BN_num_bytes(m_BN);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get as uint32
    uint32_t BigNumber::AsDword()
    {
        return (uint32_t)BN_get_word(m_BN);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// As byte array
    /// @p_MinSize : Min size
    uint8_t* BigNumber::AsByteArray(int p_MinSize)
    {
        int l_Size = (p_MinSize >= GetNumBytes()) ? p_MinSize : GetNumBytes();

        delete[] m_Array;
        m_Array = new uint8_t[l_Size];

        /// If we need more bytes than length of BigNumber set the rest to 0
        if (l_Size > GetNumBytes())
            memset((void*)m_Array, 0, l_Size);

        BN_bn2bin(m_BN, (unsigned char*)m_Array);

        std::reverse(m_Array, m_Array + l_Size);

        return m_Array;
    }
    /// As byte array
    /// @p_MinSize : Min size
    /// @p_Reverse : Reverse array
    uint8_t* BigNumber::AsByteArray(int p_MinSize, bool p_Reverse)
    {
        int l_Size = (p_MinSize >= GetNumBytes()) ? p_MinSize : GetNumBytes();

        if (m_Array)
        {
            delete[] m_Array;
            m_Array = NULL;
        }
        m_Array = new uint8_t[l_Size];

        /// If we need more bytes than length of BigNumber set the rest to 0
        if (l_Size > GetNumBytes())
            memset((void*)m_Array, 0, l_Size);

        BN_bn2bin(m_BN, (unsigned char *)m_Array);

        if (p_Reverse)
            std::reverse(m_Array, m_Array + l_Size);

        return m_Array;
    }
    /// As hex str
    const char* BigNumber::AsHexStr()
    {
        return BN_bn2hex(m_BN);
    }
    /// As dec str
    const char* BigNumber::AsDecStr()
    {
        return BN_bn2dec(m_BN);
    }

}   ///< namespace Crypto
}   ///< namespace RCN