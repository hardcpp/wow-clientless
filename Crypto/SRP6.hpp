#pragma once

#include "BigNumber.hpp"
#include <string>

namespace RCN { namespace Crypto {

    class SRP6
    {
        public:
            /// Constructor
            /// p_N : N number
            /// p_G : G number
            SRP6(BigNumber p_N, BigNumber p_G);

            /// Get A number
            /// @p_a : a input
            BigNumber GetA(BigNumber p_a);
            /// Get x number
            /// @p_Salt         : Salt
            /// @p_LogonHash    : Logon hash
            BigNumber Getx(BigNumber p_Salt, BigNumber p_LogonHash);
            /// Get u number
            /// @p_A : A number
            /// @p_B : B number
            BigNumber Getu(BigNumber p_A, BigNumber p_B);

            /// Get client session key
            /// @p_a : a input
            /// @p_B : B number
            /// @p_x : x number
            /// @p_u : u number
            BigNumber GetClientS(BigNumber p_a, BigNumber p_B, BigNumber p_x, BigNumber p_u);

            /// Get logon hash
            /// @p_Username : Account name
            /// @p_Password : Account password
            BigNumber GetLogonHash(std::string p_Username, std::string p_Password);

            /// @p_Username : Account name
            /// @p_s : s number
            /// @p_A : A number
            /// @p_B : B number
            /// @p_K : K number
            BigNumber GetM(std::string p_Username, BigNumber p_s, BigNumber p_A, BigNumber p_B, BigNumber p_K);

            /// SHAInterleave
            /// @p_S : S number
            BigNumber SHAInterleave(BigNumber p_S);

        private:
            BigNumber m_N;  ///< N number
            BigNumber m_G;  ///< G number
            BigNumber m_K;  ///< K number

    };

}   ///< namespace Crypto
}   ///< namespace RCN