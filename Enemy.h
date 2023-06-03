#pragma once
#include<iostream>
#include<graphics.h>
#include"GlobalSettings.h"
#include"Hero.h"
using namespace std;

//基类
class BaseEnemy {
public:
	void HurtFlash();
	RECT& GetRect();
	Position& GetPosition();
	bool IfDying();
	bool IfDead();
	virtual void UpdatePosition();
	virtual void Show();
	virtual void AttackedCheck(Hero* hero);
	int GetID();
protected:
	Position position;
	int HP;
	bool is_dead;	//在HP为0且hurt_flash为0时置1
	RECT rect;

	clock_t timer_time;	//记录图片播放改变的时刻
	int timer_interval;	//图片播放的间隔

	bool hurt_flash;	//置1时表示受伤后闪烁，置0时不闪烁
	int hurt_interval;	//受击的间隔或HP归零到销毁的时长 ，单位为ms
	clock_t hurt_time;	//记录受击的时刻
	bool flashing_flag;	//正在闪烁标志位
	int flash_interval;	//受击闪烁的间隔
	int flash_timer_time;	//记录flashing_flag改变的时刻

	int ID;
	
};
void BaseEnemy::HurtFlash() {
	if (hurt_flash == 0) {
		hurt_time = clock();
		flash_timer_time = clock();
		flashing_flag = 0;
	}
	else {
		int hurt_last = clock() - hurt_time;
		if (hurt_last >= hurt_interval) {
			flashing_flag = 0;
			hurt_flash = 0;
		}
		if ((HP == 0) && (hurt_flash == 0)) is_dead = 1;
	}
	if (hurt_flash) {
		if ((clock() - flash_timer_time) >= flash_interval) {
			flashing_flag = !flashing_flag;
			flash_timer_time = clock();
		}
	}
}
RECT& BaseEnemy::GetRect() {
	return rect;
}
Position& BaseEnemy::GetPosition() {
	return position;
}
bool BaseEnemy::IfDying() {
	return !HP && !is_dead;
}
bool BaseEnemy::IfDead() {
	return is_dead;
}
void BaseEnemy::UpdatePosition() {

}
void BaseEnemy::Show() {

}
void BaseEnemy::AttackedCheck(Hero* hero) {
	HurtFlash();
	Hook* hook = &hero->GetHook();
	bool hook_in = (hook->position.x >= rect.left) && (hook->position.x <= rect.right) && (hook->position.y >= rect.top) && (hook->position.y <= rect.bottom);
	if (hook_in && (hero->GetAttackState() == 1) && !hurt_flash) {
		HP = (HP == 0) ? HP : HP - 1;
		hurt_flash = 1;
	}
}
int BaseEnemy::GetID() {
	return ID;
}
//派生类

//海龟
class Turtle :public BaseEnemy {
public:
	Turtle(Position* R, int id);	//R为长度为2的数组指针，存储路线的起点和终点，要求水平方向，起点在左，终点在右
	void UpdatePosition();
	void Show();
	void AttackedCheck(Hero* hero);
	~Turtle();
private:
	IMAGE img_list[4];
	IMAGE mask_list[4];
	IMAGE* current_img;
	IMAGE* current_mask;
	
	Position route[2];	//路线的起点和终点，要求水平方向，起点在左，终点在右
	int speed;
	bool f_or_b;	//1表示从起点前往终点，0表示从终点返回起点
	bool direction_change;	//当移动方向改变时为1
};
Turtle::Turtle(Position* R, int id) {
	ID = id;

	loadimage(&img_list[0], _T("./image/turtle/right_1_i.png"));
	loadimage(&img_list[1], _T("./image/turtle/right_2_i.png"));
	loadimage(&img_list[2], _T("./image/turtle/left_1_i.png"));
	loadimage(&img_list[3], _T("./image/turtle/left_2_i.png"));

	loadimage(&mask_list[0], _T("./image/turtle/right_1_m.png"));
	loadimage(&mask_list[1], _T("./image/turtle/right_2_m.png"));
	loadimage(&mask_list[2], _T("./image/turtle/left_1_m.png"));
	loadimage(&mask_list[3], _T("./image/turtle/left_2_m.png"));

	current_img = &img_list[0];
	current_mask = &mask_list[0];
	HP = 1;

	rect.left = position.x - current_img->getwidth() * 3 / 4;
	rect.right = position.x + current_img->getwidth() * 3 / 4;
	rect.top = position.y - current_img->getheight() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;

	hurt_flash = 0;
	hurt_interval = 2000;
	hurt_time = clock();

	timer_time = clock();
	timer_interval = 750;

	flash_interval = 250;
	flashing_flag = 0;
	flash_timer_time = clock();

	is_dead = 0;

	route[0].x = R[0].x;
	route[0].y = R[0].y;
	route[1].x = R[1].x;
	route[1].y = R[1].y;

	position.x = route[0].x;
	position.y = route[0].y;

	speed = 1;
	f_or_b = 1;
	direction_change = 0;

	cout << "生成Turtle，ID为"<< id << endl;
}
void Turtle::Show() {
	if (f_or_b) {
		if ((clock() - timer_time) >= timer_interval) {
			timer_time = clock();
			if (current_img == &img_list[1]) current_img = &img_list[0];
			else current_img = &img_list[1];
			if (current_mask == &mask_list[1]) current_mask = &mask_list[0];
			else current_mask = &mask_list[1];
		}
		else if (direction_change) {
			current_img = &img_list[0];
			current_mask = &mask_list[0];
		}
	}
	else {
		if ((clock() - timer_time) >= timer_interval) {
			timer_time = clock();
			if (current_img == &img_list[3]) current_img = &img_list[2];
			else current_img = &img_list[3];
			if (current_mask == &mask_list[3]) current_mask = &mask_list[2];
			else current_mask = &mask_list[3];
		}
		else if (direction_change) {
			current_img = &img_list[2];
			current_mask = &mask_list[2];
		}
	}
	if (!flashing_flag) {
		putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_mask, SRCAND);
		putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_img, SRCPAINT);
	}
	else {

	}
}
void Turtle::AttackedCheck(Hero* hero) {
	HurtFlash();
	Hook* hook = &hero->GetHook();
	//检测hook此时是否在turtle内
	bool hook_in = (hook->position.x >= rect.left) && (hook->position.x <= rect.right) && (hook->position.y >= rect.top) && (hook->position.y <= rect.bottom);
	switch (hook->hook_direction)
	{
	case(1): {
		//向上（含左上和右上）的攻击有效
		if (hook_in && (hero->GetAttackState() == 1) && !hurt_flash) {
			HP = (HP == 0) ? HP : HP - 1;
			hurt_flash = 1;
		}
		break;
	}
	case(2): {
		//向下的攻击无效，hook被弹回
		if (hook_in && (hero->GetAttackState() == 1)) hero->SetLineTarget(hero->GetPositionX(), rect.top);	//hero->GetAttackState() == 1确保只设置一次
		break;
	}
	case(3): {
		//向左的攻击无效，hook被弹回
		if (hook_in && (hero->GetAttackState() == 1)) hero->SetLineTarget(rect.right, hero->GetPositionY());
		break;
	}
	case(4): {
		//向右的攻击无效，hook被弹回
		if (hook_in && (hero->GetAttackState() == 1)) hero->SetLineTarget(rect.left, hero->GetPositionY());
		break;
	}
	case(5): {
		//向右上的攻击有效
		if (hook_in && (hero->GetAttackState() == 1) && !hurt_flash) {
			HP = (HP == 0) ? HP : HP - 1;
			hurt_flash = 1;
		}
		break;
	}
	case(6): {
		//向右下的攻击无效，hook被弹回
		if (hook_in && (hero->GetAttackState() == 1)) {
			if ((hook->position.x > rect.left) && (hook->position.y >= rect.top)) {
				hero->SetLineTarget(hero->GetPositionX() + (rect.top - hero->GetPositionY()), rect.top);
			}
			else {
				hero->SetLineTarget(rect.left, hero->GetPositionY() + (rect.left - hero->GetPositionX()));
			}
		}
		break;
	}
	case(7): {
		//向左下的攻击无效，hook被弹回
		if (hook_in && (hero->GetAttackState() == 1)) {
			if ((hook->position.x < rect.right) && (hook->position.y >= rect.top)) {
				hero->SetLineTarget(hero->GetPositionX() - (rect.top - hero->GetPositionY()), rect.top);
			}
			else {
				hero->SetLineTarget(rect.left, hero->GetPositionY() + (rect.left - hero->GetPositionX()));
			}
		}
		break;
	}
	case(8): {
		//向左上的攻击有效
		if (hook_in && (hero->GetAttackState() == 1) && !hurt_flash) {
			HP = (HP == 0) ? HP : HP - 1;
			hurt_flash = 1;
		}
		break;
	}
	default:
		break;
	}
}
void Turtle::UpdatePosition() {
	route[0].y += scroll_speed;
	route[1].y += scroll_speed;
	if (hurt_flash == 0) {
		if (f_or_b) {
			if (position.x + speed >= route[1].x) {
				position.x = route[1].x;
				f_or_b = 0;
				direction_change = 1;
			}
			else {
				position.x += speed;
				direction_change = 0;
			}
		}
		else {
			if (position.x - speed <= route[0].x) {
				position.x = route[0].x;
				f_or_b = 1;
				direction_change = 1;
			}
			else {
				position.x -= speed;
				direction_change = 0;
			}
		}
		position.y = route[0].y;
	}
	else {
		position.y = route[0].y;
	}
	rect.left = position.x - current_img->getwidth() * 3 / 4;
	rect.right = position.x + current_img->getwidth() * 3 / 4;
	rect.top = position.y - current_img->getheight() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;
}
Turtle::~Turtle() {
	cout << "Turtle" << ID << "被销毁" << endl;
}

