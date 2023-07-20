#pragma once
#include <Windows.h>
#include <vector>

using std::vector;

struct Vec3 {
	float x, y, z;
	Vec3() { x = 0; y = 0; z = 0; }
	Vec3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
	bool operator==(const float _t){
		return x == _t && y == _t && z == _t;
	}
	bool operator<(const float _t) {
		return x < _t&& y < _t&& z < _t;
	}
	bool operator>(const float _t) {
		return x > _t&& y > _t&& z > _t;
	}
	bool operator>=(const float _t) {
		return x >= _t && y >= _t && z >= _t;
	}
	bool operator<=(const float _t) {
		return x <= _t && y <= _t && z <= _t;
	}
	bool operator!=(const float _t) {
		return x != _t || y != _t || z != _t;
	}
};
struct Vec4
{
	float x, y, z, w;
};

struct Vec2 {
	float x, y;
	Vec2() { x = 0; y = 0; }
	Vec2(float _x, float _y) { x = _x; y = _y; }
	Vec2(Vec3 _t) { x = _t.x; y = _t.y; }
	bool operator==(const float _t) {
		return x == _t && y == _t;
	}
	bool operator<(const float _t) {
		return x < _t&& y < _t;
	}
	bool operator>(const float _t) {
		return x > _t && y > _t;
	}
	bool operator>=(const float _t) {
		return x >= _t && y >= _t;
	}
	bool operator<=(const float _t) {
		return x <= _t && y <= _t;
	}
	bool operator!=(const float _t) {
		return x != _t && y != _t;
	}
};

// 3D方框坐标点
struct RECT_3DPOS {
	vector<Vec3> Pos;
	vector<Vec2> ScreenPos;
};

// 矩形结构
struct RECTINFO {
	float  width, height;
	Vec2  point;
};

struct Rotator {
	float Yaw, Pitch, Roll;
};
