#pragma once
#include<math.h>
#include<iostream>
#include"Hero.h"
#include"GlobalSettings.h"
using namespace std;

class BaseProperty {
public:
	void Show();
	void UpdatePosition();
	bool IfGotten(Hero* hero, Position tg);
	void SetTarget(Position P);
	bool ReachTarget();
	RECT& GetRect();
	Position& GetPosition();
	int GetID();
protected:
	int ID;
	IMAGE img_list;
	IMAGE mask_list;
	IMAGE* current_img;
	IMAGE* current_mask;

	bool start_flag;	//在被英雄捡到时置1，并且只触发一次
	int delta_x;
	RECT rect;
	Position position;
	Position target;
	float fun_a;	//直线线y=ax+b的系数
	float fun_b;
};
void BaseProperty::Show() {
	putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_mask, SRCAND);
	putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_img, SRCPAINT);
}
void BaseProperty::UpdatePosition() {
	if (start_flag) {
		if (position.x >= target.x)return;
		else {
			position.x = (position.x + delta_x > target.x) ? target.x : position.x + delta_x;
			position.y = fun_a * position.x + fun_b;
		}
	}
	else {
		position.y += scroll_speed;
	}
	rect.top = position.y - current_img->getheight() / 2;
	rect.right = position.x + current_img->getwidth() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;
	rect.left = position.x - current_img->getwidth() / 2;
}
bool BaseProperty::IfGotten(Hero* hero, Position tg) {
	if (start_flag == 1) return 1;
	else {
		if (RectCrush(rect, hero->GetRect())) {
			start_flag = 1;
			SetTarget(tg);
			fun_a = (float)(position.y - target.y) / (float)(position.x - target.x);
			fun_b = target.y - fun_a * target.x;
		}
		else {

		}
	}
	return RectCrush(rect, hero->GetRect());
}
void BaseProperty::SetTarget(Position P) {
	target.x = P.x;
	target.y = P.y;
}
bool BaseProperty::ReachTarget() {
	//不比较y是因为计算可能有误差。
	return (position.x == target.x) ? 1 : 0;
}
RECT& BaseProperty::GetRect() {
	return rect;
}
Position& BaseProperty::GetPosition() {
	return position;
}
int BaseProperty::GetID() {
	return ID;
}

class Box :public BaseProperty {
public:
	Box(Position P, int id) {
		ID = id;

		loadimage(&img_list, _T("./image/prop/box_i.png"));
		loadimage(&mask_list, _T("./image/prop/box_m.png"));
		current_img = &img_list;
		current_mask = &mask_list;

		start_flag = 0;
		delta_x = 4;

		position.x = P.x;
		position.y = P.y;
		target.x = game_area_width + 80;
		target.y = game_area_height / 3;

		rect.top = position.y - current_img->getheight() / 2;
		rect.right = position.x + current_img->getwidth() / 2;
		rect.bottom = position.y + current_img->getheight() / 2;
		rect.left = position.x - current_img->getwidth() / 2;

		fun_a = 0;
		fun_b = 0;

		//cout << "生成Box，ID为"<< id << endl;
	}
	~Box() {
		//cout << "Box" << ID << "被销毁" << endl;
	}
};

class SmallBox :public BaseProperty {
public:
	SmallBox(Position P, int id) {
		ID = id;

		loadimage(&img_list, _T("./image/prop/smallbox_i.png"));
		loadimage(&mask_list, _T("./image/prop/smallbox_m.png"));
		current_img = &img_list;
		current_mask = &mask_list;

		start_flag = 0;
		delta_x = 4;

		position.x = P.x;
		position.y = P.y;
		target.x = game_area_width + 40;
		target.y = game_area_height * 3 / 5;

		rect.top = position.y - current_img->getheight() / 2;
		rect.right = position.x + current_img->getwidth() / 2;
		rect.bottom = position.y + current_img->getheight() / 2;
		rect.left = position.x - current_img->getwidth() / 2;

		fun_a = 0;
		fun_b = 0;

		//cout << "生成SmallBox，ID为" << id << endl;
	}
	~SmallBox() {
		//cout << "SmallBox" << ID << "被销毁" << endl;
	}
};