//水母
class Scaleph :public BaseEnemy {
public:
	Scaleph(Position* R, int id);
	void UpdatePosition();
	void Show();
	void AttackedCheck(Hero* hero);
	~Scaleph();
private:
	IMAGE img_list[2];
	IMAGE mask_list[2];
	IMAGE* current_img;
	IMAGE* current_mask;
	Position route[2];	//路线的起点和终点，要求水平方向，起点在左，终点在右
};
Scaleph::Scaleph(Position* R, int id) {
	loadimage(&img_list[0], _T("./image/scaleph/scaleph_1_i.png"));
	loadimage(&img_list[1], _T("./image/scaleph/scaleph_2_i.png"));

	loadimage(&mask_list[0], _T("./image/scaleph/scaleph_1_m.png"));
	loadimage(&mask_list[1], _T("./image/scaleph/scaleph_2_m.png"));

	current_img = &img_list[0];
	current_mask = &mask_list[0];
	HP = 1;

	rect.left = position.x - current_img->getwidth() / 2;
	rect.right = position.x + current_img->getwidth() / 2;
	rect.top = position.y - current_img->getheight() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;

	hurt_flash = 0;
	hurt_interval = 2000;
	hurt_time = clock();

	timer_time = clock();
	timer_interval = 750;

	flash_interval = 250;
	flashing_flag = 0;
	flash_timer_time = clock();

	is_dead = 0;

	route[0].x = R[0].x;
	route[0].y = R[0].y;
	route[1].x = R[1].x;
	route[1].y = R[1].y;

	position.x = route[0].x;
	position.y = route[0].y;
}
void Scaleph::UpdatePosition() {
	route[0].y += scroll_speed;
	position.y = route[0].y;

	rect.left = position.x - current_img->getwidth() / 2;
	rect.right = position.x + current_img->getwidth() / 2;
	rect.top = position.y - current_img->getheight() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;
}
void Scaleph::Show() {
	if ((clock() - timer_time) >= timer_interval) {
		timer_time = clock();
		if (current_img == &img_list[1]) current_img = &img_list[0];
		else current_img = &img_list[1];
		if (current_mask == &mask_list[1]) current_mask = &mask_list[0];
		else current_mask = &mask_list[1];
	}
		
	if (!flashing_flag) {
		putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_mask, SRCAND);
		putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_img, SRCPAINT);
	}
	else {

	}
}
void Scaleph::AttackedCheck(Hero* hero) {
	HurtFlash();
	Hook* hook = &hero->GetHook();
	//检测hook此时是否在turtle内
	bool hook_in = (hook->position.x >= rect.left) && (hook->position.x <= rect.right) && (hook->position.y >= rect.top) && (hook->position.y <= rect.bottom);
	switch (hook->hook_direction)
	{
	case(1): {
		//向上的攻击无效，hook被弹回
		if (hook_in && (hero->GetAttackState() == 1)) hero->SetLineTarget(hero->GetPositionX(), rect.bottom);	//hero->GetAttackState() == 1确保只设置一次
		break;
	}
	case(2): {
		//向下（含左下和右下）的攻击有效
		if (hook_in && (hero->GetAttackState() == 1) && !hurt_flash) {
			HP = (HP == 0) ? HP : HP - 1;
			hurt_flash = 1;
		}
		break;
	}
	case(3): {
		//向左的攻击无效，hook被弹回
		if (hook_in && (hero->GetAttackState() == 1)) hero->SetLineTarget(rect.right, hero->GetPositionY());
		break;
	}
	case(4): {
		//向右的攻击无效，hook被弹回
		if (hook_in && (hero->GetAttackState() == 1)) hero->SetLineTarget(rect.left, hero->GetPositionY());
		break;
	}
	case(5): {
		//向右上的攻击无效
		if (hook_in && (hero->GetAttackState() == 1)) {
			if ((hook->position.x > rect.left) && (hook->position.y <= rect.bottom)) {
				hero->SetLineTarget(hero->GetPositionX() + (rect.bottom - hero->GetPositionY()), rect.bottom);
			}
			else {
				hero->SetLineTarget(rect.left, hero->GetPositionY() + (rect.left - hero->GetPositionX()));
			}
		}
		break;
	}
	case(6): {
		//向右下的攻击有效
		if (hook_in && (hook->position.y <= rect.top + current_img->getheight() / 5) && (hero->GetAttackState() == 1) && !hurt_flash) {
			HP = (HP == 0) ? HP : HP - 1;
			hurt_flash = 1;
		}
		break;
	}
	case(7): {
		//向左下的攻击有效
		if (hook_in && (hook->position.y <= rect.top + current_img->getheight() / 5) && (hero->GetAttackState() == 1) && !hurt_flash) {
			HP = (HP == 0) ? HP : HP - 1;
			hurt_flash = 1;
		}
		break;
	}
	case(8): {
		//向左上的攻击无效，hook被弹回
		if (hook_in && (hero->GetAttackState() == 1)) {
			if ((hook->position.x < rect.right) && (hook->position.y <= rect.bottom)) {
				hero->SetLineTarget(hero->GetPositionX() + (rect.bottom - hero->GetPositionY()), rect.bottom);
			}
			else {
				hero->SetLineTarget(rect.right, hero->GetPositionY() + (rect.right - hero->GetPositionX()));
			}
		}
		break;
	}
	default:
		break;
	}
}
Scaleph::~Scaleph() {
	cout << "Scaleph被销毁" << endl;
}

