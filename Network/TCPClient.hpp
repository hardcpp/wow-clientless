#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include <winsock2.h>
#include <string>
#include <stdint.h>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

namespace RCN { namespace Network {

    class TCPClient
    {
        public:
            /// Constructor
            TCPClient();
            /// Destructor
            ~TCPClient();

            /// Connect to a server
            /// @p_Address  : Server address
            /// @p_Port     : Server port
            bool Connect(std::string p_Address, uint16_t p_Port);
            /// Disconnect
            void Disconnect();

            /// Send data
            /// @p_Data : Buffer
            /// @p_Size : Buffer size
            bool Send(const uint8_t* p_Data, uint32_t p_Size);
            /// Receive data
            /// @p_Dest : Destination buffer
            /// @p_Size : Waiting size
            bool Receive(uint8_t* p_Dest, uint32_t p_Size);

        private:
            std::mutex  m_Mutex;     ///< Mutex
            SOCKET      m_Socket;    ///< Socket ID
    };

}   ///< namespace Network
}   ///< namespace RCN