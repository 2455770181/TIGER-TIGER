#include<iostream>
#include<vector>
#include<math.h>
#include<conio.h>
#include"GlobalSettings.h"
#include"Enemy.h"
#include"Barrier.h"
#include"Prop.h"
using namespace std;

void Welcome() {
	// ����������ʽ
	const TCHAR* font_style = _T("Consolas");

	// ������ɫ
	COLORREF title_color1 = RGB(213, 159, 112); // STAGE����ɫ
	COLORREF title_color2 = RGB(209, 208, 128); // CLEAR����ɫ
	COLORREF text_color = RGB(191, 235, 218); // �ı�����ɫ
	COLORREF bg_color = RGB(39, 82, 91); // ��������ɫ

	// ���������ı��Ĵ�С
	const int title_size = 80;
	const int text_size = 40;

	// ���������ı���λ��
	const int title_x = screen_width / 2; // ����ĺ�����
	const int title_y1 = screen_height / 6; // TIGER!��������
	const int title_y2 = title_y1 + title_size; // TIGER!��������
	// ���ñ�����ɫ
	setbkmode(OPAQUE);
	setbkcolor(bg_color);
	cleardevice();

	LPCTSTR title = _T("TIGER!");	//_T()��������ת���ַ�����
	LPCTSTR tplay = _T("��ʼ��Ϸ");
	LPCTSTR texit = _T("�˳���Ϸ");

	RECT tplayr, texitr;	//����������ľ�������

	BeginBatchDraw();	//��ֹͼ����˸

	setbkcolor(RGB(14, 52, 59));
	cleardevice();

	// ���ñ��������
	settextstyle(title_size, 0, font_style);

	// ���ñ������ɫ
	settextcolor(title_color1);

	// ����Ļ����������ʾTIGER!
	outtextxy(title_x - textwidth(title) / 2, title_y1, title);
	// ���ñ������ɫ
	settextcolor(title_color2);
	// ����Ļ����������ʾTIGER!
	outtextxy(title_x - textwidth(title) / 2, title_y2, title);
	
	// �����ı�������
	settextstyle(text_size, 0, font_style);

	tplayr.left = title_x - textwidth(tplay) / 2;
	tplayr.right = title_x + textwidth(tplay) / 2;
	tplayr.top = screen_height / 5 * 2.5;
	tplayr.bottom = tplayr.top + textheight(tplay);

	texitr.left = title_x - textwidth(texit) / 2;
	texitr.right = title_x + textwidth(texit) / 2;
	texitr.top = screen_height / 5 * 3;
	texitr.bottom = texitr.top + textheight(texit);

	// �����ı�����ɫ
	settextcolor(text_color);

	outtextxy(tplayr.left, tplayr.top, tplay);	//���ƿ�ʼ��Ϸ����
	outtextxy(texitr.left, texitr.top, texit);	//�����˳���Ϸ����

	EndBatchDraw();

	bool startGameClicked = false;
	ExMessage mess;
	while (!startGameClicked) {
		if (peekmessage(&mess,EM_MOUSE)) {
			if (mess.message == WM_LBUTTONDOWN) {
				if (PointInRect(mess.x, mess.y, tplayr)) {
					startGameClicked = true;
				}
				else if (PointInRect(mess.x, mess.y, texitr)) {
					exit(0);
				}
			}
		}
	}
}

class BK {
public:
	BK(IMAGE& img) :img(img), y(0) {

	}

	void Show(bool upordown) {
		if (upordown) {
			if (y <= -(img.getheight() - screen_height)) {
				y = 0;
			}
		}
		else {
			if (y >= 0) {
				y = -(img.getheight() - screen_height);
			}
		}
		
		y += scroll_speed;
		putimage(0, y, &img);	//��ͼƬ�����ϽǷŵ�ָ�����꣬Ȼ�����ͼƬ
	}
private:
	IMAGE& img;
	int y;	//����λ�õ� y ����
};

template<typename T>
void Show(vector<T*>* es) {
	auto it = es->begin();
	while (it != es->end()) {
		(*it)->Show();
		it++;
	}
}

template<typename T>
void ReleaseVector(vector<T*>* es) {
	// ����������ɾ��ÿ��Ԫ��ָ����ڴ��
	for (auto& it : *es) {
		delete it;
	}

	// �������
	es->clear();

	// �ͷ�ָ������������ڴ��
	delete es;
}

//���Ƶ��˵��������ڴ���գ��Լ����ܵ���ĵ��ߣ��޵�ʱ����߻�����ƿ��
template<typename T>
void ControlEnemyDeath(vector<T*>* es, vector<RouteNode*>* rnl, vector<InvincibleProp*>* invincibleprop_es, vector<OxygenBottle*>* oxygenbottle_es, int& extra_score) {
	auto it = es->begin();
	while (it != es->end()) {
		if ((*it)->IfDead()) {
			extra_score += 200;

			Position P = (*it)->GetPosition();
			srand((unsigned)time(NULL));
			int temp_rand = rand() % 10;
			if (temp_rand == 0) {
				InvincibleProp* IP = new InvincibleProp(P);
				invincibleprop_es->push_back(IP);
			}
			else if (temp_rand == 1) {
				OxygenBottle* OB = new OxygenBottle(P);
				oxygenbottle_es->push_back(OB);
			}
			
			auto rnl_it = rnl->begin();
			while (rnl_it != rnl->end()) {
				if ((*rnl_it)->id == (*it)->GetID()) {
					delete (*rnl_it);
					rnl->erase(rnl_it);

					delete (*it);
					es->erase(it);
					return;
				}
				rnl_it++;
			}
		}
		it++;
	}
}

template<typename T>
void RemoveObject(vector<T*>* es, vector<RouteNode*>* rnl, bool upordown) {
	int pre_distance = 50;
	auto it = es->begin();
	while (it != es->end()) {
		if ((upordown == 1) && ((*it)->GetRect().bottom <= -pre_distance)) {
			auto rnl_it = rnl->begin();
			while (rnl_it != rnl->end()) {
				if ((*rnl_it)->id == (*it)->GetID()) {
					(*rnl_it)->flag = 0;

					delete (*it);
					es->erase(it);
					return;
				}
				rnl_it++;
			}
		}
		else if ((upordown == 0) && ((*it)->GetRect().top >= game_area_height + pre_distance)) {
			auto rnl_it = rnl->begin();
			while (rnl_it != rnl->end()) {
				if ((*rnl_it)->id == (*it)->GetID()) {
					(*rnl_it)->flag = 0;

					delete (*it);
					es->erase(it);
					return;
				}
				rnl_it++;
			}
		}
		it++;
	}
}

