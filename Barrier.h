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
	Position position;	//���ĵ�����
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
	//�Թ��ӵ���ײ���
	Hook* hook = &hero->GetHook();
	bool hook_in = (hook->position.x >= rect.left) && (hook->position.x <= rect.right) && (hook->position.y >= rect.top) && (hook->position.y <= rect.bottom);
	if (hook_in && (hero->GetAttackState() == 1)) {	//hero->GetAttackState() == 1ȷ��ֻ����һ��
		switch (hook->hook_direction)
		{
		case(1): {
			//���ϵĹ�����Ч��hook������
			hero->SetLineTarget(hero->GetPositionX(), rect.bottom);	
			break;
		}
		case(2): {
			//���µĹ�����Ч��hook������
			hero->SetLineTarget(hero->GetPositionX(), rect.top);
			break;
		}
		case(3): {
			//����Ĺ�����Ч��hook������
			hero->SetLineTarget(rect.right, hero->GetPositionY());
			break;
		}
		case(4): {
			//���ҵĹ�����Ч��hook������
			hero->SetLineTarget(rect.left, hero->GetPositionY());
			break;
		}
		case(5): {
			//�����ϵĹ�����Ч
			if ((hook->position.x > rect.left) && (hook->position.y <= rect.bottom)) {
				hero->SetLineTarget(hero->GetPositionX() + (rect.bottom - hero->GetPositionY()), rect.bottom);
			}
			else {
				hero->SetLineTarget(rect.left, hero->GetPositionY() + (rect.left - hero->GetPositionX()));
			}
			break;
		}
		case(6): {
			//�����µĹ�����Ч��hook������
			if ((hook->position.x > rect.left) && (hook->position.y >= rect.top)) {
				hero->SetLineTarget(hero->GetPositionX() + (rect.top - hero->GetPositionY()), rect.top);
			}
			else {
				hero->SetLineTarget(rect.left, hero->GetPositionY() + (rect.left - hero->GetPositionX()));
			}
			break;
		}
		case(7): {
			//�����µĹ�����Ч��hook������
			if ((hook->position.x < rect.right) && (hook->position.y >= rect.top)) {
				hero->SetLineTarget(hero->GetPositionX() - (rect.top - hero->GetPositionY()), rect.top);
			}
			else {
				hero->SetLineTarget(rect.left, hero->GetPositionY() + (rect.left - hero->GetPositionX()));
			}
			break;
		}
		case(8): {
			//�����ϵĹ�����Ч��hook������
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
	
	//�����ǵ���ײ���
	RECT* hero_rect = &(hero->GetRect());
	if (RectCrush(*hero_rect, rect)) {
		//����Brick��Hero��λʸ���ǣ�����Brick�ĶԽ��ߣ����Խ���ײ�����Ϊ�ϡ��ҡ��¡����ĸ����򣬼���ǽ�ڹ⻬����Brick���ĵ�Ϊԭ��
		if (hypot(position.x - hero->GetPositionX(), position.y - hero->GetPositionY()) == 0) {
			//��ֹsin_theta��cos_theta�ĳ���Ϊ0
			hero->CollisionDetect(3);
			//cout << "���·���" << endl;
			return;
		}
		float sin_theta = (position.y - hero->GetPositionY()) / hypot(position.x - hero->GetPositionX(), position.y - hero->GetPositionY());
		float cos_theta = -(position.x - hero->GetPositionX()) / hypot(position.x - hero->GetPositionX(), position.y - hero->GetPositionY());
		float theta1 = current_img->getheight() / 2 / hypot(current_img->getwidth() / 2, current_img->getheight() / 2);
		float theta2 = current_img->getwidth() / 2 / hypot(current_img->getwidth() / 2, current_img->getheight() / 2);
		//1�����Ϸ�����2�����ҷ�����3�����·�����4���󷴵�
		if (sin_theta >= theta1){
			hero->CollisionDetect(1);
			//cout << "���Ϸ���" << endl;
		}
		else if ((sin_theta < theta1) && (cos_theta >= theta2)) {
			hero->CollisionDetect(2);
			//cout << "���ҷ���" << endl;
		}
		else if (sin_theta <= -theta1) {
			hero->CollisionDetect(3);
			//cout << "���·���" << endl;
		}
		else if ((sin_theta < theta1) && (cos_theta <= -theta2)) {
			hero->CollisionDetect(4);
			//cout << "���󷴵�" << endl;
		}
		else {
			//cout << "δ֪������ײ" << endl;
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
	//current_img �ĳ�ʼ��ֵ�� nullptr������� Brick ��ľ�̬��Ա���� SetImageAndMask �У�
	//��ҪΪ img_list �� mask_list �����ڴ沢����ͼ�񡣷����ڴ��� Brick ����ʱ��current_img ��Ȼ�ǿ�ָ�롣
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
	Wall(Position P0, int id, int wall_type);	//P0Ϊ���ϵ��£������ҵ�һ��ש�����ĵ����꣬wall_typeΪǽ�����ͣ���ֵΪ1~28
	void Show();
	void UpdatePosition();
	void CollisitionDetect(Hero* hero);
	RECT& GetRect();
	vector<RECT*>& GetRects();	//��ȡ����ש��ľ���
	vector<Position*>& GetPositions();	//��ȡ����ש���λ��
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

	//cout << "����Wall��IDΪ" << ID << endl;
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
	//cout << "Wall" << ID << "������" << endl;
	auto brick_it = brick_list.begin();
	while (brick_it != brick_list.end()) {
		delete (*brick_it);
		brick_it++;
	}
}
//ע�⣬Wall������ʱ�����ڶ������ٵ�����brick_list���ᱻ���٣���Ҫ�ֶ������ڴ