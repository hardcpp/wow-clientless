/// Append
template <typename T> void ByteBuffer::Append(T p_Value)
{
    Append((uint8_t *)&p_Value, sizeof(p_Value));
}
/// Put
template <typename T> void ByteBuffer::Put(size_t p_Pos, T p_Value)
{
    Put(p_Pos, (uint8_t *)&p_Value, sizeof(p_Value));
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Skip read of an element
template<typename T> void ByteBuffer::ReadSkip() 
{ 
    ReadSkip(sizeof(T));
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Read
template <typename T> T ByteBuffer::Read()
{
    T l_Res = Read<T>(m_ReadPos);
    m_ReadPos += sizeof(T);
    return l_Res;
}
/// Read
template <typename T> T ByteBuffer::Read(size_t p_Pos) const
{
    //if (p_Pos + sizeof(T) > GetSize())
    //    throw Core::Exception::Core(__FILE__, __LINE__, "Read overflow");

    T l_Val = *((T const*)&m_Storage[p_Pos]);
    return l_Val;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Append
template<class T> void Append(const T *p_Src, size_t p_Cnt)
{
    return Append((const uint8_t *)p_Src, p_Cnt * sizeof(T));
}
