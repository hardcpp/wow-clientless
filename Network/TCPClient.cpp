#include "TCPClient.hpp"

namespace RCN { namespace Network {

    /// Constructor
    TCPClient::TCPClient()
    {
        m_Socket = socket(AF_INET, SOCK_STREAM, 0);
    }
    /// Destructor
    TCPClient::~TCPClient()
    {
        closesocket(m_Socket);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Connect to a server
    /// @p_Address  : Server address
    /// @p_Port     : Server port
    bool TCPClient::Connect(std::string p_Address, uint16_t p_Port)
    {
        std::lock_guard<std::mutex> l_Lock(m_Mutex);

        struct sockaddr_in l_Sin;

        /// DNS based address
        if (inet_addr(p_Address.c_str()) == -1)
        {
            struct hostent *l_Hostent;
            struct in_addr **l_AddrList;

            /// Query the DNS local service
            if ((l_Hostent = gethostbyname(p_Address.c_str())) == NULL)
                return false;

            /// Get address list
            l_AddrList = (struct in_addr **)l_Hostent->h_addr_list;

            /// Pick first valid IP address
            for (int l_I = 0; l_AddrList[l_I] != NULL; l_I++)
            {
                l_Sin.sin_addr = *l_AddrList[l_I];
                break;
            }
        }
        else
        {
            /// Single IP address
            l_Sin.sin_addr.s_addr = inet_addr(p_Address.c_str());
        }

        l_Sin.sin_family    = AF_INET;          ///< IPV4 mode
        l_Sin.sin_port      = htons(p_Port);    ///< Reverse port into big endian

        if (connect(m_Socket, (struct sockaddr*)&l_Sin, sizeof(l_Sin)) < 0)
            return false;

        return true;
    }
    /// Disconnect
    void TCPClient::Disconnect()
    {
        std::lock_guard<std::mutex> l_Lock(m_Mutex);
        closesocket(m_Socket);

        m_Socket = INVALID_SOCKET;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Send data
    /// @p_Data : Buffer
    /// @p_Size : Buffer size
    bool TCPClient::Send(const uint8_t* p_Data, uint32_t p_Size)
    {
        std::lock_guard<std::mutex> l_Lock(m_Mutex);

        size_t l_Remaining  = p_Size;
        char  *l_BufPtr     = (char*)p_Data;

        while (l_Remaining > 0)
        {
            size_t l_SentLen = send(m_Socket, l_BufPtr, l_Remaining, 0);
            if (l_SentLen <= 0)
                return false;

            l_Remaining -= l_SentLen;
            l_BufPtr    += l_SentLen;
        }

        return true;
    }
    /// Receive data
    /// @p_Dest : Destination buffer
    /// @p_Size : Waiting size
    bool TCPClient::Receive(uint8_t* p_Dest, uint32_t p_Size)
    {
        //std::lock_guard<std::mutex> l_Lock(m_Mutex);

        size_t l_Remaining  = p_Size;
        char  *l_BufPtr     = (char*)p_Dest;

        while (l_Remaining > 0)
        {
            size_t l_ReceivedLen = recv(m_Socket, l_BufPtr, l_Remaining, 0);
            if (l_ReceivedLen <= 0)
                return false;

            l_Remaining -= l_ReceivedLen;
            l_BufPtr    += l_ReceivedLen;
        }

        return true;
    }

}   ///< namespace Network
}   ///< namespace RCN