//�����������InvicibleProp��OxygenBottle
template<typename T>
void RemoveObject(vector<T*>* es, bool upordown) {
	int pre_distance = 50;
	auto it = es->begin();
	while (it != es->end()) {
		if ((upordown == 1) && ((*it)->GetRect().bottom <= -pre_distance)) {
			delete (*it);
			es->erase(it);
			return;
		}
		else if ((upordown == 0) && ((*it)->GetRect().top >= game_area_height + pre_distance)) {
			delete (*it);
			es->erase(it);
			return;
		}
		it++;
	}
}

//��ȡ��Ļ�ϵ�����ľ��μ����Ӧ����
template<typename T>
void AddRectAndPosition(vector<T*>* es, vector<RECT*>& rect_es, vector<Position*>& position_es) {
	auto it = es->begin();
	while (it != es->end()) {
		rect_es.push_back(&(*it)->GetRect());
		position_es.push_back(&(*it)->GetPosition());
		it++;
	}
}

void AddRectAndPosition(vector<Wall*>* wall_es, vector<RECT*>& rect_es, vector<Position*>& position_es) {
	auto wall_it = wall_es->begin();
	while (wall_it != wall_es->end()) {
		vector<RECT*> temp_rect_list = (*wall_it)->GetRects();
		auto temp_rect_it = temp_rect_list.begin();
		while (temp_rect_it != temp_rect_list.end()) {
			rect_es.push_back(*temp_rect_it);
			temp_rect_it++;
		}
		vector<Position*> temp_position_list = (*wall_it)->GetPositions();
		auto temp_position_it = temp_position_list.begin();
		while (temp_position_it != temp_position_list.end()) {
			position_es.push_back(*temp_position_it);
			temp_position_it++;
		}
		wall_it++;
	}
}

//upordown��������������Ϊ0������Ϊ1
template<typename T>
void GenerateObject(vector<T*>* es, vector<RouteNode*>* rnl, int offsetY, bool upordown) {
	auto rnl_it = rnl->begin();
	int pre_distance = 50;
	while (rnl_it != rnl->end()) {
		//offsetY + game_area_height + pre_distance����ǰ���ɣ�ʹ�仺��������Ұ
		if (((*rnl_it)->offsetY <= offsetY + game_area_height + pre_distance) && ((*rnl_it)->offsetY >= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 1)) {
			int y = ((*rnl_it)->offsetY <= game_area_height) ? (*rnl_it)->offsetY : game_area_height + pre_distance;
			(*rnl_it)->SP.y = y;
			(*rnl_it)->END.y = y;
			Position node1_P[2] = { (*rnl_it)->SP, (*rnl_it)->END };
			T* node1 = new T(node1_P, (*rnl_it)->id);
			es->push_back(node1);
			(*rnl_it)->flag = 1;
			break;
		}
		else if (((*rnl_it)->offsetY >= offsetY - pre_distance) && ((*rnl_it)->offsetY <= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 0)) {
			(*rnl_it)->SP.y = -pre_distance;
			(*rnl_it)->END.y = -pre_distance;
			Position node2_P[2] = { (*rnl_it)->SP, (*rnl_it)->END };
			T* node2 = new T(node2_P, (*rnl_it)->id);
			es->push_back(node2);
			(*rnl_it)->flag = 1;
			break;
		}
		rnl_it++;
	}
}

template<typename T>
void GenerateProp(vector<T*>* es, vector<RouteNode*>* rnl, int offsetY, bool upordown) {
	auto rnl_it = rnl->begin();
	int pre_distance = 50;
	while (rnl_it != rnl->end()) {
		//offsetY + game_area_height + pre_distance����ǰ���ɣ�ʹ�仺��������Ұ
		if (((*rnl_it)->offsetY <= offsetY + game_area_height + pre_distance) && ((*rnl_it)->offsetY >= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 1)) {
			int y = ((*rnl_it)->offsetY <= game_area_height) ? (*rnl_it)->offsetY : game_area_height + pre_distance;
			(*rnl_it)->SP.y = y;
			Position node1_P = (*rnl_it)->SP;
			T* node1 = new T(node1_P, (*rnl_it)->id);
			es->push_back(node1);
			(*rnl_it)->flag = 1;
			break;
		}
		else if (((*rnl_it)->offsetY >= offsetY - pre_distance) && ((*rnl_it)->offsetY <= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 0)) {
			cout << "GenerateProp2" << endl;
			(*rnl_it)->SP.y = -pre_distance;
			Position node2_P = (*rnl_it)->SP;
			T* node2 = new T(node2_P, (*rnl_it)->id);
			es->push_back(node2);
			(*rnl_it)->flag = 1;
			break;
		}
		rnl_it++;
	}
}

void GenerateObject(vector<Lobster*>* es, vector<RouteNode*>* rnl, int offsetY, bool upordown) {
	auto rnl_it = rnl->begin();
	int pre_distance = 50;
	while (rnl_it != rnl->end()) {
		//offsetY + game_area_height + pre_distance����ǰ���ɣ�ʹ�仺��������Ұ
		if (((*rnl_it)->offsetY <= offsetY + game_area_height + pre_distance) && ((*rnl_it)->offsetY >= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 1)) {
			(*rnl_it)->SP.y = game_area_height + pre_distance;
			(*rnl_it)->END.y = game_area_height + pre_distance;
			Position L_P = (*rnl_it)->SP;
			Lobster* L1 = new Lobster(L_P, (*rnl_it)->id);
			es->push_back(L1);
			(*rnl_it)->flag = 1;
			break;
		}
		else if (((*rnl_it)->offsetY >= offsetY - pre_distance) && ((*rnl_it)->offsetY <= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 0)) {
			(*rnl_it)->SP.y = -pre_distance;
			(*rnl_it)->END.y = -pre_distance;
			Position LP = (*rnl_it)->SP;
			Lobster* L2 = new Lobster(LP, (*rnl_it)->id);
			es->push_back(L2);
			(*rnl_it)->flag = 1;
			break;
		}
		rnl_it++;
	}
}

