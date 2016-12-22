#pragma once

#include "../../Network/TCPClient.hpp"
#include "../../Network/ByteBuffer.hpp"

namespace RCN { namespace World { namespace Network {

    class WorldPacket : public RCN::Network::ByteBuffer
    {
        public:
            /// Constructor
            /// @p_Opcode   : New opcode
            /// @p_Reserve  : Packet reserve
            WorldPacket();
            /// Constructor
            explicit WorldPacket(uint16_t p_Opcode, size_t p_Reserve = 200);
            /// Copy constructor
            /// @p_Packet : Packet to copy
            WorldPacket(const WorldPacket& p_Packet);

            /// Initialize
            /// @p_Opcode   : New opcode
            /// @p_Reserve  : Packet reserve
            void Initialize(uint16_t p_Opcode, size_t p_Reserve = 200);

            /// Get packet opcode
            uint16_t GetOpcode() const;
            /// Set the packet opcode
            /// @p_Opcode : New opcode
            void SetOpcode(uint16_t p_Opcode);

        private:
            uint16_t m_Opcode;  ///< Opcode value

    };

}   ///< namespace Network
}   ///< namespace World
}   ///< namespace RCN
