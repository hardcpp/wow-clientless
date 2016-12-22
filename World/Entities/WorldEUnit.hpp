#pragma once

#include "WorldEObject.hpp"

namespace RCN { namespace World { namespace Entities {

    class MovementInfo
    {
        public:
            /// Constructor
            MovementInfo();

           // void Read(ByteBuffer& data);
           // void Write(ByteBuffer& data) const;

            /// Add movement flag
            /// @p_Flags : New flags
            void AddMovementFlag(Enums::MovementFlags::Type p_Flag);
            /// Remove movement flag
            /// @p_Flags : Old flags
            void RemoveMovementFlag(Enums::MovementFlags::Type p_Flag);
            /// Set movement flag
            /// @p_Flags : New flags
            bool HasMovementFlag(Enums::MovementFlags::Type p_Flag) const;
            /// Set movement flag
            /// @p_Flags : New flags
            void SetMovementFlags(Enums::MovementFlags::Type p_Flags);

            /// Set transport data
            /// @p_GUID : Transport GUID
            /// @p_X    : Transport X offset
            /// @p_Y    : Transport Y offset
            /// @p_Z    : Transport Z offset
            /// @p_O    : Transport O offset
            /// @p_Time : Transport time
            void SetTransportData(uint64_t p_GUID, float p_X, float p_Y, float p_Z, float p_O, uint32_t p_Time);
            /// Set jump info data
            /// @p_Velocity : Jump weight
            /// @p_SinAngle : Jump X axis coord
            /// @p_CosAngle : Jump Y axis coord
            /// @p_XYZSpeed : Jump axis speed 
            void SetJumpInfoData(float p_Velocity, float p_SinAngle, float p_CosAngle, float p_XYZSpeed);

            /// Reset transport data
            void ClearTransportData();
            /// Reset jump info data
            void ClearJumpInfoData();

        public:
            uint32_t    MovementFlags;
            uint32_t    Time;
            Position    Pos;
            float       Pitch;
            float       SplineElevation;
            uint32_t    FallTime;

            struct
            {
                uint64_t    GUID;       ///< Transport GUID
                Position    Pos;        ///< Transport position offset
                uint32_t    Time;       ///< Transport time
            } Transport;

            struct
            {
                float       Velocity;   ///< Jump weight
                float       SinAngle;   ///< Jump X axis coord
                float       CosAngle;   ///< Jump Y axis coord
                float       XYZSpeed;   ///< Jump axis speed 
            } JumpInfo;

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    class Unit : public Object
    {
        public:
            /// Constructor
            Unit();
            /// Destructor
            virtual ~Unit();

    };

}   ///< namespace Entities
}   ///< namespace World
}   ///< namespace RCN