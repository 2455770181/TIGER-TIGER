#pragma once
#include"GlobalSettings.h"
#include"Hero.h"
#include<math.h>

class Brick {
public:
	Brick(Position P);
	void Show();
	void UpdatePosition(int delta_x, int delta_y);
	void CollisionDetect(Hero* hero);
	Position& GetPosition();
	RECT& GetRect();
	static int height;
	static IMAGE* img_list;
	static IMAGE* mask_list;
	static void SetImageAndMask();
	static void ReleaseMemory();
protected:
	Position position;	//中心点坐标
	RECT rect;
	IMAGE* current_img;
	IMAGE* current_mask;
};
Brick::Brick(Position P) {
	position.x = P.x;
	position.y = P.y;

	current_img = img_list;
	current_mask = mask_list;

	rect.top = position.y - current_img->getheight() / 2;
	rect.right = position.x + current_img->getwidth() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;
	rect.left = position.x - current_img->getwidth() / 2;
}
void Brick::Show() {
	putimage(rect.left, rect.top, current_mask, SRCAND);
	putimage(rect.left, rect.top, current_img, SRCPAINT);
}
void Brick::UpdatePosition(int delta_x = 0, int delta_y = 0) {
	position.x += delta_x;
	position.y += delta_y;

	rect.top = position.y - current_img->getheight() / 2;
	rect.right = position.x + current_img->getwidth() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;
	rect.left = position.x - current_img->getwidth() / 2;
}
void Brick::CollisionDetect(Hero* hero) {
	//对钩子的碰撞检测
	Hook* hook = &hero->GetHook();
	bool hook_in = (hook->position.x >= rect.left) && (hook->position.x <= rect.right) && (hook->position.y >= rect.top) && (hook->position.y <= rect.bottom);
	if (hook_in && (hero->GetAttackState() == 1)) {	//hero->GetAttackState() == 1确保只设置一次
		switch (hook->hook_direction)
		{
		case(1): {
			//向上的攻击无效，hook被弹回
			hero->SetLineTarget(hero->GetPositionX(), rect.bottom);	
			break;
		}
		case(2): {
			//向下的攻击无效，hook被弹回
			hero->SetLineTarget(hero->GetPositionX(), rect.top);
			break;
		}
		case(3): {
			//向左的攻击无效，hook被弹回
			hero->SetLineTarget(rect.right, hero->GetPositionY());
			break;
		}
		case(4): {
			//向右的攻击无效，hook被弹回
			hero->SetLineTarget(rect.left, hero->GetPositionY());
			break;
		}
		case(5): {
			//向右上的攻击无效
			if ((hook->position.x > rect.left) && (hook->position.y <= rect.bottom)) {
				hero->SetLineTarget(hero->GetPositionX() + (rect.bottom - hero->GetPositionY()), rect.bottom);
			}
			else {
				hero->SetLineTarget(rect.left, hero->GetPositionY() + (rect.left - hero->GetPositionX()));
			}
			break;
		}
		case(6): {
			//向右下的攻击无效，hook被弹回
			if ((hook->position.x > rect.left) && (hook->position.y >= rect.top)) {
				hero->SetLineTarget(hero->GetPositionX() + (rect.top - hero->GetPositionY()), rect.top);
			}
			else {
				hero->SetLineTarget(rect.left, hero->GetPositionY() + (rect.left - hero->GetPositionX()));
			}
			break;
		}
		case(7): {
			//向左下的攻击无效，hook被弹回
			if ((hook->position.x < rect.right) && (hook->position.y >= rect.top)) {
				hero->SetLineTarget(hero->GetPositionX() - (rect.top - hero->GetPositionY()), rect.top);
			}
			else {
				hero->SetLineTarget(rect.left, hero->GetPositionY() + (rect.left - hero->GetPositionX()));
			}
			break;
		}
		case(8): {
			//向左上的攻击无效，hook被弹回
			if ((hook->position.x < rect.right) && (hook->position.y <= rect.bottom)) {
				hero->SetLineTarget(hero->GetPositionX() + (rect.bottom - hero->GetPositionY()), rect.bottom);
			}
			else {
				hero->SetLineTarget(rect.right, hero->GetPositionY() + (rect.right - hero->GetPositionX()));
			}
			break;
		}
		default:
			break;
		}
	}
	else {

	}
	
	//对主角的碰撞检测
	RECT* hero_rect = &(hero->GetRect());
	if (RectCrush(*hero_rect, rect)) {
		//计算Brick到Hero的位矢辐角，根据Brick的对角线，可以将碰撞区域分为上、右、下、左四个区域，假设墙壁光滑，以Brick中心点为原点
		if (hypot(position.x - hero->GetPositionX(), position.y - hero->GetPositionY()) == 0) {
			//防止sin_theta和cos_theta的除数为0
			hero->CollisionDetect(3);
			//cout << "向下反弹" << endl;
			return;
		}
		float sin_theta = (position.y - hero->GetPositionY()) / hypot(position.x - hero->GetPositionX(), position.y - hero->GetPositionY());
		float cos_theta = -(position.x - hero->GetPositionX()) / hypot(position.x - hero->GetPositionX(), position.y - hero->GetPositionY());
		float theta1 = current_img->getheight() / 2 / hypot(current_img->getwidth() / 2, current_img->getheight() / 2);
		float theta2 = current_img->getwidth() / 2 / hypot(current_img->getwidth() / 2, current_img->getheight() / 2);
		//1：向上反弹；2：向右反弹；3：向下反弹；4向左反弹
		if (sin_theta >= theta1){
			hero->CollisionDetect(1);
			//cout << "向上反弹" << endl;
		}
		else if ((sin_theta < theta1) && (cos_theta >= theta2)) {
			hero->CollisionDetect(2);
			//cout << "向右反弹" << endl;
		}
		else if (sin_theta <= -theta1) {
			hero->CollisionDetect(3);
			//cout << "向下反弹" << endl;
		}
		else if ((sin_theta < theta1) && (cos_theta <= -theta2)) {
			hero->CollisionDetect(4);
			//cout << "向左反弹" << endl;
		}
		else {
			//cout << "未知方向碰撞" << endl;
			hero->CollisionDetect(0);
		}
	}
	else {

	}
}
Position& Brick::GetPosition() {
	return position;
}
RECT& Brick::GetRect() {
	return rect;
}
int Brick::height = 0;
IMAGE* Brick::img_list = NULL;
IMAGE* Brick::mask_list = NULL;
void Brick::SetImageAndMask() {
	//current_img 的初始化值是 nullptr，因此在 Brick 类的静态成员函数 SetImageAndMask 中，
	//需要为 img_list 和 mask_list 分配内存并加载图像。否则，在创建 Brick 对象时，current_img 仍然是空指针。
	img_list = new IMAGE;
	loadimage((Brick::img_list), _T("./image/Barrier/brick1_i.png"));
	mask_list = new IMAGE;
	loadimage((Brick::mask_list), _T("./image/Barrier/brick1_m.png"));
	height = img_list->getheight();
}
void Brick::ReleaseMemory() {
	delete img_list;
	delete mask_list;
}