//红色的鱼
class RedFish :public BaseEnemy {
public:
	RedFish(Position* R, int id);
	void UpdatePosition();
	void Show();
	void UpdateRoute(Hero* hero);
	void StateControl(Hero* hero);
	void SpeedControl(vector<RECT*> rect_list, vector<Position*> position_list);	//在类内实现碰撞检测，传入待检测的矩形列表
	~RedFish();
private:
	IMAGE img_list[4];
	IMAGE mask_list[4];
	IMAGE* current_img;
	IMAGE* current_mask;

	Position patrol_route[2];	//巡逻路线的起点和终点，要求水平方向，起点在左，终点在右
	Position hunt_route[2];
	Position return_route[2];	//脱离攻击状态后返回巡逻路线时的路线
	RECT attack_range;
	bool patrol_state;
	bool attack_state;	//0表示不攻击，1表示正在攻击
	int speed;
	int v_x;	//x方向的速度
	int v_y;	//y方向的速度
	int pre_v_x;	//记录上一时刻x方向的速度，当x速度的方向发生变化时置1，使图片能够在轮播间隔内立即改变
	
	clock_t pre_in_range_time;	//记录主角上次在攻击范围内的时刻
	int a_to_p_interval;	//从脱离攻击范围到返回巡逻状态的间隔
	bool direction_change;	//当移动方向改变时为1
	bool l_or_r;	//向左移动为0，向右移动为1
 };
