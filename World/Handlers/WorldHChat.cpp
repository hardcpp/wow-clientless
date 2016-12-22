#include "WorldHChat.hpp"
#include "../Enums/WorldEnums.hpp"
#include "../Network/WorldOpcodes.hpp"

namespace RCN { namespace World { namespace Handlers {

    struct ChatMessage
    {
        uint8_t MessageType;
        uint32_t Language;
        std::string Message;
        uint8_t ChatTag;

        struct
        {
            uint64_t    GUID;
            std::string Name;
            uint32_t    ChannelRank;
        } Sender, Target;

        struct
        {
            std::string Name;
        } Channel;
    };

    /// Handle SMSG_MESSAGECHAT
    /// @p_Packet   : Packet
    /// @p_Manager  : World manager
    void Handle_MessageChat(Network::WorldPacket& p_Packet, WorldManager* p_Manager)
    {
        ChatMessage l_Message;

        p_Packet >> l_Message.MessageType;
        p_Packet >> l_Message.Language;

        switch (static_cast<Enums::ChatMessageType::Type>(l_Message.MessageType))
        {
            case Enums::ChatMessageType::CHAT_MSG_MONSTER_WHISPER:
            case Enums::ChatMessageType::CHAT_MSG_RAID_BOSS_WHISPER:
            case Enums::ChatMessageType::CHAT_MSG_RAID_BOSS_EMOTE:
            case Enums::ChatMessageType::CHAT_MSG_MONSTER_EMOTE:
                p_Packet.ReadSkip<uint32_t>();
                p_Packet >> l_Message.Sender.Name;
                p_Packet >> l_Message.Target.GUID;
                break;

            case Enums::ChatMessageType::CHAT_MSG_SAY:
            case Enums::ChatMessageType::CHAT_MSG_PARTY:
            case Enums::ChatMessageType::CHAT_MSG_YELL:
                p_Packet >> l_Message.Sender.GUID;
                p_Packet >> l_Message.Sender.GUID;
                break;

            case Enums::ChatMessageType::CHAT_MSG_MONSTER_SAY:
            case Enums::ChatMessageType::CHAT_MSG_MONSTER_YELL:
                p_Packet >> l_Message.Sender.GUID;
                p_Packet.ReadSkip<uint32_t>();
                p_Packet >> l_Message.Sender.Name;
                p_Packet >> l_Message.Target.GUID;
                break;

            case Enums::ChatMessageType::CHAT_MSG_CHANNEL:
                p_Packet >> l_Message.Channel.Name;
                p_Packet >> l_Message.Sender.ChannelRank;
                p_Packet >> l_Message.Sender.GUID;
                break;

            default:
                p_Packet >> l_Message.Sender.GUID;
                break;
        }

        p_Packet.ReadSkip<uint32_t>();
        p_Packet >> l_Message.Message;
        p_Packet >> l_Message.ChatTag;

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        printf("[CHAT]");
        switch (static_cast<Enums::ChatMessageType::Type>(l_Message.MessageType))
        {

        case Enums::ChatMessageType::CHAT_MSG_SAY:
            printf("[SAY]");
            break;
        case Enums::ChatMessageType::CHAT_MSG_PARTY:
            printf("[PARTY]");
            break;
        case Enums::ChatMessageType::CHAT_MSG_YELL:
            printf("[YELL]");
            break;

        case Enums::ChatMessageType::CHAT_MSG_MONSTER_SAY:
            printf("[MONTER_SAY]");
            break;

        case Enums::ChatMessageType::CHAT_MSG_MONSTER_YELL:
            printf("[MONTER_YELL]");
            break;

        case Enums::ChatMessageType::CHAT_MSG_CHANNEL:
            printf("[%s]", l_Message.Channel.Name.c_str());
            break;
        }
        printf(" : %s\n", l_Message.Message.c_str());
    }

}   ///< namespace Handlers
}   ///< namespace World
}   ///< namespace RCN