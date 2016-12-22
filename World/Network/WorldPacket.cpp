#include "WorldPacket.hpp"
#include "WorldOpcodes.hpp"

namespace RCN { namespace World { namespace Network {

    /// Constructor
    /// @p_Opcode   : New opcode
    /// @p_Reserve  : Packet reserve
    WorldPacket::WorldPacket()
        : ByteBuffer(0), m_Opcode(Opcodes::MSG_NULL_ACTION)
    {
    }
    /// Constructor
    WorldPacket::WorldPacket(uint16_t p_Opcode, size_t p_Reserve)
        : ByteBuffer(p_Reserve), m_Opcode(p_Opcode)
    {
    }
    /// Copy constructor
    /// @p_Packet : Packet to copy
    WorldPacket::WorldPacket(const WorldPacket& p_Packet)
        : ByteBuffer(p_Packet), m_Opcode(p_Packet.m_Opcode)
    {
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Initialize
    /// @p_Opcode   : New opcode
    /// @p_Reserve  : Packet reserve
    void WorldPacket::Initialize(uint16_t p_Opcode, size_t p_Reserve)
    {
        Clear();
        m_Storage.reserve(p_Reserve);
        m_Opcode = p_Opcode;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get packet opcode
    uint16_t WorldPacket::GetOpcode() const
    {
        return m_Opcode;
    }
    /// Set the packet opcode
    /// @p_Opcode : New opcode
    void WorldPacket::SetOpcode(uint16_t p_Opcode)
    {
        m_Opcode = p_Opcode;
    }

}   ///< namespace Network
}   ///< namespace World
}   ///< namespace RCN
