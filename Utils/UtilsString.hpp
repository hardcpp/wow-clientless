#pragma once

#include <string>
#include <vector>

namespace RCN { namespace Utils {

    /// Split string
    /// @p_Src       ; Source
    /// @p_Delimeter : Token
    /// @p_KeepEmpty : Keep empty parts
    std::vector<std::string> Split(const std::string& p_Str, const std::string& p_Delimiter, const bool p_KeepEmpty = true);

}   ///< namespace Utils
}   ///< namespace RCN