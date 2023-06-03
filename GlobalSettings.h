#pragma once
#include<graphics.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <vector>

using namespace std;

const double PI = 3.14159265358979;
constexpr int screen_width = 850;
constexpr int screen_height = 650;
constexpr int game_area_width = 680;
constexpr int game_area_height = screen_height;
constexpr int state_area_width = screen_width - game_area_width;
constexpr int state_area_height = screen_height;
constexpr unsigned int hero_HP = 4;
constexpr int max_scroll_speed = -1;
int scroll_speed = max_scroll_speed;

struct Position {
	int x;
	int y;
};

//用于存储物体的坐标信息
struct RouteNode {
    int id;
    int offsetY;
    Position SP;    //路径起点
    Position END;   //路径终点
    bool flag;  //标志位，生成时置1，未生成时置0
    unsigned int kind;   //类型
};

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

vector<RouteNode*>* ReadRouteNodeList(string dir) {
    vector<RouteNode*>* nodes = new vector<RouteNode*>();
    string line;

    ifstream file(dir);
    if (!file) {
        //cout << "无法打开文件 " << dir << endl;
        return nodes;
    }

    while (getline(file, line)) {
        RouteNode* node = new RouteNode();
        vector<string> tokens = split(line, ' ');

        // 读取并设置id属性
        node->id = stoi(tokens[0]);

        // 读取并设置offsetY属性
        node->offsetY = stoi(tokens[1]);

        // 读取并设置SP属性
        vector<string> spTokens = split(tokens[2], ',');
        node->SP.x = stoi(spTokens[0]);
        node->SP.y = stoi(spTokens[1]);

        // 读取并设置END属性
        vector<string> endTokens = split(tokens[3], ',');
        node->END.x = stoi(endTokens[0]);
        node->END.y = stoi(endTokens[1]);

        // 读取并设置flag属性
        node->flag = (tokens[4] == "1");

        // 读取并设置kind属性
        node->kind = stoi(tokens[5]);

        (*nodes).push_back(node);
    }
    return nodes;
}

void ReleaseRouteNodeList(vector<RouteNode*>* nodes) {
    // 遍历容器并删除每个元素指向的内存块
    for (auto& node : *nodes) {
        delete node;
    }

    // 清空容器
    nodes->clear();

    // 释放指向容器本身的内存块
    delete nodes;
}

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

string concatenateStringAndInt(const string& str, int num) {
    stringstream ss;
    ss << str << num;
    return ss.str();
}