RedFish::RedFish(Position* R, int id) {
	loadimage(&img_list[0], _T("./image/redfish/redfish_right_1_i.png"));
	loadimage(&img_list[1], _T("./image/redfish/redfish_right_2_i.png"));
	loadimage(&img_list[2], _T("./image/redfish/redfish_left_1_i.png"));
	loadimage(&img_list[3], _T("./image/redfish/redfish_left_2_i.png"));
	loadimage(&mask_list[0], _T("./image/redfish/redfish_right_1_m.png"));
	loadimage(&mask_list[1], _T("./image/redfish/redfish_right_2_m.png"));
	loadimage(&mask_list[2], _T("./image/redfish/redfish_left_1_m.png"));
	loadimage(&mask_list[3], _T("./image/redfish/redfish_left_2_m.png"));

	current_img = &img_list[0];
	current_mask = &mask_list[0];
	HP = 2;

	rect.left = position.x - current_img->getwidth() / 2;
	rect.right = position.x + current_img->getwidth() / 2;
	rect.top = position.y - current_img->getheight() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;

	attack_range.left = position.x - current_img->getwidth();
	attack_range.right = position.x + current_img->getwidth();
	attack_range.top = position.y - current_img->getheight() * 3;
	attack_range.bottom = position.y + current_img->getheight() * 3;

	hurt_flash = 0;
	hurt_interval = 2000;
	hurt_time = clock();

	timer_time = clock();
	timer_interval = 750;

	flash_interval = 250;
	flashing_flag = 0;
	flash_timer_time = clock();

	is_dead = 0;
	direction_change = 0;
	speed = 2;
	pre_in_range_time = clock();
	a_to_p_interval = 4000;
	attack_state = 0;
	patrol_state = 1;
	l_or_r = 1;
	v_x = speed;
	v_y = 0;
	pre_v_x = v_x;

	patrol_route[0].x = R[0].x;
	patrol_route[0].y = R[0].y;
	patrol_route[1].x = R[1].x;
	patrol_route[1].y = R[1].y;

	hunt_route[0].x = R[0].x;
	hunt_route[0].y = R[0].y;
	hunt_route[1].x = R[1].x;
	hunt_route[1].y = R[1].y;

	return_route[0].x = R[1].x;
	return_route[0].y = R[1].y;
	return_route[1].x = R[1].x;
	return_route[1].y = R[1].y;

	position.x = patrol_route[0].x;
	position.y = patrol_route[0].y;
}
void RedFish::Show() {
	if (l_or_r) {
		if ((clock() - timer_time) >= timer_interval) {
			timer_time = clock();
			if (current_img == &img_list[1]) current_img = &img_list[0];
			else current_img = &img_list[1];
			if (current_mask == &mask_list[1]) current_mask = &mask_list[0];
			else current_mask = &mask_list[1];
		}
		else if (direction_change) {
			current_img = &img_list[0];
			current_mask = &mask_list[0];
		}
	}
	else {
		if ((clock() - timer_time) >= timer_interval) {
			timer_time = clock();
			if (current_img == &img_list[3]) current_img = &img_list[2];
			else current_img = &img_list[3];
			if (current_mask == &mask_list[3]) current_mask = &mask_list[2];
			else current_mask = &mask_list[3];
		}
		else if (direction_change) {
			current_img = &img_list[2];
			current_mask = &mask_list[2];
		}
	}
	if (!flashing_flag) {
		putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_mask, SRCAND);
		putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_img, SRCPAINT);
	}
	else {

	}
}
void RedFish::UpdatePosition() {
	if (hurt_flash == 0) {
		position.x += v_x;
		position.y += v_y;
	}
	else {
		position.y += scroll_speed;
	}
	rect.left = position.x - current_img->getwidth() / 2;
	rect.right = position.x + current_img->getwidth() / 2;
	rect.top = position.y - current_img->getheight() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;

	attack_range.left = position.x - 2 * current_img->getwidth();
	attack_range.right = position.x + 2 * current_img->getwidth();
	attack_range.top = position.y - 3 * current_img->getheight();
	attack_range.bottom = position.y + 3 * current_img->getheight();
}
void RedFish::UpdateRoute(Hero* hero) {
	patrol_route[0].y += scroll_speed;
	patrol_route[1].y += scroll_speed;

	if (attack_state) {
		hunt_route[0].x = position.x;
		hunt_route[0].y = position.y;
		hunt_route[1].x = hero->GetPositionX();
		hunt_route[1].y = hero->GetPositionY();
	}

	if (!patrol_state && !attack_state) {
		return_route[0].x = position.x;
		return_route[0].y = position.y;
		return_route[1].x = patrol_route[0].x;
		return_route[1].y = patrol_route[0].y;
	}
}
void RedFish::StateControl(Hero* hero) {
	if (RectCrush(attack_range, hero->GetRect())) {
		patrol_state = 0;
		attack_state = 1;
		pre_in_range_time = clock();
	}
	else {
		if (clock() - pre_in_range_time >= a_to_p_interval) {
			attack_state = 0;
		}
	}
	//由于return_route[0]的更新可能落后于position，需要在到达patrol_route[0]后立即退出返回状态，不能用return_route[0]==patrol_route[0]
	if (!attack_state && (position.x == patrol_route[0].x) && (position.y == patrol_route[0].y)) {
		patrol_state = 1;
		return_route[0].x = patrol_route[1].x;
		return_route[0].y = patrol_route[1].y;
	}
}
void RedFish::SpeedControl(vector<RECT*> rect_list, vector<Position*> position_list) {
	//巡逻
	if (patrol_state && !attack_state) {
		speed = 1;
		if (hurt_flash == 0) {
			if (l_or_r) {
				if (position.x + v_x > patrol_route[1].x) {
					v_x = patrol_route[1].x - position.x;
				}
				else if (position.x >= patrol_route[1].x) {
					v_x = -speed;
					l_or_r = 0;
					direction_change = 1;
				}
				else {
					v_x = speed;
					direction_change = 0;
				}
			}
			else {
				if (position.x + v_x < patrol_route[0].x) {
					v_x = patrol_route[0].x - position.x;
				}
				else if (position.x <= patrol_route[0].x) {
					v_x = speed;
					l_or_r = 1;
					direction_change = 1;
				}
				else {
					v_x = -speed;
					direction_change = 0;
				}
			}
			v_y = scroll_speed;
		}
		else {

		}
	}
	//追捕
	else if (!patrol_state && attack_state) {
		speed = 3;
		if (hypot(hunt_route[1].x - hunt_route[0].x, hunt_route[1].y - hunt_route[0].y) == 0) {
			v_x = 0;
			v_y = 0;
		}
		else {
			float v_sin_theta = (hunt_route[1].y - hunt_route[0].y) / hypot(hunt_route[1].x - hunt_route[0].x, hunt_route[1].y - hunt_route[0].y);
			float v_cos_theta = (hunt_route[1].x - hunt_route[0].x) / hypot(hunt_route[1].x - hunt_route[0].x, hunt_route[1].y - hunt_route[0].y);
			//由于坐标和速度都是整数，为了防止由类型转换后精度丢失导致无法到达目标位置的问题，要确保在到达目标位置前该方向的速度不能为0。
			// 这样会使合速度不等于speed
			v_x = speed * v_cos_theta;
			v_y = speed * v_sin_theta;
			if ((hunt_route[0].x != hunt_route[1].x) && (v_x == 0)) {
				if (v_cos_theta > 0)v_x = 1;
				else if (v_cos_theta < 0)v_x = -1;
				else v_x = 0;
			}
			if ((hunt_route[0].y != hunt_route[1].y) && (v_y == 0)) {
				if (v_sin_theta > 0)v_y = 1;
				else if (v_sin_theta < 0)v_y = -1;
				else v_y = 0;
			}
			if (position.y >= hunt_route[1].y - speed && position.y <= hunt_route[1].y + speed
				&& position.x >= hunt_route[1].x - speed && position.x <= hunt_route[1].x + speed) {
				v_x = hunt_route[1].x - position.x;
				v_y = hunt_route[1].y - position.y;
			}
			//碰撞检测，不能跟上面的代码调换顺序，否则v_x和v_y的碰撞检测后的结果会被覆盖
			auto rect_list_it = rect_list.begin();
			auto position_list_it = position_list.begin();
			while ((rect_list_it != rect_list.end()) && (position_list_it != position_list.end())) {
				if (RectCrush(rect, *(*rect_list_it))) {
					float sin_theta = ((*position_list_it)->y - position.y) / hypot(position.x - (*position_list_it)->x, position.y - (*position_list_it)->y);
					float cos_theta = (position.x - (*position_list_it)->x) / hypot(position.x - (*position_list_it)->x, position.y - (*position_list_it)->y);
					float theta1 = ((*(*rect_list_it)).bottom - (*(*rect_list_it)).top) / 2 / hypot(((*(*rect_list_it)).right - (*(*rect_list_it)).left) / 2, ((*(*rect_list_it)).bottom - (*(*rect_list_it)).top) / 2);
					float theta2 = ((*(*rect_list_it)).right - (*(*rect_list_it)).left) / 2 / hypot(((*(*rect_list_it)).right - (*(*rect_list_it)).left) / 2, ((*(*rect_list_it)).bottom - (*(*rect_list_it)).top) / 2);

					//1：向上反弹；2：向右反弹；3：向下反弹；4向左反弹
					if (sin_theta >= theta1) {
						v_y = (v_y > 0) ? 0 : v_y;	//y方向的速度置零
						cout << "向上反弹" << endl;
					}
					else if ((sin_theta < theta1) && (cos_theta >= theta2)) {
						v_x = (v_x < 0) ? 0 : v_x;
						cout << "向右反弹" << endl;
					}
					else if (sin_theta <= -theta1) {
						v_y = (v_y < 0) ? 0 : v_y;
						cout << "向下反弹" << endl;
					}
					else if ((sin_theta < theta1) && (cos_theta <= -theta2)) {
						v_x = (v_x > 0) ? 0 : v_x;
						cout << "向左反弹" << endl;
					}
					else {
						cout << "未知方向碰撞" << endl;
					}
				}
				else {

				}
				rect_list_it++;
				position_list_it++;
			}
		}
		//l_or_r的控制
		if (v_x > 0)l_or_r = 1;
		else if (v_x < 0)l_or_r = 0;
		else l_or_r = l_or_r;

		//direction_change的控制
		if (!((v_x < 0 && pre_v_x < 0) || (v_x > 0 && pre_v_x > 0) || (v_x == 0 && pre_v_x == 0)))direction_change = 1;
		else direction_change = 0;

		pre_v_x = v_x;
	}
	//返回
	else if (!patrol_state && !attack_state) {
		speed = 1;
		float v_sin_theta = (return_route[1].y - return_route[0].y) / hypot(return_route[1].x - return_route[0].x, return_route[1].y - return_route[0].y);
		float v_cos_theta = (return_route[1].x - return_route[0].x) / hypot(return_route[1].x - return_route[0].x, return_route[1].y - return_route[0].y);
		//由于坐标和速度都是整数，为了防止由类型转换后精度丢失导致无法到达目标位置的问题，要确保在到达目标位置前该方向的速度不能为0。
		// 这样会使合速度不等于speed，而且要注意除数不能为0，需要在到达patrol_route[0]后立即退出返回状态
		v_x = speed * v_cos_theta;
		v_y = speed * v_sin_theta;
		if ((return_route[0].x != return_route[1].x) && (v_x == 0)) {
			if (v_cos_theta > 0)v_x = 1;
			else if (v_cos_theta < 0)v_x = -1;
			else v_x = 0;
		}
		if ((return_route[0].y != return_route[1].y) && (v_y == 0)) {
			if (v_sin_theta > 0)v_y = 1;
			else if (v_sin_theta < 0)v_y = -1;
			else v_y = 0;
		}
		if (position.y >= return_route[1].y - speed && position.y <= return_route[1].y + speed
			&& position.x >= return_route[1].x - speed && position.x <= return_route[1].x + speed) {
			v_x = return_route[1].x - position.x;
			v_y = return_route[1].y - position.y;
		}
		//碰撞检测，不能跟上面的代码调换顺序，否则v_x和v_y的碰撞检测后的结果会被覆盖
		auto rect_list_it = rect_list.begin();
		auto position_list_it = position_list.begin();
		while (rect_list_it != rect_list.end()) {
			if (RectCrush(rect, *(*rect_list_it))) {
				//计算RedFish到待检测物体的位矢辐角
				float sin_theta = ((*position_list_it)->y - position.y) / hypot(position.x - (*position_list_it)->x, position.y - (*position_list_it)->y);
				float cos_theta = (position.x - (*position_list_it)->x) / hypot(position.x - (*position_list_it)->x, position.y - (*position_list_it)->y);
				float theta1 = ((*(*rect_list_it)).bottom - (*(*rect_list_it)).top) / 2 / hypot(((*(*rect_list_it)).right - (*(*rect_list_it)).left) / 2, ((*(*rect_list_it)).bottom - (*(*rect_list_it)).top) / 2);
				float theta2 = ((*(*rect_list_it)).right - (*(*rect_list_it)).left) / 2 / hypot(((*(*rect_list_it)).right - (*(*rect_list_it)).left) / 2, ((*(*rect_list_it)).bottom - (*(*rect_list_it)).top) / 2);

				//1：向上反弹；2：向右反弹；3：向下反弹；4向左反弹
				if (sin_theta >= theta1) {
					v_y = (v_y > 0) ? 0 : v_y;	//y方向的速度置零
					cout << "向上反弹" << endl;
				}
				else if ((sin_theta < theta1) && (cos_theta >= theta2)) {
					v_x = (v_x < 0) ? 0 : v_x;
					cout << "向右反弹" << endl;
				}
				else if (sin_theta <= -theta1) {
					v_y = (v_y < 0) ? 0 : v_y;
					cout << "向下反弹" << endl;
				}
				else if ((sin_theta < theta1) && (cos_theta <= -theta2)) {
					v_x = (v_x > 0) ? 0 : v_x;
					cout << "向左反弹" << endl;
				}
				else {
					cout << "未知方向碰撞" << endl;
				}
			}
			else {

			}
			rect_list_it++;
			position_list_it++;
		}
		v_y += scroll_speed;
		//l_or_r的控制
		if (v_x > 0)l_or_r = 1;
		else if (v_x < 0)l_or_r = 0;
		else l_or_r = l_or_r;

		//direction_change的控制
		if (!((v_x < 0 && pre_v_x < 0) || (v_x > 0 && pre_v_x > 0) || (v_x == 0 && pre_v_x == 0)))direction_change = 1;
		else direction_change = 0;

		pre_v_x = v_x;
	}
	else {
		cout << "未知状态" << endl;
	}
}
RedFish::~RedFish() {
	cout << "RedFish被销毁" << endl;
}

