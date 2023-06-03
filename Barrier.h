#pragma once
#include"GlobalSettings.h"
#include"Hero.h"
#include<math.h>
class Brick {
public:
	void Show();
	void UpdatePosition(int delta_x, int delta_y);
	void CollisionDetect(Hero* hero);
	Position& GetPosition();
	RECT& GetRect();
protected:
	Position position;	//���ĵ�����
	RECT rect;

	IMAGE img_list;
	IMAGE mask_list;
	IMAGE* current_img;
	IMAGE* current_mask;
};
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
			cout << "���·���" << endl;
			return;
		}
		float sin_theta = (position.y - hero->GetPositionY()) / hypot(position.x - hero->GetPositionX(), position.y - hero->GetPositionY());
		float cos_theta = -(position.x - hero->GetPositionX()) / hypot(position.x - hero->GetPositionX(), position.y - hero->GetPositionY());
		float theta1 = current_img->getheight() / 2 / hypot(current_img->getwidth() / 2, current_img->getheight() / 2);
		float theta2 = current_img->getwidth() / 2 / hypot(current_img->getwidth() / 2, current_img->getheight() / 2);
		//1�����Ϸ�����2�����ҷ�����3�����·�����4���󷴵�
		if (sin_theta >= theta1){
			hero->CollisionDetect(1);
			cout << "���Ϸ���" << endl;
		}
		else if ((sin_theta < theta1) && (cos_theta >= theta2)) {
			hero->CollisionDetect(2);
			cout << "���ҷ���" << endl;
		}
		else if (sin_theta <= -theta1) {
			hero->CollisionDetect(3);
			cout << "���·���" << endl;
		}
		else if ((sin_theta < theta1) && (cos_theta <= -theta2)) {
			hero->CollisionDetect(4);
			cout << "���󷴵�" << endl;
		}
		else {
			cout << "δ֪������ײ" << endl;
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

class Brick_1 : public Brick {
public:
	Brick_1(Position P);
};
Brick_1::Brick_1(Position P) {
	position.x = P.x;
	position.y = P.y;

	loadimage(&img_list, _T("./image/Barrier/brick1_i.png"));
	loadimage(&mask_list, _T("./image/Barrier/brick1_m.png"));

	current_img = &img_list;
	current_mask = &mask_list;

	rect.top = position.y - current_img->getheight() / 2;
	rect.right = position.x + current_img->getwidth() / 2;
	rect.bottom = position.y + current_img->getheight() / 2;
	rect.left = position.x - current_img->getwidth() / 2;
}

class Wall_1 {
public:
	Wall_1(Position P1);	//P1Ϊ���ϵ��£������ҵ�һ��ש�����ĵ�����
	void Show();
	void UpdatePosition();
	void CollisitionDetect(Hero* hero);
	RECT& GetRect();
	vector<RECT*>& GetRects();	//��ȡ����ש��ľ���
	vector<Position*>& GetPositions();	//��ȡ����ש���λ��
	~Wall_1();
private:
	Position position;
	RECT rect;
	vector<RECT*> rect_list;
	vector<Position*> position_list;
	int brick_width;
	vector<Brick_1*> brick_list;
};
Wall_1::Wall_1(Position P1) {
	position.x = P1.x;
	position.y = P1.y;

	Brick_1* b1 = new Brick_1(P1);
	brick_width = b1->GetRect().right - b1->GetRect().left;
	Position P2 = { P1.x + brick_width,P1.y };
	Brick_1* b2 = new Brick_1(P2);
	brick_list.push_back(b1);
	brick_list.push_back(b2);

	rect_list.push_back(&b1->GetRect());
	rect_list.push_back(&b2->GetRect());
	position_list.push_back(&b1->GetPosition());
	position_list.push_back(&b2->GetPosition());

	rect.top = position.y - brick_width / 2;
	rect.right = position.x + 3 * brick_width / 2;
	rect.bottom = position.y + brick_width / 2;
	rect.left = position.x - brick_width / 2;
}
void Wall_1::Show() {
	auto it = brick_list.begin();
	while (it != brick_list.end()) {
		(*it)->Show();
		it++;
	}
}
void Wall_1::UpdatePosition() {
	auto it = brick_list.begin();
	while (it != brick_list.end()) {
		(*it)->UpdatePosition(0, scroll_speed);
		it++;
	}
	position.y += scroll_speed;
	rect.top = position.y - brick_width / 2;
	rect.right = position.x + 3 * brick_width / 2;
	rect.bottom = position.y + brick_width / 2;
	rect.left = position.x - brick_width / 2;
}
void Wall_1::CollisitionDetect(Hero* hero) {
	auto it = brick_list.begin();
	while (it != brick_list.end()) {
		(*it)->CollisionDetect(hero);
		it++;
	}
}
RECT& Wall_1::GetRect() {
	return rect;
}
vector<RECT*>& Wall_1::GetRects() {
	return rect_list;
}
vector<Position*>& Wall_1::GetPositions() {
	return position_list;
}
Wall_1::~Wall_1() {
	cout << "Wall_1������" << endl;
}