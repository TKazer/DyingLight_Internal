#include "Render.h"

void Render::DrawLosLine(Object& Obj,float Length, ImColor Color, float ThickNess)
{
    Vec3 StartPoint, EndPoint;
    Vec3 HeadPos= Obj.GetBonePos(Bone::Head);
    Vec3 Temp;
    if (!pGame->pCameraManage->Camera->WorldToScreen(HeadPos, StartPoint))
        return;
    Temp.x = HeadPos.x + cos(Obj.Yaw * M_PI / 180) * Length;
    Temp.y = HeadPos.y;
    Temp.z = HeadPos.z + sin(Obj.Yaw * M_PI / 180) * Length;
    if (!pGame->pCameraManage->Camera->WorldToScreen(Temp, EndPoint))
        return;
    Gui.Line(StartPoint, EndPoint, Color, ThickNess);
}

void Render::DrawBone(Object& Obj, ImColor Color, float ThickNess, bool View)
{
    Vec3 Previous, Current;
    Obj.BonePosList.clear();
    for (auto i : Bone::Single::List)
    {
        Previous = Vec3(0, 0, 0);
        for (auto ID : i)
        {
            Current = Obj.GetBonePos(ID);
            if (Previous == 0)
            {
                Obj.BonePosList.push_back(Current);
                Previous = Current;
                continue;
            }
            Vec3 PreviousScPos, CurrentScPos;
            if (pGame->pCameraManage->Camera->WorldToScreen(Previous, PreviousScPos) && pGame->pCameraManage->Camera->WorldToScreen(Current, CurrentScPos))
            {
                Obj.BonePosList.push_back(Current);
                if (View)
                {
                    Gui.Line(PreviousScPos, CurrentScPos, Color, ThickNess);
                }
            }
            Previous = Current;
        }
    }
}

void Render::AABB(Object& Obj, ImColor Color, float ThickNess)
{
    std::vector<Vec2> RevolvedBonePosArray;	// 旋转后骨骼坐标数组

    RECT_3DPOS Bottom, Top;
    Bottom.Pos.resize(4, {});
    Bottom.ScreenPos.resize(4, {});
    Top.Pos.resize(4, {});
    Top.ScreenPos.resize(4, {});
    Vec2 Temp_Screen;
    Vec2 OriginPos{ Obj.Pos.x,Obj.Pos.z };	// 原点

    float _left, _right, _top, _bottom, _low, _high;
    float _height;

    // 旋转坐标系
    int BonesPosListSize = Obj.BonePosList.size();
    for (int i = 0; i < BonesPosListSize; i++)
        RevolvedBonePosArray.push_back(Function::RevolveCoordinatesSystem(Obj.Yaw, OriginPos, { Obj.BonePosList[i].x, Obj.BonePosList[i].z }));

    // 计算边缘点

    _left = _right = RevolvedBonePosArray[0].x;
    _top = _bottom = RevolvedBonePosArray[0].y;
    _high = _low = Obj.BonePosList[0].y;
    for (int i = 0; i < BonesPosListSize; i++)
    {
        _left = min(_left, RevolvedBonePosArray[i].x);
        _right = max(_right, RevolvedBonePosArray[i].x);
        _top = max(_top, RevolvedBonePosArray[i].y);
        _bottom = min(_bottom, RevolvedBonePosArray[i].y);
        _low = min(_low, Obj.BonePosList[i].y);
        _high = max(_high, Obj.BonePosList[i].y);
    }
    _low -= 0.3;
    _height = _high - _low + 0.2;

    // 构造矩形

    Bottom.Pos[0] = { _left,_low,  _top };
    Bottom.Pos[1] = { _left,_low ,_bottom };
    Bottom.Pos[2] = { _right,_low ,_bottom };
    Bottom.Pos[3] = { _right,_low,_top };

    Top.Pos[0] = { _left, _low + _height,_top };
    Top.Pos[1] = { _left,_low + _height ,_bottom };
    Top.Pos[2] = { _right,_low + _height ,_bottom };
    Top.Pos[3] = { _right,_low + _height ,_top };

    // 转回原坐标系并转屏幕

    for (int i = 0; i < 4; i++)
    {
        Vec3 TempPos;
        Vec2 TempPos2;
        TempPos2 = Function::RevolveCoordinatesSystem(-Obj.Yaw, OriginPos, { Bottom.Pos[i].x,Bottom.Pos[i].z });

        Bottom.Pos[i].x = TempPos2.x;
        Bottom.Pos[i].z = TempPos2.y;
        if (!pGame->pCameraManage->Camera->WorldToScreen(Bottom.Pos[i], TempPos))
            return;
        Bottom.ScreenPos[i] = TempPos;

        Top.Pos[i].x = Bottom.Pos[i].x;
        Top.Pos[i].z = Bottom.Pos[i].z;
        if (!pGame->pCameraManage->Camera->WorldToScreen(Top.Pos[i], TempPos))
            return;
        Top.ScreenPos[i] = TempPos;
    }

    // 连接

    Gui.ConnectPoint(Bottom.ScreenPos, Color, ThickNess);
    Gui.ConnectPoint(Top.ScreenPos, Color, ThickNess);
    for (int i = 0; i < 4; i++)
        Gui.Line(Top.ScreenPos[i], Bottom.ScreenPos[i], Color, ThickNess);
}