void GenerateObject(vector<Trichiurus*>* es, vector<RouteNode*>* rnl, int offsetY, bool upordown) {
		auto rnl_it = rnl->begin();
		int pre_distance = 50;
		while (rnl_it != rnl->end()) {
			//offsetY + game_area_height + pre_distance����ǰ���ɣ�ʹ�仺��������Ұ
			if (((*rnl_it)->offsetY <= offsetY + game_area_height + pre_distance) && ((*rnl_it)->offsetY >= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 1)) {
				(*rnl_it)->SP.y = game_area_height + pre_distance;
				(*rnl_it)->END.y = game_area_height + pre_distance;
				Position Tr_P = (*rnl_it)->SP;
				Trichiurus* Tr1 = new Trichiurus(Tr_P, (*rnl_it)->id);
				es->push_back(Tr1);
				(*rnl_it)->flag = 1;
				break;
			}
			else if (((*rnl_it)->offsetY >= offsetY - pre_distance) && ((*rnl_it)->offsetY <= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 0)) {
				(*rnl_it)->SP.y = -pre_distance;
				(*rnl_it)->END.y = -pre_distance;
				Position Tr_P = (*rnl_it)->SP;
				Trichiurus* Tr2 = new Trichiurus(Tr_P, (*rnl_it)->id);
				es->push_back(Tr2);
				(*rnl_it)->flag = 1;
				break;
			}
			rnl_it++;
		}
}

void GenerateObject(vector<LanternFish*>* es, vector<RouteNode*>* rnl, int offsetY, bool upordown) {
	auto rnl_it = rnl->begin();
	int pre_distance = 50;
	while (rnl_it != rnl->end()) {
		//offsetY + game_area_height + pre_distance����ǰ���ɣ�ʹ�仺��������Ұ
		if (((*rnl_it)->offsetY <= offsetY + game_area_height + pre_distance) && ((*rnl_it)->offsetY >= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 1)) {
			(*rnl_it)->SP.y = game_area_height + pre_distance;
			(*rnl_it)->END.y = game_area_height + pre_distance;
			Position LF_P = (*rnl_it)->SP;
			LanternFish* L1 = new LanternFish(LF_P, (*rnl_it)->id, (*rnl_it)->kind);
			es->push_back(L1);
			(*rnl_it)->flag = 1;
			break;
		}
		else if (((*rnl_it)->offsetY >= offsetY - pre_distance) && ((*rnl_it)->offsetY <= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 0)) {
			(*rnl_it)->SP.y = -pre_distance;
			(*rnl_it)->END.y = -pre_distance;
			Position LF_P = (*rnl_it)->SP;
			LanternFish* L2 = new LanternFish(LF_P, (*rnl_it)->id, (*rnl_it)->kind);
			es->push_back(L2);
			(*rnl_it)->flag = 1;
			break;
		}
		rnl_it++;
	}
}

void GenerateObject(vector<Wall*>* es, vector<RouteNode*>* rnl, int offsetY, bool upordown) {
	auto rnl_it = rnl->begin();
	int pre_distance = 50;
	while (rnl_it != rnl->end()) {
		int wall_hight = ((*rnl_it)->offsetY - 1) * Brick::height;	//�����offsetY������ǽ�����᷽��ש���������-1����Ϊȡ���ĵ�������
		//offsetY + game_area_height + pre_distance����ǰ���ɣ�ʹ�仺��������Ұ
		if (((*rnl_it)->SP.y <= offsetY + game_area_height + pre_distance) && ((*rnl_it)->SP.y >= offsetY) && ((*rnl_it)->flag == 0) && (upordown == 1)) {
			int y = ((*rnl_it)->SP.y <= game_area_height) ? (*rnl_it)->SP.y : game_area_height + pre_distance;
			Position W_P = { (*rnl_it)->SP.x, y };
			Wall* W1 = new Wall(W_P, (*rnl_it)->id, (*rnl_it)->kind);
			es->push_back(W1);
			(*rnl_it)->flag = 1;
			break;
		}
		else if (((*rnl_it)->SP.y + wall_hight >= offsetY - pre_distance) && ((*rnl_it)->SP.y + wall_hight <= offsetY + game_area_height) && ((*rnl_it)->flag == 0) && (upordown == 0)) {
			Position W_P = { (*rnl_it)->SP.x, -pre_distance - wall_hight };
			Wall* W2 = new Wall(W_P, (*rnl_it)->id, (*rnl_it)->kind);
			es->push_back(W2);
			(*rnl_it)->flag = 1;
			break;
		}
		rnl_it++;
	}
}

void ControlObject(vector<Turtle*>* turtle_es, vector<RouteNode*>* rnl, Hero& hp) {
	auto turtle_it = turtle_es->begin();
	RECT* it_rect;
	while (turtle_it != turtle_es->end()) {
		it_rect = &(*turtle_it)->GetRect();
		if (!(*turtle_it)->IfDying() && !(*turtle_it)->IfDead()) hp.AttackChecked(*it_rect);

		(*turtle_it)->UpdatePosition();
		(*turtle_it)->AttackedCheck(&hp);
		turtle_it++;
	}
}

void ControlObject(vector<Scaleph*>* scaleph_es, vector<RouteNode*>* rnl, Hero& hp) {
	auto scaleph_it = scaleph_es->begin();
	RECT* it_rect;
	while (scaleph_it != scaleph_es->end()) {
		it_rect = &(*scaleph_it)->GetRect();
		if (!(*scaleph_it)->IfDying() && !(*scaleph_it)->IfDead()) hp.AttackChecked(*it_rect);

		(*scaleph_it)->UpdatePosition();
		(*scaleph_it)->AttackedCheck(&hp);
		scaleph_it++;
	}
}

