#pragma once

#include <stdint.h>

struct bignum_st;

namespace RCN { namespace Crypto {

    class BigNumber
    {
        public:
            /// Constructor
            BigNumber();
            /// Constructor
            /// @p_BN : Copy
            BigNumber(const BigNumber& p_BN);
            /// Constructor
            /// @p_Value : Copy
            BigNumber(uint32_t p_Value);
            /// Destructor
            ~BigNumber();

            /// Set value
            /// @p_Value : New value
            void SetDword(uint32_t p_Value);
            /// Set value
            /// @p_Value : New value
            void SetQword(uint64_t p_Value);
            /// Set value
            /// @p_Value : New value
            /// @p_Size  : New value size
            void SetBinary(const uint8_t* p_Value, size_t p_Size);
            /// Set value
            /// @p_Value : New value
            void SetHexStr(const char* p_Value);

            /// Set rand value
            /// @p_NumBits : Num bits
            void SetRand(int p_NumBits);

            /// Operator
            /// @p_Other : Operand right value
            BigNumber operator=(const BigNumber& p_Other);
            /// Operator
            /// @p_Other : Operand right value
            BigNumber operator+=(const BigNumber& p_Other);
            /// Operator
            /// @p_Other : Operand right value
            BigNumber operator+(const BigNumber& p_Other);
            /// Operator
            /// @p_Other : Operand right value
            BigNumber operator-=(const BigNumber& p_Other);
            /// Operator
            /// @p_Other : Operand right value
            BigNumber operator-(const BigNumber& p_Other);
            /// Operator
            /// @p_Other : Operand right value
            BigNumber operator*=(const BigNumber& p_Other);
            /// Operator
            /// @p_Other : Operand right value
            BigNumber operator*(const BigNumber& p_Other);
            /// Operator
            /// @p_Other : Operand right value
            BigNumber operator/=(const BigNumber& p_Other);
            /// Operator
            /// @p_Other : Operand right value
            BigNumber operator/(const BigNumber& p_Other);
            /// Operator
            /// @p_Other : Operand right value
            BigNumber operator%=(const BigNumber& p_Other);
            /// Operator
            /// @p_Other : Operand right value
            BigNumber operator%(const BigNumber& p_Other);

            /// Is zero
            bool IsZero() const;

            /// Mod exp
            /// @p_OtherA : A
            /// @p_OtherB : B
            BigNumber ModExp(const BigNumber& p_OtherA, const BigNumber& p_OtherB);
            /// Exp
            /// @p_Other : Other
            BigNumber Exp(const BigNumber& p_Other);

            /// Get num bytes
            int GetNumBytes();

            /// Get as uint32
            uint32_t AsDword();

            /// As byte array
            /// @p_MinSize : Min size
            uint8_t* AsByteArray(int p_MinSize = 0);
            /// As byte array
            /// @p_MinSize : Min size
            /// @p_Reverse : Reverse array
            uint8_t* AsByteArray(int p_MinSize, bool p_Reverse);
            /// As hex str
            const char* AsHexStr();
            /// As dec str
            const char* AsDecStr();

        private:
            struct bignum_st*   m_BN;       ///< The number
            uint8_t*            m_Array;    ///< Output temp

    };

}   ///< namespace Crypto
}   ///< namespace RCN