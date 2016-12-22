#pragma once

#include <algorithm>

namespace RCN { namespace Utils {

    namespace ByteConverter
    {
        /// Convert data
        /// @p_Data : Data to convert
        template<size_t T> inline void Convert(char* p_Data)
        {
            std::swap(*p_Data, *(p_Data + T - 1));
            Convert<T-2>(p_Data + 1);
        }

        /// Ignore null size data
        /// @p_Data : Data to convert
        template<> inline void Convert<0>(char* p_Data)
        {
        }
        /// Ignore central byte
        /// @p_Data : Data to convert
        template<> inline void Convert<1>(char* p_Data)
        {
        }

        /// Endian convert
        /// @p_Data : Data to convert
        template<typename T> inline void Apply(T* p_Data)
        {
            Convert<sizeof(T)>((char*)(p_Data));
        }
    }

}   ///< namespace Utils
}   ///< namespace RCN