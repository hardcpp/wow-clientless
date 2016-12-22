#pragma once

#include <stdint.h>

namespace RCN { namespace World { namespace Enums {

    namespace ResponseCodes
    {
        enum Type : uint8_t
        {
            RESPONSE_SUCCESS                                       = 0x00,
            RESPONSE_FAILURE                                       = 0x01,
            RESPONSE_CANCELLED                                     = 0x02,
            RESPONSE_DISCONNECTED                                  = 0x03,
            RESPONSE_FAILED_TO_CONNECT                             = 0x04,
            RESPONSE_CONNECTED                                     = 0x05,
            RESPONSE_VERSION_MISMATCH                              = 0x06,

            CSTATUS_CONNECTING                                     = 0x07,
            CSTATUS_NEGOTIATING_SECURITY                           = 0x08,
            CSTATUS_NEGOTIATION_COMPLETE                           = 0x09,
            CSTATUS_NEGOTIATION_FAILED                             = 0x0A,
            CSTATUS_AUTHENTICATING                                 = 0x0B,

            AUTH_OK                                                = 0x0C,
            AUTH_FAILED                                            = 0x0D,
            AUTH_REJECT                                            = 0x0E,
            AUTH_BAD_SERVER_PROOF                                  = 0x0F,
            AUTH_UNAVAILABLE                                       = 0x10,
            AUTH_SYSTEM_ERROR                                      = 0x11,
            AUTH_BILLING_ERROR                                     = 0x12,
            AUTH_BILLING_EXPIRED                                   = 0x13,
            AUTH_VERSION_MISMATCH                                  = 0x14,
            AUTH_UNKNOWN_ACCOUNT                                   = 0x15,
            AUTH_INCORRECT_PASSWORD                                = 0x16,
            AUTH_SESSION_EXPIRED                                   = 0x17,
            AUTH_SERVER_SHUTTING_DOWN                              = 0x18,
            AUTH_ALREADY_LOGGING_IN                                = 0x19,
            AUTH_LOGIN_SERVER_NOT_FOUND                            = 0x1A,
            AUTH_WAIT_QUEUE                                        = 0x1B,
            AUTH_BANNED                                            = 0x1C,
            AUTH_ALREADY_ONLINE                                    = 0x1D,
            AUTH_NO_TIME                                           = 0x1E,
            AUTH_DB_BUSY                                           = 0x1F,
            AUTH_SUSPENDED                                         = 0x20,
            AUTH_PARENTAL_CONTROL                                  = 0x21,
            AUTH_LOCKED_ENFORCED                                   = 0x02, /// Unsure

            REALM_LIST_IN_PROGRESS                                 = 0x22,
            REALM_LIST_SUCCESS                                     = 0x23,
            REALM_LIST_FAILED                                      = 0x24,
            REALM_LIST_INVALID                                     = 0x25,
            REALM_LIST_REALM_NOT_FOUND                             = 0x26,

            ACCOUNT_CREATE_IN_PROGRESS                             = 0x27,
            ACCOUNT_CREATE_SUCCESS                                 = 0x28,
            ACCOUNT_CREATE_FAILED                                  = 0x29,

            CHAR_LIST_RETRIEVING                                   = 0x2A,
            CHAR_LIST_RETRIEVED                                    = 0x2B,
            CHAR_LIST_FAILED                                       = 0x2C,

            CHAR_CREATE_IN_PROGRESS                                = 0x2D,
            CHAR_CREATE_SUCCESS                                    = 0x2E,
            CHAR_CREATE_ERROR                                      = 0x2F,
            CHAR_CREATE_FAILED                                     = 0x30,
            CHAR_CREATE_NAME_IN_USE                                = 0x31,
            CHAR_CREATE_DISABLED                                   = 0x3A,
            CHAR_CREATE_PVP_TEAMS_VIOLATION                        = 0x33,
            CHAR_CREATE_SERVER_LIMIT                               = 0x34,
            CHAR_CREATE_ACCOUNT_LIMIT                              = 0x35,
            CHAR_CREATE_SERVER_QUEUE                               = 0x30,/// UNSURE
            CHAR_CREATE_ONLY_EXISTING                              = 0x30,/// UNSURE

            CHAR_DELETE_IN_PROGRESS                                = 0x38,
            CHAR_DELETE_SUCCESS                                    = 0x39,
            CHAR_DELETE_FAILED                                     = 0x3A,
            CHAR_DELETE_FAILED_LOCKED_FOR_TRANSFER                 = 0x3A,/// UNSURE
            CHAR_DELETE_FAILED_GUILD_LEADER                        = 0x3A,/// UNSURE