void ControlObject(vector<GreenFish*>* greenfish_es, vector<RouteNode*>* rnl, Hero& hp) {
	auto greenfish_it = greenfish_es->begin();
	while (greenfish_it != greenfish_es->end())
	{
		RECT it_rect = (*greenfish_it)->GetRect();
		if (!(*greenfish_it)->IfDying() && !(*greenfish_it)->IfDead()) hp.AttackChecked(it_rect);

		(*greenfish_it)->SpeedControl();
		(*greenfish_it)->UpdateRoute();
		(*greenfish_it)->UpdatePosition();
		(*greenfish_it)->AttackedCheck(&hp);
		greenfish_it++;
	}
}

void ControlObject(vector<RedFish*>* redfish_es, vector<RouteNode*>* rnl, vector<RECT*> rect_es, vector<Position*> position_es, Hero& hp) {
	auto redfish_it = redfish_es->begin();
	while (redfish_it != redfish_es->end()) {
		RECT it_rect = (*redfish_it)->GetRect();
		if (!(*redfish_it)->IfDying() && !(*redfish_it)->IfDead()) hp.AttackChecked(it_rect);

		(*redfish_it)->UpdateRoute(&hp);
		(*redfish_it)->SpeedControl(rect_es, position_es);
		(*redfish_it)->UpdatePosition();
		(*redfish_it)->StateControl(&hp);
		(*redfish_it)->AttackedCheck(&hp);

		redfish_it++;
	}
}

void ControlObject(vector<Lobster*>* lobster_es, vector<RouteNode*>* rnl, Hero& hp) {
	auto lobster_it = lobster_es->begin();
	while (lobster_it != lobster_es->end()) {
		RECT it_rect = (*lobster_it)->GetRect();
		if (!(*lobster_it)->IfDying() && !(*lobster_it)->IfDead()) hp.AttackChecked(it_rect);

		(*lobster_it)->UpdateRoute(&hp);
		(*lobster_it)->SpeedControl();
		(*lobster_it)->UpdatePosition();
		(*lobster_it)->StateControl(&hp);
		(*lobster_it)->AttackedCheck(&hp);
		lobster_it++;
	}
}

void ControlObject(vector<Trichiurus*>* trichiurus_es, vector<RouteNode*>* rnl, Hero& hp) {
	auto trichiurus_it = trichiurus_es->begin();
	while (trichiurus_it != trichiurus_es->end()) {
		RECT it_rect = (*trichiurus_it)->GetRect();
		if (!(*trichiurus_it)->IfDying() && !(*trichiurus_it)->IfDead()) hp.AttackChecked(it_rect);

		(*trichiurus_it)->UpdateRoute(&hp);
		(*trichiurus_it)->SpeedControl();
		(*trichiurus_it)->UpdatePosition();
		(*trichiurus_it)->StateControl(&hp);
		(*trichiurus_it)->AttackedCheck(&hp);
		trichiurus_it++;
	}
}

void ControlObject(vector<LanternFish*>* lanternfish_es, vector<RouteNode*>* rnl, Hero& hp) {
	auto lanternfish_it = lanternfish_es->begin();
	while (lanternfish_it != lanternfish_es->end()) {
		RECT it_rect = (*lanternfish_it)->GetRect();
		if (!(*lanternfish_it)->IfDying() && !(*lanternfish_it)->IfDead()) hp.AttackChecked(it_rect);

		(*lanternfish_it)->UpdateRoute(&hp);
		(*lanternfish_it)->SpeedControl();
		(*lanternfish_it)->UpdatePosition();
		(*lanternfish_it)->StateControl(&hp);
		(*lanternfish_it)->AttackedCheck(&hp);
		lanternfish_it++;
	}

}

void ControlObject(vector<InvincibleProp*>* invincibleprop_es, Hero& hp) {
	auto invincibleprop_it = invincibleprop_es->begin();
	while (invincibleprop_it != invincibleprop_es->end()) {
		Position temp_p = (*invincibleprop_it)->GetPosition();
		if (((*invincibleprop_it)->IfGotten(&hp, temp_p)) && !(hp.IfDying())) {
			hp.GetInvicibleTime();
		}
		(*invincibleprop_it)->UpdatePosition();
		if ((*invincibleprop_it)->ReachTarget()) {
			delete (*invincibleprop_it);
			invincibleprop_es->erase(invincibleprop_it);
			break;
		}
		invincibleprop_it++;
	}
}

void ControlObject(vector<OxygenBottle*>* oxygenbottle_es, Hero& hp) {
	auto oxygenbottle_it = oxygenbottle_es->begin();
	while (oxygenbottle_it != oxygenbottle_es->end()) {
		Position temp_p = (*oxygenbottle_it)->GetPosition();
		if ((*oxygenbottle_it)->IfGotten(&hp, temp_p)) {
			hp.GetOxygenBottle();
		}
		(*oxygenbottle_it)->UpdatePosition();
		if ((*oxygenbottle_it)->ReachTarget()) {
			delete (*oxygenbottle_it);
			oxygenbottle_es->erase(oxygenbottle_it);
			break;
		}
		oxygenbottle_it++;
	}
}

void ControlObject(vector<Box*>* box_es, Hero& hp) {
	Position temp_p = { game_area_width + 60 ,game_area_height / 3 + 60 };
	auto box_it = box_es->begin();
	while (box_it != box_es->end()) {
		(*box_it)->IfGotten(&hp, temp_p);
		(*box_it)->UpdatePosition();
		if ((*box_it)->ReachTarget()) {
			hp.GetBox();
			delete (*box_it);
			box_es->erase(box_it);
			break;
		}
		box_it++;
	}
}

void ControlObject(vector<Ether*>* ether_es, Hero& hp) {
	Position temp_p = { screen_width - 80 ,screen_height * 0.14 };
	auto ether_it = ether_es->begin();
	while (ether_it != ether_es->end()) {
		(*ether_it)->IfGotten(&hp, temp_p);
		(*ether_it)->UpdatePosition();
		if ((*ether_it)->ReachTarget()) {
			hp.GetEther();
			delete (*ether_it);
			ether_es->erase(ether_it);
			break;
		}
		ether_it++;
	}
}

