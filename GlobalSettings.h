#pragma once
#include<graphics.h>
#include <string>
#include <sstream>
#include <Windows.h>

using namespace std;

const double PI = 3.14159265358979;
constexpr int screen_width = 850;
constexpr int screen_height = 650;
constexpr int game_area_width = 640;
constexpr int game_area_height = screen_height;
constexpr int state_area_width = screen_width - game_area_width;
constexpr int state_area_height = screen_height;
constexpr unsigned int hero_HP = 4;
constexpr int scroll_speed = 0;

struct Position {
	int x;
	int y;
};

//判断两个矩形是否相撞
bool RectCrush(RECT& r1, RECT& r2) {
	RECT r;
	r.left = r1.left - (r2.right - r2.left);
	r.right = r1.right;
	r.top = r1.top - (r2.bottom - r2.top);
	r.bottom = r1.bottom;

	return (r.left < r2.left&& r2.left < r.right&& r.top < r2.top&& r2.top < r.bottom);
}

//判断点击是否在矩形区域内
bool PointInRect(int x, int y, RECT& r) {
	return (r.left <= x && x <= r.right && r.top <= y && y <= r.bottom);
}

LPCTSTR IntToLPCTSTR(int num)
{
    // 使用stringstream将int类型的变量转换为string类型
    stringstream ss;
    ss << num;
    string str = ss.str();

    // 将string类型转换为LPCTSTR类型
    size_t len = str.length() + 1;
    wchar_t* buf = new wchar_t[len];
    size_t converted = 0;
    mbstowcs_s(&converted, buf, len, str.c_str(), _TRUNCATE);
    LPCTSTR lpStr = buf;

    return lpStr;
}