            CHAR_LOGIN_IN_PROGRESS                                 = 0x3B,
            CHAR_LOGIN_SUCCESS                                     = 0x3C,
            CHAR_LOGIN_NO_WORLD                                    = 0x3D,
            CHAR_LOGIN_DUPLICATE_CHARACTER                         = 0x3E,
            CHAR_LOGIN_NO_INSTANCES                                = 0x3F,
            CHAR_LOGIN_FAILED                                      = 0x40,
            CHAR_LOGIN_DISABLED                                    = 0x41,
            CHAR_LOGIN_NO_CHARACTER                                = 0x42,
            CHAR_LOGIN_LOCKED_FOR_TRANSFER                         = 0x40, /// UNSURE
            CHAR_LOGIN_LOCKED_BY_BILLING                           = 0x40, /// UNSURE

            CHAR_NAME_SUCCESS                                      = 0x50,
            CHAR_NAME_FAILURE                                      = 0x4F,
            CHAR_NAME_NO_NAME                                      = 0x43,
            CHAR_NAME_TOO_SHORT                                    = 0x44,
            CHAR_NAME_TOO_LONG                                     = 0x45,
            CHAR_NAME_INVALID_CHARACTER                            = 0x46,
            CHAR_NAME_MIXED_LANGUAGES                              = 0x47,
            CHAR_NAME_PROFANE                                      = 0x48,
            CHAR_NAME_RESERVED                                     = 0x49,
            CHAR_NAME_INVALID_APOSTROPHE                           = 0x4A,
            CHAR_NAME_MULTIPLE_APOSTROPHES                         = 0x4B,
            CHAR_NAME_THREE_CONSECUTIVE                            = 0x4C,
            CHAR_NAME_INVALID_SPACE                                = 0x4D,
            CHAR_NAME_CONSECUTIVE_SPACES                           = 0x4E,
            CHAR_NAME_RUSSIAN_CONSECUTIVE_SILENT_CHARACTERS        = 0x4E,/// UNSURE
            CHAR_NAME_RUSSIAN_SILENT_CHARACTER_AT_BEGINNING_OR_END = 0x4E,/// UNSURE
            CHAR_NAME_DECLENSION_DOESNT_MATCH_BASE_NAME            = 0x4E,/// UNSURE
        };
    }

    namespace ObjectUpdateType
    {
        enum Type
        {
            UPDATETYPE_VALUES               = 0,
            UPDATETYPE_MOVEMENT             = 1,
            UPDATETYPE_CREATE_OBJECT        = 2,
            UPDATETYPE_CREATE_OBJECT2       = 3,
            UPDATETYPE_OUT_OF_RANGE_OBJECTS = 4,
            UPDATETYPE_NEAR_OBJECTS         = 5
        };
    }

    namespace ObjectUpdateFlags
    {
        enum Type
        {
            UPDATEFLAG_NONE                 = 0x0000,
            UPDATEFLAG_SELF                 = 0x0001,
            UPDATEFLAG_TRANSPORT            = 0x0002,
            UPDATEFLAG_FULLGUID             = 0x0004,
            UPDATEFLAG_HIGHGUID             = 0x0008,
            UPDATEFLAG_ALL                  = 0x0010,
            UPDATEFLAG_LIVING               = 0x0020,
            UPDATEFLAG_HAS_POSITION         = 0x0040
        };
    }

    namespace UnitMoveType
    {
        enum Type
        {
            MOVE_WALK           = 0,
            MOVE_RUN            = 1,
            MOVE_RUN_BACK       = 2,
            MOVE_SWIM           = 3,
            MOVE_SWIM_BACK      = 4,
            MOVE_TURN_RATE      = 5,
            MAX
        };
    }

    namespace MovementFlags
    {
        enum Type : uint32_t
        {
            MOVEFLAG_NONE             = 0x00000000,
            MOVEFLAG_FORWARD          = 0x00000001,
            MOVEFLAG_BACKWARD         = 0x00000002,
            MOVEFLAG_STRAFE_LEFT      = 0x00000004,
            MOVEFLAG_STRAFE_RIGHT     = 0x00000008,
            MOVEFLAG_TURN_LEFT        = 0x00000010,
            MOVEFLAG_TURN_RIGHT       = 0x00000020,
            MOVEFLAG_PITCH_UP         = 0x00000040,
            MOVEFLAG_PITCH_DOWN       = 0x00000080,
            MOVEFLAG_WALK_MODE        = 0x00000100,               ///< Walking

            MOVEFLAG_LEVITATING       = 0x00000400,
            MOVEFLAG_FLYING           = 0x00000800,               ///< [-ZERO] is it really need and correct value
            MOVEFLAG_FALLING          = 0x00002000,
            MOVEFLAG_FALLINGFAR       = 0x00004000,
            MOVEFLAG_SWIMMING         = 0x00200000,               ///< appears with fly flag also
            MOVEFLAG_SPLINE_ENABLED   = 0x00400000,
            MOVEFLAG_CAN_FLY          = 0x00800000,               ///< [-ZERO] is it really need and correct value
            MOVEFLAG_FLYING_OLD       = 0x01000000,               ///< [-ZERO] is it really need and correct value