void ControlObject(vector<SmallBox*>* smallbox_es, Hero& hp) {
	auto smallbox_it = smallbox_es->begin();
	Position P0 = { game_area_width + 60 ,game_area_height * 3 / 5 - 30 };
	Position P;
	unsigned int n = 0;
	while (smallbox_it != smallbox_es->end()) {
		n = hp.GetStageProp().smallbox + 1;	//�����õ�С������hp�ĵڼ������ӣ������������ƶ���Ŀ��λ��
		P.x = (n % 2 == 0) ? P0.x + 50 : P0.x;
		P.y = P0.y + floor((n - 1) / 2) * 50;
		(*smallbox_it)->IfGotten(&hp, P);
		(*smallbox_it)->UpdatePosition();
		if ((*smallbox_it)->ReachTarget()) {
			hp.GetSmallBox();
			delete (*smallbox_it);
			smallbox_es->erase(smallbox_it);
			break;
		}
		smallbox_it++;
		n++;
	}
}

void ControlObject(vector<EnhancedHook*>* enhancedhook_es, Hero& hp) {
	Position temp_p = { screen_width - 45 ,screen_height / 3 + 60 };
	auto enhancedhook_it = enhancedhook_es->begin();
	while (enhancedhook_it != enhancedhook_es->end()) {
		(*enhancedhook_it)->IfGotten(&hp, temp_p);
		(*enhancedhook_it)->UpdatePosition();
		if ((*enhancedhook_it)->ReachTarget()) {
			hp.GetEnhancedHook();
			delete (*enhancedhook_it);
			enhancedhook_es->erase(enhancedhook_it);
			break;
		}
		enhancedhook_it++;
	}
}

void ControlObject(vector<Stab*>* stab_es, Hero& hp) {
	auto stab_it = stab_es->begin();
	while (stab_it != stab_es->end()) {
		RECT it_rect = (*stab_it)->GetRect();
		hp.AttackChecked(it_rect, 1);
		(*stab_it)->UpdatePosition();
		//��������ˮ�׺�hp���������������ﲻ�ٶ�stab_es������
		stab_it++;
	}
}

void ControlObject(vector<Wall*>* wall_es, Hero& hp) {
	auto wall_it = wall_es->begin();
	while (wall_it != wall_es->end()) {
		(*wall_it)->CollisitionDetect(&hp);
		(*wall_it)->UpdatePosition();
		wall_it++;
	}
}

//es��0��1Ϊsmallbox��ԭͼ������ͼ��2��3Ϊenhancedbox��ԭͼ������ͼ��4��5Ϊbox��ԭͼ������ͼ
void DrawStateArea(IMAGE* imglist, Hero& hp) {
	StageProperty SP = hp.GetStageProp();
	Position P0 = { game_area_width + 60 ,game_area_height * 3 / 5 - 30 };
	Position P;
	for (int i = 1;i <= SP.smallbox;i++) {
		P.x = (i % 2 == 0) ? P0.x + 50 : P0.x;
		P.y = P0.y + floor((i - 1) / 2) * 50;
		putimage(P.x - imglist[1].getwidth() / 2, P.y - imglist[1].getheight() / 2, &imglist[1], SRCAND);
		putimage(P.x - imglist[0].getwidth() / 2, P.y - imglist[0].getheight() / 2, &imglist[0], SRCPAINT);
	}
	if (SP.enhanced_hook) {
		P = { screen_width - 45 ,screen_height / 3 + 60 };
		putimage(P.x - imglist[3].getwidth() / 2, P.y - imglist[3].getheight() / 2, &imglist[3], SRCAND);
		putimage(P.x - imglist[2].getwidth() / 2, P.y - imglist[2].getheight() / 2, &imglist[2], SRCPAINT);
	}
	if (SP.box) {
		P = { game_area_width + 60 ,game_area_height / 3 + 60 };
		putimage(P.x - imglist[5].getwidth() / 2, P.y - imglist[5].getheight() / 2, &imglist[5], SRCAND);
		putimage(P.x - imglist[4].getwidth() / 2, P.y - imglist[4].getheight() / 2, &imglist[4], SRCPAINT);
	}
	int linelength = 100 * hp.GetCurrentHP() / hero_HP;
	setcolor(RGB(189, 227, 212));	//����ǰ��ɫ�������������
	fillroundrect(game_area_width + 20, 170, game_area_width + 125, 180, 1, 1);
	setcolor(RGB(57, 219, 216));	//����ǰ��ɫ�������������
	line(game_area_width + 22, 175, game_area_width + 22 + linelength, 175);
}