//绿色的鱼
class GreenFish :public BaseEnemy {
public:
	GreenFish(Position* R, int id);
	void UpdatePosition();
	void Show();
	void UpdateRoute();
	void SpeedControl();
	void AttackedCheck(Hero* hero);
	~GreenFish();
private:
	IMAGE img_list[4];
	IMAGE mask_list[4];
	IMAGE* current_img;
	IMAGE* current_mask;

	Position route[2];	//路线的起点和终点，要求水平方向，起点在左，终点在右

	bool direction_change;	//当移动方向改变时为1
	bool l_or_r;	//向左移动为0，向右移动为1
	int v_x;	//x方向的速度
	int v_y;	//y方向的速度
	int pre_v_x;	//记录上一时刻x方向的速度，当x速度的方向发生变化时置1，使图片能够在轮播间隔内立即改变
};
GreenFish::GreenFish(Position* R, int id) {
	loadimage(&img_list[0], _T("./image/greenfish/greenfish_right_1_i.png"));
	loadimage(&img_list[1], _T("./image/greenfish/greenfish_right_2_i.png"));
	loadimage(&img_list[2], _T("./image/greenfish/greenfish_left_1_i.png"));
	loadimage(&img_list[3], _T("./image/greenfish/greenfish_left_2_i.png"));
	loadimage(&mask_list[0], _T("./image/greenfish/greenfish_right_1_m.png"));
	loadimage(&mask_list[1], _T("./image/greenfish/greenfish_right_2_m.png"));
	loadimage(&mask_list[2], _T("./image/greenfish/greenfish_left_1_m.png"));
	loadimage(&mask_list[3], _T("./image/greenfish/greenfish_left_2_m.png"));

	current_img = &img_list[0];
	current_mask = &mask_list[0];
	HP = 1;

	rect.left = position.x - current_img->getwidth() / 2;
	rect.right = position.x + current_img->getwidth() / 2;
	rect.top = position.y - current_img->getheight() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;

	hurt_flash = 0;
	hurt_interval = 2000;
	hurt_time = clock();

	timer_time = clock();
	timer_interval = 750;

	flash_interval = 250;
	flashing_flag = 0;
	flash_timer_time = clock();

	is_dead = 0;
	direction_change = 0;

	l_or_r = 1;
	v_x = 1;
	v_y = 0;
	pre_v_x = v_x;

	route[0].x = R[0].x;
	route[0].y = R[0].y;
	route[1].x = R[1].x;
	route[1].y = R[1].y;

	position.x = route[0].x;
	position.y = route[0].y;
}
void GreenFish::Show() {
	if (l_or_r) {
		if ((clock() - timer_time) >= timer_interval) {
			timer_time = clock();
			if (current_img == &img_list[1]) current_img = &img_list[0];
			else current_img = &img_list[1];
			if (current_mask == &mask_list[1]) current_mask = &mask_list[0];
			else current_mask = &mask_list[1];
		}
		else if (direction_change) {
			current_img = &img_list[0];
			current_mask = &mask_list[0];
		}
	}
	else {
		if ((clock() - timer_time) >= timer_interval) {
			timer_time = clock();
			if (current_img == &img_list[3]) current_img = &img_list[2];
			else current_img = &img_list[3];
			if (current_mask == &mask_list[3]) current_mask = &mask_list[2];
			else current_mask = &mask_list[3];
		}
		else if (direction_change) {
			current_img = &img_list[2];
			current_mask = &mask_list[2];
		}
	}
	if (!flashing_flag) {
		putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_mask, SRCAND);
		putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_img, SRCPAINT);
	}
	else {

	}
}
void GreenFish::UpdatePosition() {
	if (hurt_flash == 0) {
		position.x += v_x;
		position.y += v_y;
	}
	else {
		position.y += scroll_speed;
	}
	rect.left = position.x - current_img->getwidth() / 2;
	rect.right = position.x + current_img->getwidth() / 2;
	rect.top = position.y - current_img->getheight() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;
}
void GreenFish::UpdateRoute() {
	route[0].y += scroll_speed;
	route[1].y += scroll_speed;
}
void GreenFish::SpeedControl() {
	int speed = 1;
	if (hurt_flash == 0) {
		if (l_or_r) {
			if (position.x + v_x > route[1].x) {
				v_x = route[1].x - position.x;
			}
			else if (position.x >= route[1].x) {
				v_x = -speed;
				l_or_r = 0;
				direction_change = 1;
			}
			else {
				v_x = speed;
				direction_change = 0;
			}
		}
		else {
			if (position.x + v_x < route[0].x) {
				v_x = route[0].x - position.x;
			}
			else if (position.x <= route[0].x) {
				v_x = speed;
				l_or_r = 1;
				direction_change = 1;
			}
			else {
				v_x = -speed;
				direction_change = 0;
			}
		}
		v_y = scroll_speed;
	}
	else {

	}
}
void GreenFish::AttackedCheck(Hero* hero) {
	HurtFlash();
	Hook* hook = &hero->GetHook();
	bool hook_in = (hook->position.x >= rect.left) && (hook->position.x <= rect.right) && (hook->position.y >= rect.top) && (hook->position.y <= rect.bottom);
	if (hook_in && (hero->GetAttackState() == 1) && !hurt_flash) {
		HP = (HP == 0) ? HP : HP - 1;
		hurt_flash = 1;
	}
}
GreenFish::~GreenFish() {
	cout << "GreenFish被销毁" << endl;
}