            MOVEFLAG_ONTRANSPORT      = 0x02000000,               ///< Used for flying on some creatures
            MOVEFLAG_SPLINE_ELEVATION = 0x04000000,               ///< used for flight paths
            MOVEFLAG_ROOT             = 0x08000000,               ///< used for flight paths
            MOVEFLAG_WATERWALKING     = 0x10000000,               ///< prevent unit from falling through water
            MOVEFLAG_SAFE_FALL        = 0x20000000,               ///< active rogue safe fall spell (passive)
            MOVEFLAG_HOVER            = 0x40000000
        };
    }

    namespace EquipmentSlots
    {
        enum Type : uint16_t
        {
            EQUIPMENT_SLOT_START        = 0,
            EQUIPMENT_SLOT_HEAD         = 0,
            EQUIPMENT_SLOT_NECK         = 1,
            EQUIPMENT_SLOT_SHOULDERS    = 2,
            EQUIPMENT_SLOT_BODY         = 3,
            EQUIPMENT_SLOT_CHEST        = 4,
            EQUIPMENT_SLOT_WAIST        = 5,
            EQUIPMENT_SLOT_LEGS         = 6,
            EQUIPMENT_SLOT_FEET         = 7,
            EQUIPMENT_SLOT_WRISTS       = 8,
            EQUIPMENT_SLOT_HANDS        = 9,
            EQUIPMENT_SLOT_FINGER1      = 10,
            EQUIPMENT_SLOT_FINGER2      = 11,
            EQUIPMENT_SLOT_TRINKET1     = 12,
            EQUIPMENT_SLOT_TRINKET2     = 13,
            EQUIPMENT_SLOT_BACK         = 14,
            EQUIPMENT_SLOT_MAINHAND     = 15,
            EQUIPMENT_SLOT_OFFHAND      = 16,
            EQUIPMENT_SLOT_RANGED       = 17,
            EQUIPMENT_SLOT_TABARD       = 18,
            EQUIPMENT_SLOT_END          = 19
        };
    }

    namespace ChatMessageType
    {
        enum Type
        {
            CHAT_MSG_ADDON                  = 0xFFFFFFFF,
            CHAT_MSG_SAY                    = 0x00,
            CHAT_MSG_PARTY                  = 0x01,
            CHAT_MSG_RAID                   = 0x02,
            CHAT_MSG_GUILD                  = 0x03,
            CHAT_MSG_OFFICER                = 0x04,
            CHAT_MSG_YELL                   = 0x05,
            CHAT_MSG_WHISPER                = 0x06,
            CHAT_MSG_WHISPER_INFORM         = 0x07,
            CHAT_MSG_EMOTE                  = 0x08,
            CHAT_MSG_TEXT_EMOTE             = 0x09,
            CHAT_MSG_SYSTEM                 = 0x0A,
            CHAT_MSG_MONSTER_SAY            = 0x0B,
            CHAT_MSG_MONSTER_YELL           = 0x0C,
            CHAT_MSG_MONSTER_EMOTE          = 0x0D,
            CHAT_MSG_CHANNEL                = 0x0E,
            CHAT_MSG_CHANNEL_JOIN           = 0x0F,
            CHAT_MSG_CHANNEL_LEAVE          = 0x10,
            CHAT_MSG_CHANNEL_LIST           = 0x11,
            CHAT_MSG_CHANNEL_NOTICE         = 0x12,
            CHAT_MSG_CHANNEL_NOTICE_USER    = 0x13,
            CHAT_MSG_AFK                    = 0x14,
            CHAT_MSG_DND                    = 0x15,
            CHAT_MSG_IGNORED                = 0x16,
            CHAT_MSG_SKILL                  = 0x17,
            CHAT_MSG_LOOT                   = 0x18,
            CHAT_MSG_MONSTER_WHISPER        = 0x1A,
            CHAT_MSG_BG_SYSTEM_NEUTRAL      = 0x52,
            CHAT_MSG_BG_SYSTEM_ALLIANCE     = 0x53,
            CHAT_MSG_BG_SYSTEM_HORDE        = 0x54,
            CHAT_MSG_RAID_LEADER            = 0x57,
            CHAT_MSG_RAID_WARNING           = 0x58,
            CHAT_MSG_RAID_BOSS_WHISPER      = 0x59,
            CHAT_MSG_RAID_BOSS_EMOTE        = 0x5A,
            CHAT_MSG_BATTLEGROUND           = 0x5C,
            CHAT_MSG_BATTLEGROUND_LEADER    = 0x5D
        };
    }

}   ///< namespace Enums
}   ///< namespace World
}   ///< namespace RCN