void ShowResult(unsigned int box, unsigned int smallbox, int base_score) {
	// ����������ʽ
	const TCHAR* font_style = _T("Consolas");

	// ������ɫ
	COLORREF title_color1 = RGB(213, 159, 112); // STAGE����ɫ
	COLORREF title_color2 = RGB(209, 208, 128); // CLEAR����ɫ
	COLORREF text_color = RGB(191, 235, 218); // �ı�����ɫ
	COLORREF bg_color = RGB(39, 82, 91); // ��������ɫ

	// ���������ı��Ĵ�С
	const int title_size = 80;
	const int text_size = 40;

	// ���������ı���λ��
	const int title_x = screen_width / 2; // ����ĺ�����
	const int title_y1 = screen_height / 6; // STAGE��������
	const int title_y2 = title_y1 + title_size; // CLEAR��������
	const int text_x1 = screen_width / 4; // �����С����ĺ�����
	const int text_x2 = text_x1 + screen_width / 2; // �������ֵܷĺ�����
	const int text_y1 = screen_height / 2; // �����������
	const int text_y2 = text_y1 + text_size * 2; // С�����������
	const int text_y3 = text_y2 + text_size * 2; // �ֵܷ�������

	// ���ñ�����ɫ
	setbkmode(OPAQUE);
	setbkcolor(bg_color);
	cleardevice();

	// ���ñ��������
	settextstyle(title_size, 0, font_style);

	// ���ñ������ɫ
	settextcolor(title_color1);

	// ����Ļ����������ʾSTAGE
	outtextxy(title_x - textwidth(_T("STAGE")) / 2, title_y1, _T("STAGE"));

	// ���ñ������ɫ
	settextcolor(title_color2);

	// ����Ļ����������ʾCLEAR
	outtextxy(title_x - textwidth(_T("CLEAR")) / 2, title_y2, _T("CLEAR"));

	// �����ı�������
	settextstyle(text_size, 0, font_style);

	// �����ı�����ɫ
	settextcolor(text_color);

	// ����Ļ�м���ʾ�����С���������
	TCHAR box_text[20];
	TCHAR smallbox_text[20];
	_stprintf_s(box_text, _T("���䣺%u"), box);
	_stprintf_s(smallbox_text, _T("С���䣺%u"), smallbox);
	outtextxy(text_x1, text_y1, box_text);
	outtextxy(text_x1, text_y2, smallbox_text);

	// �����ܷ�
	int score = box * 1000 + smallbox * 500 + base_score;

	// ���嶯����֡���ͼ��ʱ��
	const int frames = 60;
	const int interval = 20;

	// ���嶯������ʼ�ͽ���ʱ��
	clock_t start_time = clock();
	clock_t end_time = start_time + frames * interval;

	// ���嶯���ĵ�ǰʱ��ͽ���
	clock_t current_time;
	double progress;

	// ���嶯���ĵ�ǰ�������ı�
	int current_score;
	TCHAR score_text[20];

	// ѭ�����Ŷ�����ֱ������Enter����ﵽ����ʱ��
	while (1)
	{
		// ��ȡ��ǰʱ��
		current_time = clock();

		// �ж��Ƿ�ﵽ����ʱ��
		if (current_time >= end_time)
		{
			// ���õ�ǰ����Ϊ�ܷ�
			current_score = score;
		}
		else
		{
			// ���㶯���Ľ���
			progress = (current_time - start_time) * 1.0 / (end_time - start_time);

			// ���㵱ǰ����
			current_score = base_score + round((score - base_score) * progress);
		}

		// ��ʽ����ǰ�������ı�
		_stprintf_s(score_text, _T("�ܷ֣�%d"), current_score);

		// ����Ļ�м���ʾ��ǰ����

		outtextxy(text_x2, text_y3, score_text);

		// ˢ����Ļ
		FlushBatchDraw();

		if (GetAsyncKeyState(0x0D))break;

		// �ӳ�һ��ʱ��
		Sleep(interval);
	}
}