class Ether :public BaseProperty {
public:
	static IMAGE* img_list;
	static IMAGE* mask_list;
	static void SetImageAndMask();
	static void ReleaseMemory();
	Ether(Position P, int id) {
		ID = id;

		current_img = img_list;
		current_mask = mask_list;

		start_flag = 0;
		delta_x = 4;

		position.x = P.x;
		position.y = P.y;
		target.x = screen_width - 40;
		target.y = screen_height / 5;

		rect.top = position.y - current_img->getheight() / 2;
		rect.right = position.x + current_img->getwidth() / 2;
		rect.bottom = position.y + current_img->getheight() / 2;
		rect.left = position.x - current_img->getwidth() / 2;

		fun_a = 0;
		fun_b = 0;

		//cout << "生成Ether，ID为" << id << endl;
	}
	~Ether() {
		//cout << "Ether" << ID << "被销毁" << endl;
	}
};
IMAGE* Ether::img_list = NULL;
IMAGE* Ether::mask_list = NULL;
void Ether::SetImageAndMask() {
	//current_img 的初始化值是 nullptr，因此在 Brick 类的静态成员函数 SetImageAndMask 中，
	//需要为 img_list 和 mask_list 分配内存并加载图像。否则，在创建 Brick 对象时，current_img 仍然是空指针。
	img_list = new IMAGE;
	loadimage((Ether::img_list), _T("./image/prop/ether_i.png"));
	mask_list = new IMAGE;
	loadimage((Ether::mask_list), _T("./image/prop/ether_m.png"));
}
void Ether::ReleaseMemory() {
	delete img_list;
	delete mask_list;
}

class EnhancedHook :public BaseProperty {
public:
	EnhancedHook(Position P, int id) {
		ID = id;

		loadimage(&img_list, _T("./image/prop/enhanced_hook_i.png"));
		loadimage(&mask_list, _T("./image/prop/enhanced_hook_m.png"));
		current_img = &img_list;
		current_mask = &mask_list;

		start_flag = 0;
		delta_x = 4;

		position.x = P.x;
		position.y = P.y;
		target.x = screen_width - 60;
		target.y = screen_height / 2;

		rect.top = position.y - current_img->getheight() / 4;
		rect.right = position.x + current_img->getwidth() / 4;
		rect.bottom = position.y + current_img->getheight() / 4;
		rect.left = position.x - current_img->getwidth() / 4;

		fun_a = 0;
		fun_b = 0;

		//cout << "生成EnhancedHook，ID为" << id << endl;
	}
	~EnhancedHook() {
		//cout << "EnhancedHook" << ID << "被销毁" << endl;
	}
};

class OxygenBottle :public BaseProperty {
public:
	OxygenBottle(Position P) {
		loadimage(&img_list, _T("./image/prop/oxygen_bottle_i.png"));
		loadimage(&mask_list, _T("./image/prop/oxygen_bottle_m.png"));
		current_img = &img_list;
		current_mask = &mask_list;

		start_flag = 0;
		delta_x = 4;

		position.x = P.x;
		position.y = P.y;
		target.x = game_area_width + 40;
		target.y = game_area_height * 3 / 5;

		rect.top = position.y - current_img->getheight() / 2;
		rect.right = position.x + current_img->getwidth() / 2;
		rect.bottom = position.y + current_img->getheight() / 2;
		rect.left = position.x - current_img->getwidth() / 2;

		fun_a = 0;
		fun_b = 0;
	}
	~OxygenBottle() {
		//cout << "OxygenBottle被销毁" << endl;
	}
};

