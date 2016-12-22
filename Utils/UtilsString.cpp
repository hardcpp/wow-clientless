#include "UtilsString.hpp"
#include <algorithm>

namespace RCN { namespace Utils {

    /// Split string
    /// @p_Src       ; Source
    /// @p_Delimeter : Token
    /// @p_KeepEmpty : Keep empty parts
    std::vector<std::string> Split(const std::string& p_Str, const std::string& p_Delimiter, const bool p_KeepEmpty)
    {
        std::vector<std::string> l_Result;
        if (p_Delimiter.empty())
        {
            l_Result.push_back(p_Str);
            return l_Result;
        }

        std::string::const_iterator l_SubStart = p_Str.begin(), l_SubEnd;
        while (true)
        {
            l_SubEnd = std::search(l_SubStart, p_Str.end(), p_Delimiter.begin(), p_Delimiter.end());

            std::string l_Temp(l_SubStart, l_SubEnd);

            if (p_KeepEmpty || !l_Temp.empty())
                l_Result.push_back(l_Temp);

            if (l_SubEnd == p_Str.end())
                break;

            l_SubStart = l_SubEnd + p_Delimiter.size();
        }

        return l_Result;
    }

}   ///< namespace Utils
}   ///< namespace RCN