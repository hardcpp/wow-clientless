#pragma once

#include <stdint.h>

namespace RCN { namespace Logon {

    #if defined( __GNUC__ )
        #pragma pack(1)
    #else
        #pragma pack(push,1)
    #endif

    struct Header
    {
        uint8_t   Opcode;
    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    struct Client_AuthLogonChallenge
    {
        Header    _Header;
        uint8_t   Error;
        uint16_t  Size;
        uint8_t   GameName[4];
        uint8_t   Version1;
        uint8_t   Version2;
        uint8_t   Version3;
        uint16_t  Build;
        uint8_t   Platform[4];
        uint8_t   OS[4];
        uint8_t   Country[4];
        uint32_t  TimezoneBias;
        uint32_t  IP;
        uint8_t   UsernameLen;
        uint8_t   Username[15];
    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    struct Server_GruntAuthChallenge
    {
        uint8_t   Error;
        uint8_t   Result;
    };

    struct Server_GruntAuthChallenge_OnSuccess
    {
        uint8_t B[32];
        uint8_t GLen;
        uint8_t G[1];
        uint8_t NLen;
        uint8_t N[32];
        uint8_t Salt[32];
        uint8_t CRCSalt[16];
        uint8_t SecurityFlag;
    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    struct Client_AuthLogonProof
    {
        Header    _Header;
        uint8_t   A[32];
        uint8_t   M1[20];
        uint8_t   CRCHash[20];
        uint8_t   SurveyID;
        uint8_t   SecurityFlags;
    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    struct Server_GruntAuthProof
    {
        uint8_t   Result;
    };

    struct Server_GruntAuthProof_OnSuccess
    {
        uint8_t   M2[20];
        uint32_t  Unk2;
    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    struct Client_AuthRealmList
    {
        Header    _Header;
        uint32_t   Unk;
    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    struct Server_AuthRealmList
    {
        uint16_t   Size;
    };

    #if defined( __GNUC__ )
        #pragma pack()
    #else
        #pragma pack(pop)
    #endif

}   ///< namespace Logon
}   ///< namespace RCN
