#include "WorldEUnit.hpp"

namespace RCN { namespace World { namespace Entities {

    /// Constructor
    MovementInfo::MovementInfo()
    {
        MovementFlags = 0;
        Time = 0;
        Pos.X = 0.0f;
        Pos.Y = 0.0f;
        Pos.Z = 0.0f;
        Pos.O = 0.0f;
        Pitch = 0.0f;
        SplineElevation = 0.0f;
        FallTime = 0;

        ClearTransportData();
        ClearJumpInfoData();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Add movement flag
    /// @p_Flags : New flags
    void MovementInfo::AddMovementFlag(Enums::MovementFlags::Type p_Flag)
    {
        this->MovementFlags |= p_Flag;
    }
    /// Remove movement flag
    /// @p_Flags : Old flags
    void MovementInfo::RemoveMovementFlag(Enums::MovementFlags::Type p_Flag)
    {
        this->MovementFlags &= ~p_Flag;
    }
    /// Set movement flag
    /// @p_Flags : New flags
    bool MovementInfo::HasMovementFlag(Enums::MovementFlags::Type p_Flag) const
    {
        return (this->MovementFlags & p_Flag) != 0;
    }
    /// Set movement flag
    /// @p_Flags : New flags
    void MovementInfo::SetMovementFlags(Enums::MovementFlags::Type p_Flags)
    {
        this->MovementFlags = p_Flags;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set transport data
    /// @p_GUID : Transport GUID
    /// @p_X    : Transport X offset
    /// @p_Y    : Transport Y offset
    /// @p_Z    : Transport Z offset
    /// @p_O    : Transport O offset
    /// @p_Time : Transport time
    void MovementInfo::SetTransportData(uint64_t p_GUID, float p_X, float p_Y, float p_Z, float p_O, uint32_t p_Time)
    {
        this->Transport.GUID = p_GUID;
        this->Transport.Pos.X = p_X;
        this->Transport.Pos.Y = p_Y;
        this->Transport.Pos.Z = p_Z;
        this->Transport.Pos.O = p_O;
        this->Transport.Time = p_Time;
    }
    /// Set jump info data
    /// @p_Velocity : Jump weight
    /// @p_SinAngle : Jump X axis coord
    /// @p_CosAngle : Jump Y axis coord
    /// @p_XYZSpeed : Jump axis speed 
    void MovementInfo::SetJumpInfoData(float p_Velocity, float p_SinAngle, float p_CosAngle, float p_XYZSpeed)
    {
        this->JumpInfo.Velocity = p_Velocity;
        this->JumpInfo.SinAngle = p_SinAngle;
        this->JumpInfo.CosAngle = p_CosAngle;
        this->JumpInfo.XYZSpeed = p_XYZSpeed;
    }

    /// Reset transport data
    void MovementInfo::ClearTransportData()
    {
        this->Transport.GUID = 0;
        this->Transport.Pos.X = 0.0f;
        this->Transport.Pos.Y = 0.0f;
        this->Transport.Pos.Z = 0.0f;
        this->Transport.Pos.O = 0.0f;
        this->Transport.Time = 0;
    }
    /// Reset jump info data
    void MovementInfo::ClearJumpInfoData()
    {
        this->JumpInfo.Velocity = 0.0f;
        this->JumpInfo.SinAngle = 0.0f;
        this->JumpInfo.CosAngle = 0.0f;
        this->JumpInfo.XYZSpeed = 0.0f;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////



}   ///< namespace Entities
}   ///< namespace World
}   ///< namespace RCN