//灯笼鱼
class LanternFish :public BaseEnemy{
public:
	LanternFish(Position P, int orient, int id);
	void UpdatePosition();
	void Show();
	void StateControl(Hero* hero);
	void UpdateRoute(Hero* hero);
	void SpeedControl();
	~LanternFish();
private:
	IMAGE img_list[4];
	IMAGE mask_list[4];
	IMAGE* current_img;
	IMAGE* current_mask;

	bool direction_change;	//当移动方向改变时为1
	bool l_or_r;	//向左移动为0，向右移动为1
	int v_x;	//x方向的速度
	int v_y;	//y方向的速度
	int pre_v_x;	//记录上一时刻x方向的速度，当x速度的方向发生变化时置1，使图片能够在轮播间隔内立即改变
	int orientation;	//-1朝左，1朝右

	RECT attack_range;
	int attack_state;	//0表示不攻击，1表示正在攻击，2表示返回状态
	int residence_period;	//到达目标位置后在等待状态停留的时间
	Position hunt_route[2];
	Position return_route[2];	//脱离攻击状态后返回巡逻路线时的路线
	int attack_speed;
	int return_speed;
	clock_t arrival_time;	//到达攻击目标的时刻
};
LanternFish::LanternFish(Position P, int orient, int id) {
	if (orient < 0) {
		loadimage(&img_list[0], _T("./image/lanternfish/lanternfish_left_1_i.png"));
		loadimage(&img_list[1], _T("./image/lanternfish/lanternfish_left_2_i.png"));
		loadimage(&img_list[2], _T("./image/lanternfish/lanternfish_right_1_i.png"));
		loadimage(&img_list[3], _T("./image/lanternfish/lanternfish_right_2_i.png"));
		loadimage(&mask_list[0], _T("./image/lanternfish/lanternfish_left_1_m.png"));
		loadimage(&mask_list[1], _T("./image/lanternfish/lanternfish_left_2_m.png"));
		loadimage(&mask_list[2], _T("./image/lanternfish/lanternfish_right_1_m.png"));
		loadimage(&mask_list[3], _T("./image/lanternfish/lanternfish_right_2_m.png"));
	}
	else {
		loadimage(&img_list[0], _T("./image/lanternfish/lanternfish_right_1_i.png"));
		loadimage(&img_list[1], _T("./image/lanternfish/lanternfish_right_2_i.png"));
		loadimage(&img_list[2], _T("./image/lanternfish/lanternfish_left_1_i.png"));
		loadimage(&img_list[3], _T("./image/lanternfish/lanternfish_left_2_i.png"));
		loadimage(&mask_list[0], _T("./image/lanternfish/lanternfish_right_1_m.png"));
		loadimage(&mask_list[1], _T("./image/lanternfish/lanternfish_right_2_m.png"));
		loadimage(&mask_list[2], _T("./image/lanternfish/lanternfish_left_1_m.png"));
		loadimage(&mask_list[3], _T("./image/lanternfish/lanternfish_left_2_m.png"));
	}
	orientation = (orient < 0) ? -1 : 1;

	current_img = &img_list[0];
	current_mask = &mask_list[0];
	HP = 1;

	position.x = P.x;
	position.y = P.y;

	rect.left = position.x - current_img->getwidth() / 2;
	rect.right = position.x + current_img->getwidth() / 2;
	rect.top = position.y - current_img->getheight() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;

	hurt_flash = 0;
	hurt_interval = 2000;
	hurt_time = clock();

	timer_time = clock();
	timer_interval = 750;

	flash_interval = 250;
	flashing_flag = 0;
	flash_timer_time = clock();

	is_dead = 0;
	direction_change = 0;

	l_or_r = 1;
	v_x = 1 * orientation;
	v_y = 0;
	pre_v_x = v_x;

	attack_range.top = position.y - current_img->getheight() / 2;
	attack_range.bottom = position.y + current_img->getheight() / 2;
	if (orientation == -1) {
		attack_range.right = position.x + current_img->getwidth() /2;
		attack_range.left = position.x - current_img->getwidth() * 4;
	}
	else {
		attack_range.right = position.x + current_img->getwidth() * 4;
		attack_range.left = position.x - current_img->getwidth() / 2;
	}

	attack_state = 0;
	attack_speed = 10 * orientation;
	return_speed = -1 * orientation;
	residence_period = 3000;

	hunt_route[0].x = position.x;
	hunt_route[0].y = position.y;
	hunt_route[1].y = position.y;
	if (orientation == -1) {
		hunt_route[1].x = attack_range.left;
	}
	else {
		hunt_route[1].x = attack_range.right;
	}

	return_route[0].x = hunt_route[1].x;
	return_route[0].y = hunt_route[1].y;
	return_route[1].x = position.x;
	return_route[1].y = position.y;

	arrival_time = clock();
}
void LanternFish::UpdatePosition() {
	if (hurt_flash == 0) {
		position.x += v_x;
		position.y += v_y;
	}
	else {
		position.y += scroll_speed;
	}
	rect.left = position.x - current_img->getwidth() / 2;
	rect.right = position.x + current_img->getwidth() / 2;
	rect.top = position.y - current_img->getheight() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;

	attack_range.top = position.y - current_img->getheight() / 2;
	attack_range.bottom = position.y + current_img->getheight() / 2;
	if (orientation == -1) {
		attack_range.right = position.x + current_img->getwidth() / 2;
		attack_range.left = position.x - current_img->getwidth() * 4;
	}
	else {
		attack_range.right = position.x + current_img->getwidth() * 4;
		attack_range.left = position.x - current_img->getwidth() / 2;
	}
}
void LanternFish::Show() {
	if (l_or_r) {
		if ((clock() - timer_time) >= timer_interval) {
			timer_time = clock();
			if (current_img == &img_list[1]) current_img = &img_list[0];
			else current_img = &img_list[1];
			if (current_mask == &mask_list[1]) current_mask = &mask_list[0];
			else current_mask = &mask_list[1];
		}
		else if (direction_change) {
			current_img = &img_list[0];
			current_mask = &mask_list[0];
		}
	}
	else {
		if ((clock() - timer_time) >= timer_interval) {
			timer_time = clock();
			if (current_img == &img_list[3]) current_img = &img_list[2];
			else current_img = &img_list[3];
			if (current_mask == &mask_list[3]) current_mask = &mask_list[2];
			else current_mask = &mask_list[3];
		}
		else if (direction_change) {
			current_img = &img_list[2];
			current_mask = &mask_list[2];
		}
	}
	if (!flashing_flag) {
		putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_mask, SRCAND);
		putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_img, SRCPAINT);
	}
	else {

	}
}
void LanternFish::StateControl(Hero* hero) {
	if ((attack_state == 0) && (RectCrush(attack_range, hero->GetRect()) == 1)) {
		attack_state = 1;
	}
	else if ((attack_state == 1) && 
		((orientation == 1 && position.x >= hunt_route[1].x) ||
			(orientation == -1 && position.x <= hunt_route[1].x))) {
		arrival_time = clock();
		attack_state = 2;
	}
	else if (attack_state == 2) {
		if (clock() - arrival_time >= residence_period) {
			attack_state = 3;
		}
	}
	else if ((attack_state == 3) &&
		((orientation == 1 && position.x <= return_route[1].x) ||
			(orientation == -1 && position.x <= return_route[1].x))) {
		attack_state = 0;
	}
}
void LanternFish::UpdateRoute(Hero* hero) {
	if (attack_state == 0) {
		hunt_route[1].x = hero->GetPositionX();
	}
	hunt_route[0].x = position.x;
	hunt_route[0].y = position.y;
	hunt_route[1].y += scroll_speed;

	return_route[0].x = position.x;
	return_route[0].y = position.y;
	return_route[1].y += scroll_speed;
}
void LanternFish::SpeedControl() {
	switch (attack_state)
	{
	case(0): {
		l_or_r = (orientation < 0) ? 0 : 1;
		v_x = 0;
		break;
	}
	case(1): {
		if (orientation == -1) {
			l_or_r = 0;
			if (hurt_flash == 0) {
				if (position.x + v_x < hunt_route[1].x) {
					v_x = hunt_route[1].x - position.x;
				}
				else if (position.x <= hunt_route[1].x) {
					v_x = 0;
				}
				else {
					v_x = attack_speed * orientation;
					direction_change = 0;
				}
				v_y = scroll_speed;
			}
			else {

			}
		}
		else {
			l_or_r = 1;
			if (hurt_flash == 0) {
				if (position.x + v_x > hunt_route[1].x) {
					v_x = hunt_route[1].x - position.x;
				}
				else if (position.x >= hunt_route[1].x) {
					v_x = 0;
				}
				else {
					v_x = attack_speed * orientation;
					direction_change = 0;
				}
			}
			else {

			}
		}
		break;
	}
	case(2): {
		v_x = 0;
		break;
	}
	case(3): {
		if (orientation == -1) {
			if (hurt_flash == 0) {
				if (position.x + v_x > return_route[1].x) {
					v_x = return_route[1].x - position.x;
				}
				else if (position.x >= return_route[1].x) {
					v_x = 0;
					l_or_r = 0;
					direction_change = 1;
				}
				else if ((position.x < return_route[1].x) && (v_x == 0)) {
					v_x = return_speed * orientation;
					l_or_r = 1;
					direction_change = 1;	//在停留阶段结束后立即转向
				}
				else {
					v_x = return_speed * orientation;
					direction_change = 0;
					l_or_r = 1;
				}
			}
			else {

			}
		}
		else {
			if (hurt_flash == 0) {
				if (position.x + v_x < return_route[1].x) {
					v_x = return_route[1].x - position.x;
				}
				else if (position.x <= return_route[1].x) {
					v_x = 0;
					l_or_r = 1;
					direction_change = 1;
				}
				else if ((position.x > return_route[1].x) && (v_x == 0)) {
					v_x = return_speed * orientation;
					l_or_r = 0;
					direction_change = 1;	//在停留阶段结束后立即转向
				}
				else {
					v_x = return_speed * orientation;
					direction_change = 0;
					l_or_r = 0;
				}
			}
			else {

			}
		}
		break;
	}
	default: {
		cout << "未知状态" << endl;
		break;
	}
	}
	v_y = scroll_speed;
}
LanternFish::~LanternFish() {
	cout << "LanternFish被销毁" << endl;
}

