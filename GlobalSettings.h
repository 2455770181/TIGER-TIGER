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

//���ڴ洢�����������Ϣ
struct RouteNode {
    int id;
    int offsetY;
    Position SP;    //·�����
    Position END;   //·���յ�
    bool flag;  //��־λ������ʱ��1��δ����ʱ��0
    unsigned int kind;   //����
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
        //cout << "�޷����ļ� " << dir << endl;
        return nodes;
    }

    while (getline(file, line)) {
        RouteNode* node = new RouteNode();
        vector<string> tokens = split(line, ' ');

        // ��ȡ������id����
        node->id = stoi(tokens[0]);

        // ��ȡ������offsetY����
        node->offsetY = stoi(tokens[1]);

        // ��ȡ������SP����
        vector<string> spTokens = split(tokens[2], ',');
        node->SP.x = stoi(spTokens[0]);
        node->SP.y = stoi(spTokens[1]);

        // ��ȡ������END����
        vector<string> endTokens = split(tokens[3], ',');
        node->END.x = stoi(endTokens[0]);
        node->END.y = stoi(endTokens[1]);

        // ��ȡ������flag����
        node->flag = (tokens[4] == "1");

        // ��ȡ������kind����
        node->kind = stoi(tokens[5]);

        (*nodes).push_back(node);
    }
    return nodes;
}

void ReleaseRouteNodeList(vector<RouteNode*>* nodes) {
    // ����������ɾ��ÿ��Ԫ��ָ����ڴ��
    for (auto& node : *nodes) {
        delete node;
    }

    // �������
    nodes->clear();

    // �ͷ�ָ������������ڴ��
    delete nodes;
}

//�ж����������Ƿ���ײ
bool RectCrush(RECT& r1, RECT& r2) {
	RECT r;
	r.left = r1.left - (r2.right - r2.left);
	r.right = r1.right;
	r.top = r1.top - (r2.bottom - r2.top);
	r.bottom = r1.bottom;

	return (r.left < r2.left&& r2.left < r.right&& r.top < r2.top&& r2.top < r.bottom);
}

//�жϵ���Ƿ��ھ���������
bool PointInRect(int x, int y, RECT& r) {
	return (r.left <= x && x <= r.right && r.top <= y && y <= r.bottom);
}

LPCTSTR IntToLPCTSTR(int num)
{
    // ʹ��stringstream��int���͵ı���ת��Ϊstring����
    stringstream ss;
    ss << num;
    string str = ss.str();

    // ��string����ת��ΪLPCTSTR����
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