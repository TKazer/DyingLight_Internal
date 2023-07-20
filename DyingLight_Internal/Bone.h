#pragma once
#include <Windows.h>
#include <list>

namespace Bone
{
    enum BoneAllID : DWORD
    {
        Pelvis = 0,
        Spine,
        Spine1,
        Spine2,
        Spine3,
        Neck,
        Neck1,
        Neck2,
        Head,
        EyeCamera,
        LClavicle,
        LUpperArm,
        LForeArm,
        LHand,
        RClavicle,
        RUpperArm,
        RForeArm,
        RHand,
        LThigh,
        RThigh,
        LCalf,
        RCalf,
        LFoot,
        RFoot,
        LSole,
        RSole
    };

    namespace Single
    {
        std::list<DWORD> Trunk = { Head,Neck,Spine3,Spine2,Spine1,Pelvis };
        std::list<DWORD> LeftArm = { Neck,LUpperArm,LForeArm ,LHand };
        std::list<DWORD> RightArm = { Neck,RUpperArm,RForeArm ,RHand };
        std::list<DWORD> LeftLeg = { Pelvis,LThigh ,LCalf,LFoot };
        std::list<DWORD> RightLeg = { Pelvis,RThigh ,RCalf,RFoot };
        std::list<std::list<DWORD>> List = { Trunk,LeftArm ,RightArm ,LeftLeg ,RightLeg };
    }
}