//龙虾
class Lobster :public BaseEnemy {
public:
	Lobster(Position P, int id);
	void UpdatePosition();
	void Show();
	void StateControl(Hero* hero);
	void UpdateRoute(Hero* hero);
	void SpeedControl();
	~Lobster();
private:
	IMAGE img_list[2];
	IMAGE mask_list[2];
	IMAGE* current_img;
	IMAGE* current_mask;

	int v_y;	//y方向的速度

	RECT attack_range;
	int attack_state;	//0表示不攻击，1表示正在攻击，2表示返回状态
	int residence_period;	//到达目标位置后在等待状态停留的时间
	Position hunt_route[2];
	Position return_route[2];	//脱离攻击状态后返回巡逻路线时的路线
	int attack_speed;
	int return_speed;
	clock_t arrival_time;	//到达攻击目标的时刻
};
Lobster::Lobster(Position P, int id) {
	loadimage(&img_list[0], _T("./image/lobster/lobster1_i.png"));
	loadimage(&img_list[1], _T("./image/lobster/lobster2_i.png"));
	loadimage(&mask_list[0], _T("./image/lobster/lobster1_m.png"));
	loadimage(&mask_list[1], _T("./image/lobster/lobster2_m.png"));

	current_img = &img_list[0];
	current_mask = &mask_list[0];
	HP = 1;

	position.x = P.x;
	position.y = P.y;

	rect.left = position.x - current_img->getwidth() / 2;
	rect.right = position.x + current_img->getwidth() / 2;
	rect.top = position.y - current_img->getheight() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;

	hurt_flash = 0;
	hurt_interval = 2000;
	hurt_time = clock();

	timer_time = clock();
	timer_interval = 750;

	flash_interval = 250;
	flashing_flag = 0;
	flash_timer_time = clock();

	is_dead = 0;

	v_y = 0;

	attack_range.top = position.y - current_img->getheight() / 2;
	attack_range.bottom = position.y + current_img->getheight() * 4;
	attack_range.right = position.x + current_img->getwidth() / 2;
	attack_range.left = position.x - current_img->getwidth() / 2;

	attack_state = 0;
	attack_speed = 10;
	return_speed = -5;
	residence_period = 2000;

	hunt_route[0].x = position.x;
	hunt_route[0].y = position.y;
	hunt_route[1].y = attack_range.bottom;
	hunt_route[1].x = position.x;

	return_route[0].x = hunt_route[1].x;
	return_route[0].y = hunt_route[1].y;
	return_route[1].x = position.x;
	return_route[1].y = position.y;

	arrival_time = clock();
}
void Lobster::UpdatePosition() {
	if (hurt_flash == 0) {
		position.y += v_y;
	}
	else {
		position.y += scroll_speed;
	}
	rect.left = position.x - current_img->getwidth() / 2;
	rect.right = position.x + current_img->getwidth() / 2;
	rect.top = position.y - current_img->getheight() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;

	attack_range.top = position.y - current_img->getheight() / 2;
	attack_range.bottom = position.y + current_img->getheight() * 4;
	attack_range.right = position.x + current_img->getwidth() / 2;
	attack_range.left = position.x - current_img->getwidth() / 2;
}
void Lobster::Show() {
	if ((clock() - timer_time) >= timer_interval) {
		timer_time = clock();
		if (current_img == &img_list[1]) current_img = &img_list[0];
		else current_img = &img_list[1];
		if (current_mask == &mask_list[1]) current_mask = &mask_list[0];
		else current_mask = &mask_list[1];
	}

	if (!flashing_flag) {
		putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_mask, SRCAND);
		putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_img, SRCPAINT);
	}
	else {

	}
}
void Lobster::StateControl(Hero* hero) {
	if ((attack_state == 0) && (RectCrush(attack_range, hero->GetRect()) == 1)) {
		attack_state = 1;
	}
	else if ((attack_state == 1) && (position.y >= hunt_route[1].y)) {
		arrival_time = clock();
		attack_state = 2;
	}
	else if (attack_state == 2) {
		if (clock() - arrival_time >= residence_period) {
			attack_state = 3;
		}
	}
	else if ((attack_state == 3) && (position.y <= return_route[1].y)) {
		attack_state = 0;
	}
}
void Lobster::UpdateRoute(Hero* hero) {
	if (attack_state == 0) {
		hunt_route[1].y = hero->GetPositionY();
	}
	hunt_route[0].x = position.x;
	hunt_route[0].y = position.y;

	return_route[0].x = position.x;
	return_route[0].y = position.y;
	return_route[1].y += scroll_speed;
}
void Lobster::SpeedControl() {
	switch (attack_state)
	{
	case(0): {
		v_y = scroll_speed;
		break;
	}
	case(1): {
		if (hurt_flash == 0) {
			if (position.y + v_y > hunt_route[1].y) {
				v_y = hunt_route[1].y - position.y;
			}
			else {
				v_y = attack_speed;
			}
		}
		else {

		}
		break;
	}
	case(2): {
		v_y = scroll_speed;
		break;
	}
	case(3): {
		if (hurt_flash == 0) {
			if (position.y + v_y < return_route[1].y) {
				v_y = return_route[1].y - position.y;
			}
			else if (position.y <= return_route[1].y) {
				v_y = scroll_speed;
			}
			else {
				v_y = return_speed;
			}
		}
		else {

		}
		break;
	}
	default: {
		cout << "未知状态" << endl;
		break;
	}
	}
}
Lobster::~Lobster() {
	cout << "Lobster被销毁" << endl;
}