class InvincibleProp :public BaseProperty {
public:
	InvincibleProp(Position P) {
		loadimage(&img_list, _T("./image/prop/invincibleprop_i.png"));
		loadimage(&mask_list, _T("./image/prop/invincibleprop_m.png"));
		current_img = &img_list;
		current_mask = &mask_list;

		start_flag = 0;
		delta_x = 4;

		position.x = P.x;
		position.y = P.y;
		target.x = game_area_width + 40;
		target.y = game_area_height * 3 / 5;

		rect.top = position.y - current_img->getheight() / 2;
		rect.right = position.x + current_img->getwidth() / 2;
		rect.bottom = position.y + current_img->getheight() / 2;
		rect.left = position.x - current_img->getwidth() / 2;

		fun_a = 0;
		fun_b = 0;
	}
	~InvincibleProp() {
		//cout << "InvincibleProp被销毁" << endl;
	}
};

class Stab {
public:
	static IMAGE* img_list;
	static IMAGE* mask_list;
	Stab(Position P, int id);
	void Show();
	void UpdatePosition();
	RECT& GetRect();
	int GetID();
	static void SetImageAndMask();
	static void ReleaseMemory();
	~Stab();
private:
	int ID;
	IMAGE* current_img;
	IMAGE* current_mask;

	RECT rect;
	Position position;

	clock_t timer_time;	//记录图片播放改变的时刻
	int timer_interval;	//图片播放的间隔
};
Stab::Stab(Position P, int id) {
	ID = id;

	current_img = &img_list[0];
	current_mask = &mask_list[0];

	position.x = P.x;
	position.y = P.y;

	rect.top = position.y - current_img->getheight() / 8;
	rect.right = position.x + current_img->getwidth() / 8;
	rect.bottom = position.y + current_img->getheight() / 8;
	rect.left = position.x - current_img->getwidth() / 8;

	timer_time = clock();
	timer_interval = 750;

	//cout << "生成Stab，ID为" << id << endl;
}
void Stab::Show() {
	if ((clock() - timer_time) >= timer_interval) {
		timer_time = clock();
		if (current_img == &img_list[1]) current_img = &img_list[0];
		else current_img = &img_list[1];
		if (current_mask == &mask_list[1]) current_mask = &mask_list[0];
		else current_mask = &mask_list[1];
	}
	putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_mask, SRCAND);
	putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_img, SRCPAINT);
}
void Stab::UpdatePosition() {
	position.y += scroll_speed;

	rect.top = position.y - current_img->getheight() / 8;
	rect.right = position.x + current_img->getwidth() / 8;
	rect.bottom = position.y + current_img->getheight() / 8;
	rect.left = position.x - current_img->getwidth() / 8;
}
RECT& Stab::GetRect() {
	return rect;
}
int Stab::GetID() {
	return ID;
}
IMAGE* Stab::img_list = NULL;
IMAGE* Stab::mask_list = NULL;
void Stab::SetImageAndMask() {
	//current_img 的初始化值是 nullptr，因此在 Brick 类的静态成员函数 SetImageAndMask 中，
	//需要为 img_list 和 mask_list 分配内存并加载图像。否则，在创建 Brick 对象时，current_img 仍然是空指针。
	//IMAGE(2)这个数据在堆区，但是接收它的指针在栈区
	img_list = new IMAGE[2];
	loadimage(&img_list[0], _T("./image/prop/stab_1_i.png"));
	loadimage(&img_list[1], _T("./image/prop/stab_2_i.png"));
	mask_list = new IMAGE[2];
	loadimage(&mask_list[0], _T("./image/prop/stab_1_m.png"));
	loadimage(&mask_list[1], _T("./image/prop/stab_2_m.png"));
}
void Stab::ReleaseMemory() {
	//释放数组必须加上[]
	delete[] img_list;
	delete[] mask_list;
}
Stab::~Stab() {
	//cout << "Stab" << ID << "被销毁" << endl;
}