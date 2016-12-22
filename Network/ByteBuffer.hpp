#pragma once

#include <stdint.h>
#include <vector>
#include <string>

namespace RCN { namespace Network {

    /// Byte buffer class
    class ByteBuffer
    {
        public:
            const static size_t DEFAULT_SIZE = 0x1000;

            /// Constructor
            ByteBuffer();
            /// Constructor
            ByteBuffer(size_t res);
            /// Constructor
            ByteBuffer(const ByteBuffer &buf);

            /// Clear
            void Clear();

            /// Append
            template <typename T> void Append(T pValue);
            /// Put
            template <typename T> void Put(size_t pPos, T pValue);

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            /// Write
            ByteBuffer &operator<<(uint8_t pValue);
            /// Write
            ByteBuffer &operator<<(uint16_t pValue);
            /// Write
            ByteBuffer &operator<<(uint32_t pValue);
            /// Write
            ByteBuffer &operator<<(uint64_t pValue);

            /// Write
            ByteBuffer &operator<<(int8_t pValue);
            /// Write
            ByteBuffer &operator<<(int16_t pValue);
            /// Write
            ByteBuffer &operator<<(int32_t pValue);
            /// Write
            ByteBuffer &operator<<(int64_t pValue);

            /// Write
            ByteBuffer &operator<<(float pValue);
            /// Write
            ByteBuffer &operator<<(double pValue);

            /// Write
            ByteBuffer &operator<<(const std::string &pValue);
            /// Write
            ByteBuffer &operator<<(const char *pStr);

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            /// Read
            ByteBuffer &operator>>(bool &pValue);

            /// Read
            ByteBuffer &operator>>(uint8_t &pValue);
            /// Read
            ByteBuffer &operator>>(uint16_t &pValue);
            /// Read
            ByteBuffer &operator>>(uint32_t &pValue);
            /// Read
            ByteBuffer &operator>>(uint64_t &pValue);

            /// Read
            ByteBuffer &operator>>(int8_t &pValue);
            /// Read
            ByteBuffer &operator>>(int16_t &pValue);
            /// Read
            ByteBuffer &operator>>(int32_t &pValue);
            /// Read
            ByteBuffer &operator>>(int64_t &pValue);

            /// Read
            ByteBuffer &operator>>(float &pValue);

            /// Read
            ByteBuffer &operator>>(std::string& pValue);
            /// Read
            uint8_t operator[](size_t pos) const;

            //////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////

            /// Get read pos
            size_t ReadPosition() const;
            /// Set read pos
            size_t ReadPosition(size_t pValue);

            /// Read finis ?
            void ReadFinish();

            /// Get write pos
            size_t WritePos() const;
            /// Set write pos
            size_t WritePos(size_t pValue);

            /// Skip read of an element
            template<typename T> void ReadSkip();

            /// Skip read of an element
            void ReadSkip(size_t pSkip);

            /// Read
            template <typename T> T Read();
            /// Read
            template <typename T> T Read(size_t pPos) const;

            /// Read
            void Read(uint8_t *pDest, size_t pLen);

            /// Read a fixed length ASCII string
            /// @p_Size : String size
            std::string ReadFixedString(std::size_t p_Size);
            /// Write a fixed length ASCII string
            /// @p_Str  : The string
            /// @p_Size : Full size
            void WriteFixedString(std::string p_Str, std::size_t p_Size);

            /// Get contents
            const uint8_t * GetData() const;

            /// Get Size
            size_t GetSize() const;
            /// Get is empty
            bool IsEmpty() const;

            /// Resize
            void Resise(size_t pNewSize);
            /// Reserve
            void Reserve(size_t pRessize);

            /// Append string
            void Append(const std::string& pStr);
            /// Append string
            void Append(const char *pSrc, size_t pCnt);

            /// Append
            template<class T> void Append(const T *pSrc, size_t pCnt);

            /// Append
            void Append(const uint8_t *pSrc, size_t pCnt);
            /// Append
            void Append(const ByteBuffer& pBuffer);

            /// Put
            void Put(size_t pPos, const uint8_t *pSrc, size_t pCount);

            /// Read a pack GUID
            uint64_t ReadPackGUID();

        protected:
            size_t m_ReadPos;    /// Read position
            size_t m_WritePos;   /// Write position

            std::vector<uint8_t> m_Storage;    /// Data

    };

    #include "ByteBuffer.inl"

}   ///< namespace Network
}   ///< namespace RCN