//带鱼
class Trichiurus :public BaseEnemy {
public:
	Trichiurus(Position P, int id);
	void UpdatePosition();
	void Show();
	void StateControl(Hero* hero);
	void UpdateRoute(Hero* hero);
	void SpeedControl();
	~Trichiurus();
private:
	IMAGE img_list[2];
	IMAGE mask_list[2];
	IMAGE* current_img;
	IMAGE* current_mask;

	int v_y;	//y方向的速度

	RECT attack_range;
	int attack_state;	//0表示不攻击，1表示正在攻击，2表示返回状态
	int residence_period;	//到达目标位置后在等待状态停留的时间
	Position hunt_route[2];
	Position return_route[2];	//脱离攻击状态后返回巡逻路线时的路线
	int attack_speed;
	int return_speed;
	clock_t arrival_time;	//到达攻击目标的时刻
};
Trichiurus::Trichiurus(Position P, int id) {
	loadimage(&img_list[0], _T("./image/Trichiurus/Trichiurus_1_i.png"));
	loadimage(&img_list[1], _T("./image/Trichiurus/Trichiurus_2_i.png"));
	loadimage(&mask_list[0], _T("./image/Trichiurus/Trichiurus_1_m.png"));
	loadimage(&mask_list[1], _T("./image/Trichiurus/Trichiurus_2_m.png"));

	current_img = &img_list[0];
	current_mask = &mask_list[0];
	HP = 1;

	position.x = P.x;
	position.y = P.y;

	rect.left = position.x - current_img->getwidth() / 4;
	rect.right = position.x + current_img->getwidth() / 4;
	rect.top = position.y - current_img->getheight() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;

	hurt_flash = 0;
	hurt_interval = 2000;
	hurt_time = clock();

	timer_time = clock();
	timer_interval = 750;

	flash_interval = 250;
	flashing_flag = 0;
	flash_timer_time = clock();

	is_dead = 0;

	v_y = 0;

	attack_range.top = position.y - current_img->getheight() * 2;
	attack_range.bottom = position.y + current_img->getheight() / 2;
	attack_range.right = position.x + current_img->getwidth() / 2;
	attack_range.left = position.x - current_img->getwidth() / 2;

	attack_state = 0;
	attack_speed = -10;
	return_speed = 5;
	residence_period = 2000;

	hunt_route[0].x = position.x;
	hunt_route[0].y = position.y;
	hunt_route[1].y = attack_range.top;
	hunt_route[1].x = position.x;

	return_route[0].x = hunt_route[1].x;
	return_route[0].y = hunt_route[1].y;
	return_route[1].x = position.x;
	return_route[1].y = position.y;

	arrival_time = clock();
}
void Trichiurus::UpdatePosition() {
	if (hurt_flash == 0) {
		position.y += v_y;
	}
	else {
		position.y += scroll_speed;
	}
	rect.left = position.x - current_img->getwidth() / 4;
	rect.right = position.x + current_img->getwidth() / 4;
	rect.top = position.y - current_img->getheight() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;

	attack_range.top = position.y - current_img->getheight() * 2;
	attack_range.bottom = position.y + current_img->getheight() / 2;
	attack_range.right = position.x + current_img->getwidth() / 2;
	attack_range.left = position.x - current_img->getwidth() / 2;
}
void Trichiurus::Show() {
	if ((clock() - timer_time) >= timer_interval) {
		timer_time = clock();
		if (current_img == &img_list[1]) current_img = &img_list[0];
		else current_img = &img_list[1];
		if (current_mask == &mask_list[1]) current_mask = &mask_list[0];
		else current_mask = &mask_list[1];
	}

	if (!flashing_flag) {
		putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_mask, SRCAND);
		putimage(position.x - current_img->getwidth() / 2, position.y - current_img->getheight() / 2, current_img, SRCPAINT);
	}
	else {

	}
}
void Trichiurus::StateControl(Hero* hero) {
	if ((attack_state == 0) && (RectCrush(attack_range, hero->GetRect()) == 1)) {
		attack_state = 1;
	}
	else if ((attack_state == 1) && (position.y - current_img->getheight() / 2 <= hunt_route[1].y)) {
		arrival_time = clock();
		attack_state = 2;
	}
	else if (attack_state == 2) {
		if (clock() - arrival_time >= residence_period) {
			attack_state = 3;
		}
	}
	else if ((attack_state == 3) && (position.y >= return_route[1].y)) {
		attack_state = 0;
	}
}
void Trichiurus::UpdateRoute(Hero* hero) {
	if (attack_state == 0) {
		hunt_route[1].y = hero->GetPositionY();
	}
	hunt_route[0].x = position.x;
	hunt_route[0].y = position.y;

	return_route[0].x = position.x;
	return_route[0].y = position.y;
	return_route[1].y += scroll_speed;
}
void Trichiurus::SpeedControl() {
	switch (attack_state)
	{
	case(0): {
		v_y = scroll_speed;
		break;
	}
	case(1): {
		if (hurt_flash == 0) {
			if (position.y - current_img->getheight() / 2 + v_y < hunt_route[1].y) {
				v_y = hunt_route[1].y - position.y + current_img->getheight() / 2;
			}
			else {
				v_y = attack_speed;
			}
		}
		else {

		}
		break;
	}
	case(2): {
		v_y = scroll_speed;
		break;
	}
	case(3): {
		if (hurt_flash == 0) {
			if (position.y + v_y > return_route[1].y) {
				v_y = return_route[1].y - position.y;
			}
			else if (position.y >= return_route[1].y) {
				v_y = scroll_speed;
			}
			else {
				v_y = return_speed;
			}
		}
		else {

		}
		break;
	}
	default: {
		cout << "未知状态" << endl;
		break;
	}
	}
}
Trichiurus::~Trichiurus() {
	cout << "Trichiurus被销毁" << endl;
}