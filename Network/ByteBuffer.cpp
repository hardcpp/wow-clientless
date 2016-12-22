#include "ByteBuffer.hpp"

namespace RCN { namespace Network {

    /// Constructor
    ByteBuffer::ByteBuffer()
        : m_ReadPos(0), m_WritePos(0)
    {
        m_Storage.reserve(DEFAULT_SIZE);
    }
    /// Constructor
    ByteBuffer::ByteBuffer(size_t pRes)
        : m_ReadPos(0), m_WritePos(0)
    {
        m_Storage.reserve(pRes);
    }
    /// Constructor
    ByteBuffer::ByteBuffer(const ByteBuffer &pBuf)
        : m_ReadPos(pBuf.m_ReadPos), m_WritePos(pBuf.m_WritePos), m_Storage(pBuf.m_Storage)
    { 

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Clear
    void ByteBuffer::Clear()
    {
        m_Storage.clear();
        m_ReadPos = m_WritePos = 0;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Write
    ByteBuffer &ByteBuffer::operator<<(uint8_t pValue)
    {
        Append<uint8_t>(pValue);
        return *this;
    }
    /// Write
    ByteBuffer &ByteBuffer::operator<<(uint16_t pValue)
    {
        Append<uint16_t>(pValue);
        return *this;
    }
    /// Write
    ByteBuffer &ByteBuffer::operator<<(uint32_t pValue)
    {
        Append<uint32_t>(pValue);
        return *this;
    }
    /// Write
    ByteBuffer &ByteBuffer::operator<<(uint64_t pValue)
    {
        Append<uint64_t>(pValue);
        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Write
    ByteBuffer &ByteBuffer::operator<<(int8_t pValue)
    {
        Append<int8_t>(pValue);
        return *this;
    }
    /// Write
    ByteBuffer &ByteBuffer::operator<<(int16_t pValue)
    {
        Append<int16_t>(pValue);
        return *this;
    }
    /// Write
    ByteBuffer &ByteBuffer::operator<<(int32_t pValue)
    {
        Append<int32_t>(pValue);
        return *this;
    }
    /// Write
    ByteBuffer &ByteBuffer::operator<<(int64_t pValue)
    {
        Append<int64_t>(pValue);
        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Write
    ByteBuffer &ByteBuffer::operator<<(float pValue)
    {
        Append<float>(pValue);
        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Write
    ByteBuffer &ByteBuffer::operator<<(const std::string &pValue)
    {
        Append((uint8_t const *)pValue.c_str(), pValue.length());
        Append((uint8_t)0);
        return *this;
    }
    /// Write
    ByteBuffer &ByteBuffer::operator<<(const char *pStr)
    {
        Append((uint8_t const *)pStr, pStr ? strlen(pStr) : 0);
        Append((uint8_t)0);
        return *this;
    }
  
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Read
    ByteBuffer &ByteBuffer::operator>>(bool &pValue)
    {
        pValue = Read<char>() > 0 ? true : false;
        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Read
    ByteBuffer &ByteBuffer::operator>>(uint8_t &pValue)
    {
        pValue = Read<uint8_t>();
        return *this;
    }
    /// Read
    ByteBuffer &ByteBuffer::operator>>(uint16_t &pValue)
    {
        pValue = Read<uint16_t>();
        return *this;
    }
    /// Read
    ByteBuffer &ByteBuffer::operator>>(uint32_t &pValue)
    {
        pValue = Read<uint32_t>();
        return *this;
    }
    /// Read
    ByteBuffer &ByteBuffer::operator>>(uint64_t &pValue)
    {
        pValue = Read<uint64_t>();
        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Read
    ByteBuffer &ByteBuffer::operator>>(int8_t &pValue)
    {
        pValue = Read<int8_t>();
        return *this;
    }
    /// Read
    ByteBuffer &ByteBuffer::operator>>(int16_t &pValue)
    {
        pValue = Read<int16_t>();
        return *this;
    }
    /// Read
    ByteBuffer &ByteBuffer::operator>>(int32_t &pValue)
    {
        pValue = Read<int32_t>();
        return *this;
    }
    /// Read
    ByteBuffer &ByteBuffer::operator>>(int64_t &pValue)
    {
        pValue = Read<int64_t>();
        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Read
    ByteBuffer &ByteBuffer::operator>>(float &pValue)
    {
        pValue = Read<float>();
        return *this;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Read
    ByteBuffer &ByteBuffer::operator>>(std::string& pValue)
    {
        pValue.clear();
        while (ReadPosition() < GetSize())
        {
            char lC = Read<char>();
            if (lC == 0)
                break;
            pValue += lC;
        }
        return *this;
    }
    /// Read
    uint8_t ByteBuffer::operator[](size_t pos) const
    {
        return Read<uint8_t>(pos);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get read pos
    size_t ByteBuffer::ReadPosition() const 
    { 
        return m_ReadPos; 
    }
    /// Set read pos
    size_t ByteBuffer::ReadPosition(size_t pValue)
    {
        m_ReadPos = pValue;
        return m_ReadPos;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Read finis ?
    void ByteBuffer::ReadFinish()
    {
        m_ReadPos = WritePos();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get write pos
    size_t ByteBuffer::WritePos() const 
    { 
        return m_WritePos; 
    }
    /// Set write pos
    size_t ByteBuffer::WritePos(size_t pValue)
    {
        m_WritePos = pValue;
        return m_WritePos;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Skip read of an element
    void ByteBuffer::ReadSkip(size_t pSkip)
    {
        //if (m_ReadPos + pSkip > GetSize())
        //    throw Core::Exception::Core(__FILE__, __LINE__, "Read skip overflow");

        m_ReadPos += pSkip;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Read
    void ByteBuffer::Read(uint8_t *pDest, size_t pLen)
    {
        //if (m_ReadPos  + pLen > GetSize())
        //    throw Core::Exception::Core(__FILE__, __LINE__, "Read overflow");

        memcpy(pDest, &m_Storage[m_ReadPos], pLen);
        m_ReadPos += pLen;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Read a fixed length ASCII string
    /// @p_Size : String size
    std::string ByteBuffer::ReadFixedString(std::size_t p_Size)
    {
        //if (m_ReadPos + p_Size > GetSize())
        //    throw Core::Exception::Core(__FILE__, __LINE__, "Read overflow");

        std::string l_String(&m_Storage[m_ReadPos], &m_Storage[m_ReadPos + p_Size]);
        m_ReadPos += p_Size;

        return l_String;
    }
    /// Write a fixed length ASCII string
    /// @p_Str  : The string
    /// @p_Size : Full size
    void ByteBuffer::WriteFixedString(std::string p_Str, std::size_t p_Size)
    {
        std::size_t l_StringLen = p_Str.length() > p_Size ? p_Size : p_Str.length();
        Append((char*)&p_Str[0], l_StringLen);

        std::size_t l_PaddingSize = p_Size - l_StringLen;

        for (std::size_t l_I = 0; l_I < l_PaddingSize; ++l_I)
            (*this) << uint8_t(0);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get contents
    const uint8_t * ByteBuffer::GetData() const 
    { 
        return &m_Storage[0]; 
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get Size
    size_t ByteBuffer::GetSize() const 
    { 
        return m_Storage.size(); 
    }
    /// Get is empty
    bool ByteBuffer::IsEmpty() const
    { 
        return m_Storage.empty(); 
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Resize
    void ByteBuffer::Resise(size_t pNewSize)
    {
        m_Storage.resize(pNewSize);
        m_ReadPos = 0;
        m_WritePos = GetSize();
    }
    /// Reserve
    void ByteBuffer::Reserve(size_t pResize)
    {
        if (pResize > GetSize())
            m_Storage.reserve(pResize);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Append string
    void ByteBuffer::Append(const std::string& pStr)
    {
        Append((uint8_t const*)pStr.c_str(), pStr.size() + 1);
    }
    /// Append string
    void ByteBuffer::Append(const char *pSrc, size_t pCnt)
    {
        return Append((const uint8_t *)pSrc, pCnt);
    }
    /// Append
    void ByteBuffer::Append(const uint8_t *pSrc, size_t pCnt)
    {
        if (!pCnt)
            return;

        //assert(GetSize() < 10000000);

        if (m_Storage.size() < m_WritePos + pCnt)
            m_Storage.resize(m_WritePos + pCnt);

        memcpy(&m_Storage[m_WritePos], pSrc, pCnt);

        m_WritePos += pCnt;
    }
    /// Append
    void ByteBuffer::Append(const ByteBuffer& pBuffer)
    {
        if (pBuffer.WritePos())
            Append(pBuffer.GetData(), pBuffer.WritePos());
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Put
    void ByteBuffer::Put(size_t pPos, const uint8_t *pSrc, size_t pCount)
    {
        //if (pPos + pCount > GetSize())
        //    throw Core::Exception::Core(__FILE__, __LINE__, "Put overflow");

        memcpy(&m_Storage[pPos], pSrc, pCount);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Read a pack GUID
    uint64_t ByteBuffer::ReadPackGUID()
    {
        uint64_t l_GUID = 0;
        uint8_t l_GUIDMark = 0;
        (*this) >> l_GUIDMark;

        for (int l_I = 0; l_I < 8; ++l_I)
        {
            if (l_GUIDMark & (uint8_t(1) << l_I))
            {
                uint8_t l_Bit;
                (*this) >> l_Bit;
                l_GUID |= (uint64_t(l_Bit) << (l_I * 8));
            }
        }

        return l_GUID;
    }

}   ///< namespace Network
}   ///< namespace RCN