bool Play() {
	setbkcolor(WHITE);
	cleardevice();	//�������ʹ�õ�ǰ����ɫ��ջ�ͼ�豸
	bool playing = true;

	IMAGE bkimg;

	loadimage(&bkimg, _T("./image/Background/background-final.png"));

	BK bk = BK(bkimg);

	LPCTSTR SCORE = _T("SCORE");
	LPCTSTR ETHER = _T("ETHER");
	LPCTSTR AIR = _T("AIR");

	//�������ֱ߾�
	int text_margin_left = state_area_width / 4;
	int text_margin_top = state_area_height / 40;
	int text_height = 30;

	settextstyle(text_height, 0, _T("Arial"));
	settextcolor(RGB(233, 255, 255));
	setbkmode(TRANSPARENT);

	int score = 0;	//������
	int extra_score = 0;	//�����
	LPCTSTR lpStr_score = IntToLPCTSTR(score);
	int ether = 0;
	LPCTSTR lpStr_ether = IntToLPCTSTR(ether);

	//��¼��������̫��������ʣ������������ߡ�ʰȡ���ı���
	RECT state_area;
	state_area.left = game_area_width;
	state_area.right = state_area.left + state_area_width;
	state_area.bottom = state_area_height;
	state_area.top = 0;
	
	//ʰȡ���ı����ڸ�������ʾ
	RECT box_area;
	box_area.left = state_area.left + 26;
	box_area.right = state_area.right - 26;
	box_area.bottom = state_area.bottom - 110;
	box_area.top = state_area.top + 330;
	//Ϊ�˼����ڴ�ʹ�ã�״̬��������ֻ��Ҫ��ʾͼƬ������Ҫ������Ӧ�Ķ���
	IMAGE state_area_img[6];
	IMAGE smallbox_image;
	IMAGE smallbox_mask;
	loadimage(&smallbox_image, _T("./image/prop/smallbox_i.png"));
	loadimage(&smallbox_mask, _T("./image/prop/smallbox_m.png"));
	state_area_img[0] = smallbox_image;
	state_area_img[1] = smallbox_mask;
	IMAGE enhancedhook_image;
	IMAGE enhancedhook_mask;
	loadimage(&enhancedhook_image, _T("./image/prop/enhanced_hook_i.png"));
	loadimage(&enhancedhook_mask, _T("./image/prop/enhanced_hook_m.png"));
	state_area_img[2] = enhancedhook_image;
	state_area_img[3] = enhancedhook_mask;
	IMAGE box_image;
	IMAGE box_mask;
	loadimage(&box_image, _T("./image/prop/box_i.png"));
	loadimage(&box_mask, _T("./image/prop/box_m.png"));
	state_area_img[4] = box_image;
	state_area_img[5] = box_mask;
	
	const int MaxOffsetY = 6500;	//��ͼ�ܳ���
	int OffsetY = 0;	//���ڼ�¼����ͼ�Ѿ���������·��
	scroll_speed = max_scroll_speed;

	bool upordown = 1;	//����Ϊ0������Ϊ1

	Hero hp = Hero();
	RECT hp_rect = hp.GetRect();

	//�洢��ǰ��������Ҫ��ʾ�ĵ���
	vector<Turtle*>* turtle_es = new vector<Turtle*>();
	vector<RouteNode*>* turtle_rnl = ReadRouteNodeList("./enemy_data/Turtle.txt");

	vector<Scaleph*>* scaleph_es = new vector<Scaleph*>();
	vector<RouteNode*>* scaleph_rnl = ReadRouteNodeList("./enemy_data/Scaleph.txt");

	vector<GreenFish*>* greenfish_es = new vector<GreenFish*>();
	vector<RouteNode*>* greenfish_rnl = ReadRouteNodeList("./enemy_data/GreenFish.txt");

	vector<RedFish*>* redfish_es = new vector<RedFish*>();
	vector<RouteNode*>* redfish_rnl = ReadRouteNodeList("./enemy_data/RedFish.txt");

	vector<Lobster*>* lobster_es = new vector<Lobster*>();
	vector<RouteNode*>* lobster_rnl = ReadRouteNodeList("./enemy_data/Lobster.txt");

	vector<Trichiurus*>* trichiurus_es = new vector<Trichiurus*>();
	vector<RouteNode*>* trichiurus_rnl = ReadRouteNodeList("./enemy_data/Trichiurus.txt");

	vector<LanternFish*>* lanternFish_es = new vector<LanternFish*>();
	vector<RouteNode*>* lanternFish_rnl = ReadRouteNodeList("./enemy_data/LanternFish.txt");

	//�洢��ǰ��������Ҫ��ʾ�ĵ���
	vector<InvincibleProp*>* invincibleprop_es = new vector<InvincibleProp*>();	//�޵е����ڵ��˱��������ʵ��䣬�ø�����Ϊ10%

	vector<OxygenBottle*>* oxygenbottle_es = new vector<OxygenBottle*>();	//����ƿ�ڵ��˱��������ʵ��䣬�ø�����Ϊ10%

	vector<Box*>* box_es = new vector<Box*>();
	vector<RouteNode*>* box_rnl = ReadRouteNodeList("./prop_data/Box.txt");

	Ether::SetImageAndMask();
	vector<Ether*>* ether_es = new vector<Ether*>();
	vector<RouteNode*>* ether_rnl = ReadRouteNodeList("./prop_data/Ether.txt");

	vector<SmallBox*>* smallbox_es = new vector<SmallBox*>();
	vector<RouteNode*>* smallbox_rnl = ReadRouteNodeList("./prop_data/SmallBox.txt");

	Stab::SetImageAndMask();
	vector<Stab*>* stab_es = new vector<Stab*>();
	vector<RouteNode*>* stab_rnl = ReadRouteNodeList("./prop_data/Stab.txt");

	vector<EnhancedHook*>* enhancedhook_es = new vector<EnhancedHook*>();
	vector<RouteNode*>* enhancedhook_rnl = ReadRouteNodeList("./prop_data/EnhancedHook.txt");

	//��Ҫ�ȵ���Brick�ľ�̬��Ա��������Brick��ͼƬ
	Brick::SetImageAndMask();
	vector<Wall*>* wall_es = new vector<Wall*>();
	vector<RouteNode*>* wall_rnl = ReadRouteNodeList("./wall_data/walls_position/Wall.txt");

	setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 5);
	setlinecolor(BLACK);

	clock_t start_time = clock();
	int preparation_period = 3000;

	bool if_complete = 0;

	while (playing) {
		GenerateObject(turtle_es, turtle_rnl, OffsetY, upordown);
		GenerateObject(scaleph_es, scaleph_rnl, OffsetY, upordown);
		GenerateObject(greenfish_es, greenfish_rnl, OffsetY, upordown);
		GenerateObject(redfish_es, redfish_rnl, OffsetY, upordown);
		GenerateObject(lobster_es, lobster_rnl, OffsetY, upordown);
		GenerateObject(trichiurus_es, trichiurus_rnl, OffsetY, upordown);
		GenerateObject(lanternFish_es, lanternFish_rnl, OffsetY, upordown);
		GenerateProp(ether_es, ether_rnl, OffsetY, upordown);
		GenerateProp(smallbox_es, smallbox_rnl, OffsetY, upordown);
		GenerateProp(enhancedhook_es, enhancedhook_rnl, OffsetY, upordown);
		GenerateProp(stab_es, stab_rnl, OffsetY, upordown);
		GenerateProp(box_es, box_rnl, OffsetY, upordown);
		GenerateObject(wall_es, wall_rnl, OffsetY, upordown);

		ether = (ether < hp.GetStageProp().ether) ? ether + 1 : ether;
		lpStr_ether = IntToLPCTSTR(ether);
		int new_score = hp.GetStageProp().ether * 100 + extra_score;
		score = (score < new_score) ? score + 10 : new_score;
		lpStr_score = IntToLPCTSTR(score);

		BeginBatchDraw();

		setbkcolor(WHITE);
		cleardevice();	//�������ʹ�õ�ǰ����ɫ��ջ�ͼ�豸
		bk.Show(upordown);
		OffsetY -= scroll_speed;
		if(upordown) hp.Attack();	//�ϸ�ʱ���ܹ���
		hp.Show();

		Show(turtle_es);
		Show(scaleph_es);
		Show(greenfish_es);
		Show(lobster_es);
		Show(trichiurus_es);
		Show(lanternFish_es);
		Show(redfish_es);

		Show(wall_es);

		setfillcolor(RGB(19, 69, 71));	//���������ɫ
		solidrectangle(state_area.left, state_area.top, state_area.right, state_area.bottom);
		setcolor(RGB(189, 227, 212));
		outtextxy(state_area.left + 0.5 * text_margin_left, state_area.top + text_margin_top, SCORE);
		outtextxy(state_area.left + 0.5 * text_margin_left, state_area.top + text_margin_top + text_height, lpStr_score);
		outtextxy(state_area.left + 0.5 * text_margin_left, state_area.top + text_margin_top + text_height * 2, ETHER);
		outtextxy(state_area.left + 0.5 * text_margin_left, state_area.top + text_margin_top + text_height * 3, lpStr_ether);
		outtextxy(state_area.left + 0.5 * text_margin_left, state_area.top + text_margin_top + text_height * 4, AIR);

		setcolor(RGB(208, 255, 248));	//����ǰ��ɫ�������������
		fillroundrect(box_area.left, box_area.top, box_area.right, box_area.bottom, 1, 1);

		Show(invincibleprop_es);
		Show(oxygenbottle_es);
		Show(ether_es);
		Show(smallbox_es);
		Show(enhancedhook_es);
		Show(stab_es);
		Show(box_es);

		DrawStateArea(state_area_img, hp);

		EndBatchDraw();

		hp.Control();

		if (hp.IfDead()) {
			cout << "Hero����" << endl;
			break;
		}

		//�洢��ǰ��������Ҫ�����ײ������ľ���
		vector<RECT*>rect_es;
		//�洢��ǰ��������Ҫ�����ײ�������λ��
		vector<Position*>position_es;

		ControlObject(turtle_es, turtle_rnl, hp);
		ControlObject(scaleph_es, scaleph_rnl, hp);
		ControlObject(greenfish_es, greenfish_rnl, hp);
		ControlObject(lobster_es, lobster_rnl, hp);
		ControlObject(trichiurus_es, trichiurus_rnl, hp);
		ControlObject(lanternFish_es, lanternFish_rnl, hp);
		ControlObject(wall_es, hp);
		AddRectAndPosition(turtle_es, rect_es, position_es);
		AddRectAndPosition(scaleph_es, rect_es, position_es);
		AddRectAndPosition(greenfish_es, rect_es, position_es);
		AddRectAndPosition(lobster_es, rect_es, position_es);
		AddRectAndPosition(trichiurus_es, rect_es, position_es);
		AddRectAndPosition(lanternFish_es, rect_es, position_es);
		AddRectAndPosition(wall_es, rect_es, position_es);
		ControlObject(redfish_es, redfish_rnl, rect_es, position_es, hp);

		ControlEnemyDeath(turtle_es, turtle_rnl, invincibleprop_es, oxygenbottle_es, extra_score);
		ControlEnemyDeath(scaleph_es, scaleph_rnl, invincibleprop_es, oxygenbottle_es, extra_score);
		ControlEnemyDeath(greenfish_es, greenfish_rnl, invincibleprop_es, oxygenbottle_es, extra_score);
		ControlEnemyDeath(redfish_es, redfish_rnl, invincibleprop_es, oxygenbottle_es, extra_score);
		ControlEnemyDeath(lobster_es, lobster_rnl, invincibleprop_es, oxygenbottle_es, extra_score);
		ControlEnemyDeath(trichiurus_es, trichiurus_rnl, invincibleprop_es, oxygenbottle_es, extra_score);
		ControlEnemyDeath(lanternFish_es, lanternFish_rnl, invincibleprop_es, oxygenbottle_es, extra_score);
		ControlObject(invincibleprop_es, hp);
		ControlObject(oxygenbottle_es, hp);
		ControlObject(ether_es, hp);
		ControlObject(smallbox_es, hp);
		ControlObject(enhancedhook_es, hp);
		ControlObject(stab_es, hp);
		ControlObject(box_es, hp);

		RemoveObject(turtle_es, turtle_rnl, upordown);
		RemoveObject(scaleph_es, scaleph_rnl, upordown);
		RemoveObject(greenfish_es, greenfish_rnl, upordown);
		RemoveObject(redfish_es, redfish_rnl, upordown);
		RemoveObject(lobster_es, lobster_rnl, upordown);
		RemoveObject(trichiurus_es, trichiurus_rnl, upordown);
		RemoveObject(lanternFish_es, lanternFish_rnl, upordown);
		RemoveObject(invincibleprop_es, upordown);
		RemoveObject(oxygenbottle_es, upordown);
		RemoveObject(ether_es, ether_rnl, upordown);
		RemoveObject(smallbox_es, smallbox_rnl, upordown);
		RemoveObject(enhancedhook_es, enhancedhook_rnl, upordown);
		RemoveObject(stab_es, stab_rnl, upordown);
		RemoveObject(box_es, box_rnl, upordown);
		RemoveObject(wall_es, wall_rnl, upordown);

		if ((OffsetY >= MaxOffsetY - game_area_height) && (upordown == 1)) {
			if (clock() - start_time < preparation_period) scroll_speed = 0;
			else scroll_speed = max_scroll_speed;
			if ((hp.GetStageProp().box > 0) && (box_es->begin() == box_es->end())) {
				hp.Startfloat();
				start_time = clock();
				if (clock() - start_time < preparation_period) {
					upordown = 0;
					scroll_speed = -max_scroll_speed;
				}
				else scroll_speed = 0;
			}
			else {
				scroll_speed = 0;
			}
		}
		else if ((OffsetY <= 0) && (upordown == 0)) {
			scroll_speed = 0;
			hp.CompleteFloat();
			if (hp.GetPositionY() < -50)break;
		}
		if (hp.IfDying()) scroll_speed = 0;
		Sleep(13);
	}

	//����Ϸ����ʱҪ�ֶ��ͷŶ������ݣ�
	ReleaseVector(turtle_es);
	ReleaseRouteNodeList(turtle_rnl);
	ReleaseVector(scaleph_es);
	ReleaseRouteNodeList(scaleph_rnl);
	ReleaseVector(greenfish_es);
	ReleaseRouteNodeList(greenfish_rnl);
	ReleaseVector(redfish_es);
	ReleaseRouteNodeList(redfish_rnl);
	ReleaseVector(lobster_es);
	ReleaseRouteNodeList(lobster_rnl);
	ReleaseVector(trichiurus_es);
	ReleaseRouteNodeList(trichiurus_rnl);
	ReleaseVector(lanternFish_es);
	ReleaseRouteNodeList(lanternFish_rnl);
	ReleaseVector(invincibleprop_es);
	ReleaseVector(oxygenbottle_es);
	ReleaseVector(ether_es);
	ReleaseRouteNodeList(ether_rnl);
	ReleaseVector(smallbox_es);
	ReleaseRouteNodeList(smallbox_rnl);
	ReleaseVector(enhancedhook_es);
	ReleaseRouteNodeList(enhancedhook_rnl);
	ReleaseVector(stab_es);
	ReleaseRouteNodeList(stab_rnl);
	ReleaseVector(box_es);
	ReleaseRouteNodeList(box_rnl);
	ReleaseVector(wall_es);
	ReleaseRouteNodeList(wall_rnl);
	//�ڳ������ʱ�ͷž�̬��Ա�����ĵ��ڴ�
	Brick::ReleaseMemory();
	Ether::ReleaseMemory();
	Stab::ReleaseMemory();

	ShowResult(hp.GetStageProp().box, hp.GetStageProp().smallbox, score);
	return true;
}

int main() {
	initgraph(screen_width, screen_height, EX_SHOWCONSOLE);

	bool is_playing = true;
	while (is_playing) {
		Welcome();
		is_playing = Play();
	}
	system("pause");
	closegraph();
	return 0;
}