class Wall {
public:
	Wall(Position P0, int id, int wall_type);	//P0为从上到下，从左到右第一块砖的中心点坐标，wall_type为墙的类型，其值为1~28
	void Show();
	void UpdatePosition();
	void CollisitionDetect(Hero* hero);
	RECT& GetRect();
	vector<RECT*>& GetRects();	//获取所有砖块的矩形
	vector<Position*>& GetPositions();	//获取所有砖块的位置
	int GetID();
	~Wall();
private:
	int ID;
	Position position;
	RECT rect;
	vector<RECT*> rect_list;
	vector<Position*> position_list;
	int brick_width;
	vector<Brick*> brick_list;
};
Wall::Wall(Position P1, int id, int wall_type) {
	ID = id;

	position.x = P1.x;
	position.y = P1.y;

	string str = "./wall_data/bricks_relative_position/Wall_";
	stringstream ss;
	ss << str << wall_type << ".txt";
	string result = ss.str();
	vector<RouteNode*>* rnl = ReadRouteNodeList(result);

	Position P = P1;
	Brick b1 = Brick(P);
	brick_width = Brick::height;

	rect.top = position.y - brick_width / 2;
	rect.right = position.x + brick_width / 2;
	rect.bottom = position.y + brick_width / 2;
	rect.left = position.x - brick_width / 2;

	auto rnl_it = rnl->begin();
	while (rnl_it != rnl->end()) {
		P = { brick_width * (*rnl_it)->SP.x + P1.x,brick_width * (*rnl_it)->SP.y + P1.y };
		Brick* b = new Brick(P);
		brick_list.push_back(b);
		rect_list.push_back(&b->GetRect());
		position_list.push_back(&b->GetPosition());
		rect.right = (P.x > rect.right) ? P.x : rect.right;
		rect.left = (P.x < rect.left) ? P.x : rect.left;
		rect.bottom = (P.y + brick_width / 2 > rect.bottom) ? P.y + brick_width / 2: rect.bottom;
		rnl_it++;
	}
	ReleaseRouteNodeList(rnl);

	//cout << "生成Wall，ID为" << ID << endl;
}
void Wall::Show() {
	auto it = brick_list.begin();
	while (it != brick_list.end()) {
		(*it)->Show();
		it++;
	}
}
void Wall::UpdatePosition() {
	auto it = brick_list.begin();
	while (it != brick_list.end()) {
		(*it)->UpdatePosition(0, scroll_speed);
		it++;
	}
	position.y += scroll_speed;
	rect.top += scroll_speed;
	rect.bottom += scroll_speed;
}
void Wall::CollisitionDetect(Hero* hero) {
	auto it = brick_list.begin();
	while (it != brick_list.end()) {
		(*it)->CollisionDetect(hero);
		it++;
	}
}
RECT& Wall::GetRect() {
	return rect;
}
vector<RECT*>& Wall::GetRects() {
	return rect_list;
}
vector<Position*>& Wall::GetPositions() {
	return position_list;
}
int Wall::GetID() {
	return ID;
}
Wall::~Wall() {
	//cout << "Wall" << ID << "被销毁" << endl;
	auto brick_it = brick_list.begin();
	while (brick_it != brick_list.end()) {
		delete (*brick_it);
		brick_it++;
	}
}
//注意，Wall被销毁时，其在堆区开辟的属性brick_list不会被销毁，需要手动回收内存