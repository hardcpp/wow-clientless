#pragma once

#include "../Network/TCPClient.hpp"
#include "../Crypto/BigNumber.hpp"
#include <string>
#include <stdint.h>
#include <vector>

namespace RCN { namespace Logon {

    struct RealmInfo
    {
        uint32_t    Icon;
        uint8_t     Flags;
        std::string Name;
        std::string Address;
        float       PopulationLevel;
        uint8_t     AmountOfCharacters;
        uint8_t     TimeZone;
        uint8_t     UnkRealmID;
    };

    class LogonManager
    {
        public:
            /// Constructor
            /// @p_Server   : Server address
            /// @p_Port     : Auth server port
            /// @p_User     : Username
            /// @p_Password : Password
            LogonManager(std::string p_Server, uint16_t p_Port, std::string p_User, std::string p_Password);

            /// Do the connection
            bool Connect();

        public:
            Crypto::BigNumber       SessionKey;     ///< Account session key
            std::vector<RealmInfo>  Realms;         ///< Realms

        private:
            /// Send ClientMessage::CMD_AUTH_LOGON_CHALLENGE
            void SendAuthLogonChallenge();

            /// Handle Opcodes::CMD_AUTH_LOGON_CHALLENGE
            void HandleAuthLogonChallenge();
            /// Handle Opcodes::CMD_AUTH_LOGON_PROOF
            void HandleAuthLogonProof();
            /// Handle Opcodes::CMD_REALM_LIST
            void HandleRealmList();

        private:
            std::string m_Server;       ///< Logon server address
            uint16_t    m_Port;         ///< Logon server port
            std::string m_User;         ///< Account name
            std::string m_Password;     ///< Account password

            Network::TCPClient m_Client;    ///< TCP Client

            bool m_Connecting;          ///< Is currently connecting
            bool m_IsAuthed;            ///< Is correctly authed
    };

}   ///< namespace Logon
}   ///< namespace RCN
